#define BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS

#include "podge.hpp"
#include "os/resources.hpp"

#include <stdexcept>
#include <tuple>
#include <functional>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <fstream>

#include <pugixml.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <boost/scope_exit.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/erase.hpp>

namespace podge {

// Fades out an SDL_Mixer channel upon destruction.
//
// The primary use case is when playing an infinite loop and 
// having the channel's lifetime be bound to the initiator of 
// that loop (e.g. an entity).
struct mix_channel {
	explicit mix_channel(int channel);

	mix_channel() = default;
	mix_channel(const mix_channel &) = delete;
	mix_channel(mix_channel &&o);

	mix_channel &operator =(const mix_channel &) = delete;
	mix_channel &operator =(mix_channel &&o);

	~mix_channel();

	int id() const;
	explicit operator bool() const;
	void clear();

	// Find an unused Mix channel.
	// Returns an empty mix_channel if all channels are in use, otherwise the free channel number.
	static mix_channel next();

private:
	boost::optional<int> id_;
};

mix_channel::mix_channel(int channel) :
	id_(channel)
{
}

mix_channel::mix_channel(mix_channel &&o) :
	id_(o.id_)
{
	o.id_ = boost::none;
}

mix_channel &mix_channel::operator =(mix_channel &&o) {
	id_ = o.id_;
	o.id_ = boost::none;
	return *this;
}

mix_channel::~mix_channel() {
	clear();
}

int mix_channel::id() const {
	return *id_;
}

mix_channel::operator bool() const {
	return bool(id_);
}

void mix_channel::clear() {
	if(id_) {
		if(Mix_Playing(*id_)) {
			Mix_FadeOutChannel(*id_, 500);
		}
	}
	id_ = boost::none;
}

// Find an unused Mix channel.
// Returns an empty mix_channel if all channels are in use, otherwise the free channel number.
mix_channel mix_channel::next() {
	for(auto i(0); i != PODGE_MIX_NUM_CHANNELS; ++i) {
		if(!Mix_Playing(i)) {
			return mix_channel(i);
		}
	}
	return {};
}

b2Vec2 to_b2Vec2(const glm::vec2 &v) {
	return {v.x, v.y};
}

glm::vec2 to_vec2(const b2Vec2 &v) {
	return {v.x, v.y};
}

b2AABB compute_body_bounds(b2Body *body) {
	assert(body->GetFixtureList() != nullptr);
	b2AABB aabb;
	aabb.lowerBound.x = std::numeric_limits<float>::infinity();
	aabb.lowerBound.y = std::numeric_limits<float>::infinity();
	aabb.upperBound.x = -std::numeric_limits<float>::infinity();
	aabb.upperBound.y = -std::numeric_limits<float>::infinity();
	b2AABB temp;
	const auto &xform(body->GetTransform());
	for(auto fixture(body->GetFixtureList()); fixture != nullptr; fixture = fixture->GetNext()) {
		auto shp(fixture->GetShape());
		for(auto i(0), n(shp->GetChildCount()); i != n; ++i) {
			shp->ComputeAABB(&temp, xform, i);
			aabb.Combine(temp);
		}
	}
	return aabb;
}

void to_nvg_xform(const glm::mat3 &m, float *xform) {
	xform[0] = m[0][0];
	xform[1] = m[0][1];
	xform[2] = m[1][0];
	xform[3] = m[1][1];
	xform[4] = m[2][0];
	xform[5] = m[2][1];
}

resource_path::resource_path() {
}

resource_path::resource_path(const std::string &path) {
	if(!path.empty()) {
		typedef boost::algorithm::split_iterator<std::string::const_iterator> string_split_iterator;
		string_split_iterator last;
		for(string_split_iterator it(boost::make_split_iterator<const std::string>(path, boost::algorithm::first_finder("/"))); it != last; ++it) {
			parts_.emplace_back(it->begin(), it->end());
		}
	}
}

resource_path::resource_path(const std::vector<std::string> &parts) :
	parts_(parts)
{
}

resource_path resource_path::parent() const {
	assert(!empty());
	return resource_path(std::vector<std::string>(parts_.begin(), parts_.begin() + parts_.size() - 1));
}

resource_path resource_path::canonical(const resource_path &base) const {
	assert(!empty());
	if(empty()) {
		// we reserve the empty resource_path to mean a non-existent path, so it should not be used with canonical().
		// if you want paths that refer to the current directory use resource_path(".").
		// we check this in release mode as well since this function is typically used rarely and this error can be hard to debug.
		PODGE_THROW_ERROR();
	}
	resource_path result(base);
	for(const auto &part : parts_) {
		if(part == "..") {
			result.parts_.pop_back();
		} else if(part != ".") {
			result.parts_.push_back(part);
		}
	}
	return result;
}

resource_path resource_path::operator /(const resource_path &path) const {
	resource_path result(*this);
	result.parts_.reserve(result.parts_.size() + path.parts_.size());
	std::copy(path.parts_.begin(), path.parts_.end(), std::back_inserter(result.parts_));
	return result;
}

resource_path resource_path::operator /(const std::string &path) const {
	return (*this)/resource_path(path);
}

std::string resource_path::str() const {
	std::ostringstream oss;
	for(auto it(parts_.begin()); it != parts_.end();) {
		oss << *it;
		++it;
		if(it != parts_.end()) {
			oss << "/";
		}
	}
	return oss.str();
}

bool resource_path::empty() const {
	return parts_.empty();
}

namespace ptr_transforms {

struct entity_to_entity {
	entity &operator()(const std::unique_ptr<entity> &e) const {
		return *e;
	}
};

struct const_entity_to_entity {
	const entity &operator()(const std::unique_ptr<entity> &e) const {
		return *e;
	}
};

struct const_shape_to_shape {
	const b2Shape *operator()(const std::unique_ptr<b2Shape> &shape) const {
		return shape.get();
	}
};

struct layer_to_layer {
	layer &operator()(const std::unique_ptr<layer> &e) const {
		return *e;
	}
};

struct const_layer_to_layer {
	const layer &operator()(const std::unique_ptr<layer> &e) const {
		return *e;
	}
};

}

static void check_xml_parse_result(const resource_path &path, const pugi::xml_parse_result &res) {
	if(!res) {
		std::ostringstream oss;
		oss << "Error parsing " << path.str() << " at offset " << res.offset << ": " << res.description();
		throw std::runtime_error(oss.str());
	}
}

podge_registry registry;

#define PODGE_REGISTER_COMPONENT(T) static ::podge::podge_registry_init PODGE_CONCAT(init_, __COUNTER__, _) (::podge::podge_registry_init::component_tag(), [](::podge::podge_registry_init::register_component_fn register_component) { \
	register_component.do_register<T>(registry); \
})
#define PODGE_REGISTER_SYSTEM(T) static ::podge::podge_registry_init PODGE_CONCAT(init_, __COUNTER__, _) (::podge::podge_registry_init::system_tag(), [](::podge::podge_registry_init::register_system_fn register_system) { \
	register_system(new T()); \
})
#define PODGE_REGISTER_CONTACT_HANDLER(T) static ::podge::podge_registry_init PODGE_CONCAT(init_, __COUNTER__, _) (::podge::podge_registry_init::contact_handler_tag(), [](::podge::podge_registry_init::register_contact_handler_fn register_contact_handler) { \
	register_contact_handler(new T()); \
})

PODGE_REGISTER_COMPONENT(core_component);
PODGE_REGISTER_COMPONENT(tile_component);
PODGE_REGISTER_COMPONENT(fixture_component);

collision_shape::collision_shape() {
	properties.add_component<fixture_component>();
}

namespace util {

static entity &entity_from_body(b2Body *body) {
	return *static_cast<entity *>(body->GetUserData());
}

// parses a comma-separated list of names
static void parse_names(const std::string &str, std::vector<std::string> &out) {
	std::string s(str);
	boost::erase_all(s, " ");
	auto first(boost::make_split_iterator(s, boost::first_finder(",")));
	decltype(first) last;
	for(auto it(first); it != last; ++it) {
		out.emplace_back(it->begin(), it->end());
	}
}

// Casts a ray from a point to all the shapes of a body and returns the closest point
// May return empty if the ray is cast from inside the body.
// The ray casting ignores any walls that may be between the point and the body (only ray casts against the body's shapes).
static boost::optional<b2Vec2> ray_cast(const b2Vec2 &from, b2Body *body) {
	auto dist_min(std::numeric_limits<float>::infinity());
	glm::vec2 dir_min;
	for(auto fixture(body->GetFixtureList()); fixture != nullptr; fixture = fixture->GetNext()) {
		auto shp(fixture->GetShape());
		for(auto i(0); i != shp->GetChildCount(); ++i) {
			b2AABB aabb;
			shp->ComputeAABB(&aabb, body->GetTransform(), i);
			b2RayCastInput in;
			in.p1 = from;
			in.p2 = aabb.GetCenter();
			in.maxFraction = 1.0f;
			b2RayCastOutput out;
			if(shp->RayCast(&out, in, body->GetTransform(), i)) {
				auto dir(to_vec2(in.p2) - to_vec2(in.p1));
				auto len(glm::length(dir));
				auto d(out.fraction*len);
				if(d < dist_min) {
					dir_min = dir/len;
					dist_min = d;
				}
			}
		}
	}
	if(std::isinf(dist_min)) {
		return {};
	} else {
		return to_b2Vec2(to_vec2(from) + dir_min*dist_min);
	}
}

// Applies a drag force to the entity's body.
// csl: The approximate cross-sectional length of the body (e.g. for a fixed-rotation body this could be the width).
static void apply_air_drag(entity &e, float csl) {
	static const float drag_constant(0.1f);
	auto vel(to_vec2(e.body()->GetLinearVelocity()));
	auto v(glm::length(vel));
	if(v > 0.0f) {
		auto drag(drag_constant*csl*v*v);
		auto fd(to_b2Vec2(-glm::normalize(vel)*drag));
		e.body()->ApplyForceToCenter(fd, true);
	}
}

// Applies a b2Transform to the current NanoVG transform matrix.
static void apply_transform(NVGcontext *vg, const b2Transform &t) {
	nvgTranslate(vg, t.p.x, t.p.y);
	nvgTransform(vg, t.q.c, t.q.s, -t.q.s, t.q.c, 0.0f, 0.0f);
}

// Draws a NanoVG path for a shape for the given child.
static void draw_shape(NVGcontext *vg, b2Shape *shp, int32 child_index) {
	static const auto draw_edge([](NVGcontext *vg, b2EdgeShape *edge) {
		nvgMoveTo(vg, edge->m_vertex1.x, edge->m_vertex1.y);
		nvgLineTo(vg, edge->m_vertex2.x, edge->m_vertex2.y);
	});
	nvgBeginPath(vg);
	switch(shp->GetType()) {
		case b2Shape::e_circle:
		{
			assert(child_index == 0);
			auto circ(static_cast<b2CircleShape *>(shp));
			nvgCircle(vg, circ->m_p.x, circ->m_p.y, circ->m_radius);
			break;
		}
		case b2Shape::e_edge:
		{
			assert(child_index == 0);
			draw_edge(vg, static_cast<b2EdgeShape *>(shp));
			break;
		}
		case b2Shape::e_polygon:
		{
			assert(child_index == 0);
			auto poly(static_cast<b2PolygonShape *>(shp));
			auto n(poly->GetVertexCount());
			if(n > 0) {
				const auto &v0(poly->GetVertex(0));
				nvgMoveTo(vg, v0.x, v0.y);
				for(auto i(1); i != n; ++i) {
					const auto &v(poly->GetVertex(i));
					nvgLineTo(vg, v.x, v.y);
				}
				nvgClosePath(vg);
			}
			break;
		}
		case b2Shape::e_chain:
		{
			b2EdgeShape edge;
			auto chain(static_cast<b2ChainShape *>(shp));
			chain->GetChildEdge(&edge, child_index);
			draw_edge(vg, &edge);
			break;
		}
		case b2Shape::e_typeCount: // not used: just meant to satisfy the enum switch compiler warning
		{
			assert(!"unexpected shape type");
			break;
		}
	}
}

static bool point_in_body(b2Body *body, const b2Vec2 &pt) {
	const auto &xform(body->GetTransform());
	for(auto fixture(body->GetFixtureList()); fixture != nullptr; fixture = fixture->GetNext()) {
		auto shp(fixture->GetShape());
		if(shp->TestPoint(xform, pt)) {
			return true;
		}
	}
	return false;
}

static std::vector<b2Vec2> parse_points(const char *points, float x0, float y0) {
	std::vector<b2Vec2> result;
	std::istringstream points_iss(points);
	while(!points_iss.eof()) {
		int dx_px, dy_px;
		points_iss >> dx_px;
		if(points_iss.get() != ',') {
			PODGE_THROW_ERROR();
		}
		points_iss >> dy_px;
		result.emplace_back(dx_px * pixels_to_meters + x0, -dy_px * pixels_to_meters + y0);
	}
	return result;
}

static std::pair<b2Vec2, b2Vec2> points_minmax(const std::vector<b2Vec2> &points) {
	std::pair<b2Vec2, b2Vec2> r;
	r.first.x = std::numeric_limits<float>::infinity();
	r.first.y = std::numeric_limits<float>::infinity();
	r.second.x = -std::numeric_limits<float>::infinity();
	r.second.y = -std::numeric_limits<float>::infinity();
	for(const auto &pt : points) {
		r.first.x = std::min(pt.x, r.first.x);
		r.first.y = std::min(pt.y, r.first.y);
		r.second.x = std::max(pt.x, r.second.x);
		r.second.y = std::max(pt.y, r.second.y);
	}
	return r;
}

static void scale_body(b2Body *body, const glm::vec2 &scale) {
	std::vector<std::unique_ptr<b2Shape>> shapes;
	std::vector<b2FixtureDef> defs;
	std::vector<b2Fixture *> old_fixtures;
	for(auto fixture(body->GetFixtureList()); fixture; fixture = fixture->GetNext()) {
		old_fixtures.emplace_back(fixture);
		b2FixtureDef def;
		def.userData = fixture->GetUserData();
		def.friction = fixture->GetFriction();
		def.restitution = fixture->GetRestitution();
		def.density = fixture->GetDensity();
		def.isSensor = fixture->IsSensor();
		def.filter = fixture->GetFilterData();
		auto handled(false);
		switch(fixture->GetType()) {
			case b2Shape::e_circle:
				{
					if(std::abs(scale.x - scale.y) >= std::numeric_limits<float>::epsilon()) {
						PODGE_THROW_ERROR();
					}
					auto shape(static_cast<b2CircleShape *>(fixture->GetShape()));
					std::unique_ptr<b2CircleShape> new_shape(new b2CircleShape());
					new_shape->m_p.x = shape->m_p.x * scale.x;
					new_shape->m_p.y = shape->m_p.y * scale.y;
					new_shape->m_radius = shape->m_radius * scale.x;
					def.shape = new_shape.get();
					shapes.emplace_back(std::move(new_shape));
					handled = true;
				}
				break;
			case b2Shape::e_edge:
				{
					auto shape(static_cast<b2EdgeShape *>(fixture->GetShape()));
					std::unique_ptr<b2EdgeShape> new_shape(new b2EdgeShape());
					new_shape->m_vertex1.x = shape->m_vertex1.x * scale.x;
					new_shape->m_vertex1.y = shape->m_vertex1.y * scale.y;
					new_shape->m_vertex2.x = shape->m_vertex2.x * scale.x;
					new_shape->m_vertex2.y = shape->m_vertex2.y * scale.y;
					if(shape->m_hasVertex0) {
						new_shape->m_hasVertex0 = true;
						new_shape->m_vertex0.x = shape->m_vertex0.x * scale.x;
						new_shape->m_vertex0.y = shape->m_vertex0.y * scale.y;
					}
					if(shape->m_hasVertex3) {
						new_shape->m_hasVertex3 = true;
						new_shape->m_vertex3.x = shape->m_vertex3.x * scale.x;
						new_shape->m_vertex3.y = shape->m_vertex3.y * scale.y;
					}
					def.shape = new_shape.get();
					shapes.emplace_back(std::move(new_shape));
					handled = true;
				}
				break;
			case b2Shape::e_polygon:
				{
					auto shape(static_cast<b2PolygonShape *>(fixture->GetShape()));
					std::unique_ptr<b2PolygonShape> new_shape(new b2PolygonShape());
					std::vector<b2Vec2> new_vertices(shape->m_vertices, shape->m_vertices + shape->m_count);
					for(auto &v : new_vertices) {
						v.x *= scale.x;
						v.y *= scale.y;
					}
					new_shape->Set(new_vertices.data(), new_vertices.size());
					def.shape = new_shape.get();
					shapes.emplace_back(std::move(new_shape));
					handled = true;
				}
				break;
			case b2Shape::e_chain:
				{
					auto shape(static_cast<b2ChainShape *>(fixture->GetShape()));
					std::unique_ptr<b2ChainShape> new_shape(new b2ChainShape());
					for(auto it(shape->m_vertices); it != shape->m_vertices + shape->m_count; ++it) {
						it->x *= scale.x;
						it->y *= scale.y;
					}
					if(shape->m_hasPrevVertex) {
						shape->m_prevVertex.x *= scale.x;
						shape->m_prevVertex.y *= scale.y;
					}
					if(shape->m_hasNextVertex) {
						shape->m_nextVertex.x *= scale.x;
						shape->m_nextVertex.y *= scale.y;
					}
					def.shape = new_shape.get();
					shapes.emplace_back(std::move(new_shape));
					handled = true;
				}
				break;
			case b2Shape::e_typeCount:
				break;
		}
		defs.emplace_back(std::move(def));
		assert(handled);
	}
	for(auto fixture : old_fixtures) {
		body->DestroyFixture(fixture);
	}
	for(auto &def : defs) {
		body->CreateFixture(&def);
	}
}

}

}

#define PODGE_SYSTEM_FWD
#include <systems.inl>
#undef PODGE_SYSTEM_FWD

#include <systems.inl>

#include <types.inl>

namespace podge {

resource_pool::resource_pool(NVGcontext *vg) :
	vg_(vg)
{
}

resource_pool::~resource_pool() {
	for(const auto &p : samples_) {
		Mix_FreeChunk(p.second);
	}
	for(const auto &p : images_) {
		nvgDeleteImage(vg_, p.second);
	}
}

const nm::json &resource_pool::load_json(const resource_path &path) const {
	auto pathstr(path.str());
	auto it(jsons_.find(pathstr));
	if(it != jsons_.end()) {
		return it->second;
	} else {
		auto data(get_resource(pathstr));
		auto p(jsons_.emplace(pathstr, nm::json::parse(data.begin(), data.end())));
		assert(p.second);
		return p.first->second;
	}
}

int resource_pool::load_image(const resource_path &path) const {
	auto pathstr(path.str());
	auto it(images_.find(pathstr));
	if(it != images_.end()) {
		return it->second;
	} else {
		auto data(get_resource(pathstr));
		auto ptr(const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(data.c_str())));
		auto flags(NVG_IMAGE_FLIPY); // we flip Y because our convention is to specify image coordinates from bottom-left to top-right (normally NanoVG expects top-left to bottom-right)
		auto res(nvgCreateImageMem(vg_, flags, ptr, data.size()));
		if(res == 0) {
			PODGE_THROW_ERROR();
		}
		auto r(images_.emplace(pathstr, res));
		assert(r.second);
		return res;
	}
}

Mix_Chunk *resource_pool::load_sample(const resource_path &path) const {
	auto pathstr(path.str());
	auto it(samples_.find(pathstr));
	if(it != samples_.end()) {
		return it->second;
	} else {
		auto data(get_resource(pathstr));
		auto rw(SDL_RWFromConstMem(data.c_str(), data.size()));
		BOOST_SCOPE_EXIT(rw) {
			SDL_RWclose(rw);
		} BOOST_SCOPE_EXIT_END
		auto sample(Mix_LoadWAV_RW(rw, 0));
		if(sample == nullptr) {
			PODGE_THROW_MIX_ERROR();
		}
		auto r(samples_.emplace(pathstr, sample));
		assert(r.second);
		return sample;
	}
}

validation_error::validation_error(std::string msg) :
	msg(std::move(msg))
{
}

static glm::vec4 parse_color(const char *value) {
	auto len(strlen(value));
	if(len < 1 || value[0] != '#') {
		PODGE_THROW_ERROR();
	}
	glm::vec4 col;
	if(len == 7) {
		col[3] = 1.0f;
		col[0] = float(strtol(std::string(value+1, 2).c_str(), nullptr, 16))/255.0f;
		col[1] = float(strtol(std::string(value+3, 2).c_str(), nullptr, 16))/255.0f;
		col[2] = float(strtol(std::string(value+5, 2).c_str(), nullptr, 16))/255.0f;
	} else if(len == 9) {
		col[3] = float(strtol(std::string(value+1, 2).c_str(), nullptr, 16))/255.0f;
		col[0] = float(strtol(std::string(value+3, 2).c_str(), nullptr, 16))/255.0f;
		col[1] = float(strtol(std::string(value+5, 2).c_str(), nullptr, 16))/255.0f;
		col[2] = float(strtol(std::string(value+7, 2).c_str(), nullptr, 16))/255.0f;
	} else {
		PODGE_THROW_ERROR();
	}
	return col;
}

void object_component::validate(const context &ctx) const {
}	

object::object(const object &o) {
	for(const auto &p : o) {
		auto res(emplace(p.first, p.second->clone()));
		assert(res.second);
	}
}

object &object::operator =(const object &o) {
	clear();
	for(const auto &p : o) {
		auto res(emplace(p.first, p.second->clone()));
		assert(res.second);
	}
	return *this;
}

object_component &object::component(std::type_index cidx) {
	auto it(find(cidx));
	assert(it != end());
	return *it->second;
}

const object_component &object::component(std::type_index cidx) const {
	auto it(find(cidx));
	assert(it != end());
	return *it->second;
}

bool object::has_component(std::type_index cidx) const {
	return find(cidx) != end();
}

void object::load_properties_xml(pugi::xml_node properties_node, const resource_path &cwd) {
	auto property_xpns(properties_node.select_nodes("./property"));
	for(auto property_xpn : property_xpns) {
		auto property_node(property_xpn.node());
		auto type(property_node.attribute("type").value());
		auto name(property_node.attribute("name").value());
		auto value(property_node.attribute("value").value());
		if(strcmp(type, "bool") == 0) {
			registry.set_public_component_property(*this, name, strcmp(value, "true") == 0);
		} else if(strcmp(type, "color") == 0) {
			registry.set_public_component_property(*this, name, parse_color(value));
		} else if(strcmp(type, "float") == 0) {
			registry.set_public_component_property(*this, name, strtof(value, nullptr));
		} else if(strcmp(type, "file") == 0) {
			resource_path path(value);
			if(path.empty()) {
				registry.set_public_component_property(*this, name, resource_path());
			} else {
				registry.set_public_component_property(*this, name, path.canonical(cwd));
			}
		} else if(strcmp(type, "int") == 0) {
			registry.set_public_component_property(*this, name, int(strtol(value, nullptr, 10)));
		} else {
			registry.set_public_component_property(*this, name, std::string(value));
		}
	}
}

std::vector<std::type_index> entity_system::components() const {
	return {};
}

void entity_system::init(entity &e) const {
}

void entity_system::add(entity &e) const {
}

void entity_system::step(entity &e) const  {
}

bool entity_system::should_handle_input_last() const {
	return false;
}

bool entity_system::handle_input(entity &e, const input &in) const {
	return false;
}

bool entity_system::visible(const entity &e) const {
	return true;
}

void entity_system::render(const entity &e) const {
}

entity_type::entity_type(const char *name, bool has_body, std::vector<const entity_system *> systems) :
	name_(name),
	has_body_(has_body),
	systems_(std::move(systems))
{
}

const char *entity_type::name() const {
	return name_;
}

bool entity_type::has_body() const {
	return has_body_;
}

const std::vector<const entity_system *> &entity_type::systems() const {
	return systems_;
}


entity_spec entity_spec::from_tile_xml(pugi::xml_node tile_node, pugi::xml_node tileset_node, const resource_path &cwd) {
	const auto &type(registry.type(tile_node.attribute("type").value()));
	entity_spec spec(type);
	spec.add_component<core_component>();
	spec.add_component<tile_component>();
	auto &cc(spec.component<core_component>());
	auto tileset_image_xpn(tileset_node.select_node("./image"));
	if(tileset_image_xpn) {
		// spritesheets are not supported (once vector graphics and animations supported is added they will be useless)
		PODGE_THROW_ERROR();
	}
	float w, h;
	auto tile_image_xpn(tile_node.select_node("./image"));
	assert(tile_image_xpn);
	auto tile_image_node(tile_image_xpn.node());
	w = tile_image_node.attribute("width").as_int() * pixels_to_meters;
	h = tile_image_node.attribute("height").as_int() * pixels_to_meters;
	cc.width = w;
	cc.height = h;
	cc.flip_horizontal = false;
	cc.flip_vertical = false;
	// load properties
	auto properties_xpn(tile_node.select_node("./properties"));
	if(properties_xpn) {
		spec.load_properties_xml(properties_xpn.node(), cwd);
	}
	// load collision shapes
	auto objectgroup_xpn(tile_node.select_node("./objectgroup"));
	if(objectgroup_xpn) {
		auto objectgroup_node(objectgroup_xpn.node());
		auto object_xpns(objectgroup_node.select_nodes("./object"));
		cc.collision_shapes.reserve(object_xpns.size());
		for(auto object_xpn : object_xpns) {
			auto object_node(object_xpn.node());
			auto x(object_node.attribute("x").as_float() * pixels_to_meters - w/2.0f);
			auto y(h/2.0f - object_node.attribute("y").as_float() * pixels_to_meters);
			auto rng(object_node.children());
			std::size_t num_not_properties(0);
			for(auto child : rng) {
				if(strcmp(child.name(), "properties") != 0) {
					++num_not_properties;
				}
			}
			auto cs(std::make_shared<collision_shape>());
			{
				auto properties_xpn(object_node.select_node("./properties"));
				if(properties_xpn) {
					cs->properties.load_properties_xml(properties_xpn.node(), cwd);
				}
			}
			if(num_not_properties == 0) { // rectangle
				std::unique_ptr<b2PolygonShape> shp(new b2PolygonShape());
				auto obj_width(object_node.attribute("width").as_float() * pixels_to_meters);
				auto obj_height(object_node.attribute("height").as_float() * pixels_to_meters);
				b2Vec2 center(x + obj_width/2.0f, y - obj_height/2.0f);
				shp->SetAsBox(obj_width/2.0f, obj_height/2.0f, center, 0.0f);
				cs->shape = std::move(shp);
			} else if(object_node.select_node("./ellipse")) { // circle 
				std::unique_ptr<b2CircleShape> shp(new b2CircleShape());
				auto obj_width(object_node.attribute("width").as_float() * pixels_to_meters);
				auto obj_height(object_node.attribute("height").as_float() * pixels_to_meters);
				if(obj_width != obj_height) {
					PODGE_THROW_ERROR();
				}
				shp->m_p.Set(x + obj_width/2.0f, y - obj_height/2.0f);
				shp->m_radius = obj_width/2.0f;
				cs->shape = std::move(shp);
			} else if(object_node.select_node("./polygon")) { // polygon
				std::unique_ptr<b2PolygonShape> shp(new b2PolygonShape());
				auto polygon_node(object_node.select_node("./polygon").node());
				auto points(util::parse_points(polygon_node.attribute("points").value(), x, y));
				shp->Set(points.data(), points.size());
				cs->shape = std::move(shp);
			} else if(object_node.select_node("./polyline")) { // polyline
				std::unique_ptr<b2ChainShape> shp(new b2ChainShape());
				auto polyline_node(object_node.select_node("./polyline").node());
				auto points(util::parse_points(polyline_node.attribute("points").value(), x, y));
				shp->CreateChain(points.data(), points.size());
				cs->shape = std::move(shp);
			} else {
				PODGE_THROW_ERROR();
			}
			assert(cs->shape);
			cc.collision_shapes.emplace_back(std::move(cs));
		}
	}
	return spec;
}

entity_spec entity_spec::from_object_xml(pugi::xml_node object_node, const resource_path &cwd) {
	const auto &type(registry.type(object_node.attribute("type").value()));
	entity_spec spec(type);
	spec.add_component<core_component>();
	auto &cc(spec.component<core_component>());
	cc.flip_horizontal = false;
	cc.flip_vertical = false;
	auto properties_xpn(object_node.select_node("./properties"));
	if(properties_xpn) {
		spec.load_properties_xml(properties_xpn.node(), cwd);
	}
	auto &collision_shapes(cc.collision_shapes);
	if(object_node.select_node("./polygon") || object_node.select_node("./polyline")) {
		auto polygon_xpn(object_node.select_node("./polygon"));
		auto polyline_xpn(object_node.select_node("./polyline"));
		auto points(util::parse_points(polygon_xpn 
			? polygon_xpn.node().attribute("points").value() 
			: polyline_xpn.node().attribute("points").value(), 0.0f, 0.0f));
		auto mm(util::points_minmax(points));
		auto w(mm.second.x - mm.first.x);
		auto h(mm.second.y - mm.first.y);
		auto dx(-w/2.0f);
		auto dy(h/2.0f);
		for(auto &p : points) {
			p.x += dx;
			p.y += dy;
		}
		cc.width = w;
		cc.height = h;
		if(polygon_xpn) {
			auto cs(std::make_shared<collision_shape>());
			std::unique_ptr<b2PolygonShape> shp(new b2PolygonShape());
			shp->Set(points.data(), points.size());
			cs->shape = std::move(shp);
			collision_shapes.emplace_back(std::move(cs));
		} else {
			auto cs(std::make_shared<collision_shape>());
			std::unique_ptr<b2ChainShape> shp(new b2ChainShape());
			shp->CreateChain(points.data(), points.size());
			cs->shape = std::move(shp);
			collision_shapes.emplace_back(std::move(cs));
		}
	} else if(object_node.select_node("./point")) {
		cc.width = 0.0f;
		cc.height = 0.0f;
	} else { // rectangle or circle
		bool ellipse(object_node.select_node("./ellipse"));
		auto w(object_node.attribute("width").as_float() * pixels_to_meters);
		auto h(object_node.attribute("height").as_float() * pixels_to_meters);
		cc.width = w;
		cc.height = h;
		if(ellipse && w != h) {
			PODGE_THROW_ERROR();
		}
		if(ellipse) {
			auto cs(std::make_shared<collision_shape>());
			std::unique_ptr<b2CircleShape> shp(new b2CircleShape());
			shp->m_radius = w/2.0f;
			cs->shape = std::move(shp);
			collision_shapes.emplace_back(std::move(cs));
		} else {
			auto cs(std::make_shared<collision_shape>());
			std::unique_ptr<b2PolygonShape> shp(new b2PolygonShape());
			shp->SetAsBox(w/2.0f, h/2.0f);
			cs->shape = std::move(shp);
			collision_shapes.emplace_back(std::move(cs));
		}
	}
	return spec;
}

entity_spec::entity_spec(const entity_type &type) : 
	type_(&type),
	z_index_(0),
	visible_(true)
{
	std::unordered_set<std::type_index> cidxs;
	for(auto s : type.systems()) {
	   for(auto cidx : s->components()) {
		   cidxs.insert(cidx);
	   }
	}
	for(auto cidx : cidxs) {
		registry.add_component_to(*this, cidx);
	}
}

const entity_type &entity_spec::type() const {
	return *type_;
}

void entity_spec::z_index(int val) {
	z_index_ = val;
}

int entity_spec::z_index() const {
	return z_index_;
}

void entity_spec::visible(bool val) {
	visible_ = val;
}

bool entity_spec::visible() const {
	return visible_;
}

entity::entity(const entity_spec &spec, layer &lyr) :
	entity_spec(spec),
	lyr_(lyr),
	added_(false),
	remove_(false)
{
	auto &lvl(level::current());
	if(type().has_body()) {
		b2BodyDef def;
		def.type = b2_dynamicBody;
		body_.emplace(lvl.world().CreateBody(&def));
		body()->SetUserData(this);
		extra_body_data_.emplace();
	}
	for(auto s : type().systems()) {
		s->init(*this);
	}
}

entity::~entity() {
	auto &lvl(level::current());
	if(body_) {
		lvl.world().DestroyBody(*body_);
	}
}

bool entity::has_body() const {
	return bool(body_);
}

b2Body *entity::body() {
	return *body_;
}

const b2Body *entity::body() const {
	return *body_;
}

object &entity::fixture_data(b2Fixture *fixture) {
	assert(has_body());
	assert(fixture->GetBody() == body());
	if(fixture->GetUserData() != nullptr) {
		return *static_cast<object *>(fixture->GetUserData());
	}
	std::unique_ptr<object> obj(new object());
	obj->add_component<fixture_component>();
	auto ptr(obj.get());
	extra_body_data_->fixture_data.emplace_back(std::move(obj));
	fixture->SetUserData(ptr);
	return *ptr;
}

const object &entity::fixture_data(b2Fixture *fixture) const {
	static object empty;
	assert(has_body());
	assert(fixture->GetBody() == body());
	if(fixture->GetUserData() != nullptr) {
		return *static_cast<object *>(fixture->GetUserData());
	} else {
		return empty;
	}
}

void entity::translate(const glm::vec2 &offset) {
	if(has_body()) {
		const auto &pos(body()->GetPosition());
		body()->SetTransform({pos.x + offset.x, pos.y + offset.y}, body()->GetAngle());
	}
}

layer &entity::lyr() {
	return lyr_;
}

const layer &entity::lyr() const {
	return lyr_;
}

void entity::bind(entity &e) {
	e.incoming_binds_.emplace(this);
	outgoing_binds_.emplace(&e);
}

void entity::unbind(entity &e) {
	outgoing_binds_.erase(&e);
	e.incoming_binds_.erase(this);
}

void entity::remove() {
	remove_ = true;
	for(auto e : incoming_binds_) {
		e->outgoing_binds_.erase(this);
		e->remove();
	}
	for(auto e : outgoing_binds_) {
		e->incoming_binds_.erase(this);
	}
}

bool entity::marked_removed_() const {
	return remove_;
}

void entity::mark_added_() {
	added_ = true;
}

bool entity::marked_added_() const {
	return added_;
}

entity_contact::entity_contact(b2Contact *contact, bool swap) :
	contact_(contact),
	swap_(swap)
{
}

entity &entity_contact::entity_a() {
	return util::entity_from_body(fixture_a()->GetBody());
}

entity &entity_contact::entity_b() {
	return util::entity_from_body(fixture_b()->GetBody());
}

entity_contact_world_manifold entity_contact::world_manifold() const {
	entity_contact_world_manifold res;
	b2WorldManifold wm;
	contact_->GetWorldManifold(&wm);
	res.normal = swap_ ? -wm.normal : wm.normal;
	std::memcpy(res.points, wm.points, sizeof res.points);
	std::memcpy(res.separations, wm.separations, sizeof res.separations);
	res.point_count = contact_->GetManifold()->pointCount;
	return res;
}

b2Vec2 entity_contact::normal() const {
	return world_manifold().normal;
}

b2Vec2 entity_contact::point() const {
	auto wm(world_manifold());
	assert(wm.point_count > 0);
	b2Vec2 res(0.0f, 0.0f);
	for(auto i(0); i != wm.point_count; ++i) {
		res.x += wm.points[i].x;
		res.y += wm.points[i].y;
	}
	return res;
}

bool entity_contact::touching() const {
	return contact_->IsTouching();
}

void entity_contact::enabled(bool flag) {
	contact_->SetEnabled(flag);
}

bool entity_contact::enabled() const {
	return contact_->IsEnabled();
}

b2Fixture *entity_contact::fixture_a() {
	return swap_ ? contact_->GetFixtureB() : contact_->GetFixtureA();
}

b2Fixture *entity_contact::fixture_b() {
	return swap_ ? contact_->GetFixtureA() : contact_->GetFixtureB();
}

void entity_contact::friction(float32 val) {
	contact_->SetFriction(val);
}

float32 entity_contact::friction() const {
	return contact_->GetFriction();
}

void entity_contact::reset_friction() {
	contact_->ResetFriction();
}

void entity_contact::restitution(float32 val) {
	contact_->SetRestitution(val);
}

float32 entity_contact::restitution() const {
	return contact_->GetRestitution();
}

void entity_contact::reset_restitution() {
	contact_->ResetRestitution();
}

void entity_contact::tangent_speed(float32 speed) {
	contact_->SetTangentSpeed(speed);
}

float32 entity_contact::tangent_speed() const {
	return contact_->GetTangentSpeed();
}

bool entity_contact_handler::should_collide(entity &a, entity &b) const {
	return true;
}

void entity_contact_handler::begin_contact(entity_contact &contact) const {
}

void entity_contact_handler::end_contact(entity &a, entity &b) const {
}

void entity_contact_handler::pre_solve(entity_contact &contact) const {
}

void entity_contact_handler::post_solve(entity_contact &contact, const b2ContactImpulse *impulse) const {
}

void podge_registry::add_component_to(object &obj, std::type_index cidx) const {
	auto it(component_adders_.find(cidx));
	assert(it != component_adders_.end());
	it->second(obj);
}

bool podge_registry::is_component_public(std::type_index cidx) const {
	return public_components_.find(cidx) != public_components_.end();
}

boost::optional<std::type_index> podge_registry::property_public_component(const std::string &prop) const {
	auto it(prop_to_public_component_.find(prop));
	if(it == prop_to_public_component_.end()) {
		return boost::none;
	} else {
		return it->second;
	}
}

const std::vector<podge_registry::public_component_property> &podge_registry::public_component_properties(std::type_index cidx) const {
	auto it(public_component_properties_.find(cidx));
	assert(it != public_component_properties_.end());
	return it->second;
}

const entity_type &podge_registry::type(const char *name) const {
	if(strlen(name) == 0) {
		name = "placeholder";
	}
	return check_iterator(types_.find(boost::string_view(name)), types_.end())->second;
}

const entity_system &podge_registry::system(const char *name) const {
	return *check_iterator(systems_.find(boost::string_view(name)), systems_.end())->second;
}

bool podge_registry::type_exists(const char *name) const {
	return types_.find(boost::string_view(name)) != types_.end();
}

bool podge_registry::system_exists(const char *name) const {
	return systems_.find(boost::string_view(name)) != systems_.end();
}

bool podge_registry::ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	if(!b2ContactFilter::ShouldCollide(fixtureA, fixtureB)) {
		return false;
	}
	auto &a(util::entity_from_body(fixtureA->GetBody()));
	auto &b(util::entity_from_body(fixtureB->GetBody()));
	// special cases
	if(a.marked_removed_() || b.marked_removed_()) {
		return false;
	}
	// type-specific handling
	auto res(true);
	iterate_handlers(a, b, [&](const entity_contact_handler &handler, bool swap) {
		if(swap) {
			res = handler.should_collide(b, a);
		} else {
			res = handler.should_collide(a, b);
		}
	});
	return res;
}

void podge_registry::BeginContact(b2Contact *contact) {
	auto &a(util::entity_from_body(contact->GetFixtureA()->GetBody()));
	auto &b(util::entity_from_body(contact->GetFixtureB()->GetBody()));
	iterate_handlers(a, b, [&](const entity_contact_handler &handler, bool swap) {
		entity_contact c(contact, swap);
		handler.begin_contact(c);
	});
}

void podge_registry::EndContact(b2Contact *contact) {
	auto &a(util::entity_from_body(contact->GetFixtureA()->GetBody()));
	auto &b(util::entity_from_body(contact->GetFixtureB()->GetBody()));
	iterate_handlers(a, b, [&](const entity_contact_handler &handler, bool swap) {
		if(swap) {
			handler.end_contact(b, a);
		} else {
			handler.end_contact(a, b);
		}
	});
}

void podge_registry::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	auto &a(util::entity_from_body(contact->GetFixtureA()->GetBody()));
	auto &b(util::entity_from_body(contact->GetFixtureB()->GetBody()));
	iterate_handlers(a, b, [&](const entity_contact_handler &handler, bool swap) {
		entity_contact c(contact, swap);
		handler.pre_solve(c);
	});
}

void podge_registry::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	auto &a(util::entity_from_body(contact->GetFixtureA()->GetBody()));
	auto &b(util::entity_from_body(contact->GetFixtureB()->GetBody()));
	iterate_handlers(a, b, [&](const entity_contact_handler &handler, bool swap) {
		entity_contact c(contact, swap);
		handler.post_solve(c, impulse);
	});
}

void podge_registry_init::register_system_fn::operator()(entity_system *s) const {
	auto res(registry.systems_.emplace(s->name(), s).second);
	assert(res);
}

void podge_registry_init::register_contact_handler_fn::operator()(entity_contact_handler *handler) const {
	auto mask(handler->mask());
	std::shared_ptr<entity_contact_handler> ptr(handler);
	for(const auto &types : mask) {
		switch(types.size()) {
			case 1:
				registry.wildcard_contact_handlers_[types[0]].emplace_back(ptr);
				break;
			case 2:
				registry.exact_contact_handlers_[std::make_pair(boost::string_view(types[0]), boost::string_view(types[1]))].emplace_back(ptr);
				break;
			default:
				PODGE_THROW_ERROR();
		}
	}
}

void podge_registry_init::register_type_fn::operator()(const char *name, bool has_body, std::initializer_list<const char *> systems) const {
	std::vector<const entity_system *> v;
	for(auto sn : systems) {
		v.emplace_back(&registry.system(sn));
	}
	auto res(registry.types_.emplace(name, entity_type(name, has_body, std::move(v))).second);
	assert(res);
}

void tileset::add_entity_spec(unsigned int id, const entity_spec &spec) {
	specs_.emplace(id, spec);
}

const entity_spec &tileset::spec(unsigned int id) const {
	return check_iterator(specs_.find(id), specs_.end())->second;
}

struct id_iterator_transform {
	template <typename T>
	unsigned int operator()(T &&p) const {
		return p.first;
	}
};

tileset::id_iterator tileset::begin() const {
	return id_iterator(boost::make_transform_iterator<id_iterator_transform>(specs_.begin()));
}

tileset::id_iterator tileset::end() const {
	return id_iterator(boost::make_transform_iterator<id_iterator_transform>(specs_.end()));
}

tileset::id_iterator tileset::rbegin() const {
	return id_iterator(boost::make_transform_iterator<id_iterator_transform>(specs_.rbegin()));
}

tileset::id_iterator tileset::rend() const {
	return id_iterator(boost::make_transform_iterator<id_iterator_transform>(specs_.rend()));
}

std::size_t tileset::size() const {
	return specs_.size();
}

void tileset::load_xml(pugi::xml_node tsx_node, const resource_path &cwd) {
	auto tileset_node(tsx_node.select_node("/tileset").node());
	auto tile_xpns(tsx_node.select_nodes("/tileset/tile"));
	for(auto tile_xpn : tile_xpns) {
		auto tile_node(tile_xpn.node());
		auto spec(entity_spec::from_tile_xml(tile_node, tileset_node, cwd));
		auto &tc(spec.component<tile_component>());
		tc.source = this;
		auto id(tile_node.attribute("id").as_uint());
		specs_.emplace(id, std::move(spec));
	}
}

const entity_spec &tileset_map::spec(int gid) const {
	auto it(find(gid));
	check_iterator(it, end());
	const auto &ts(*it->second);
	return ts.spec(gid - it->first.lower());
}

std::unique_ptr<layer> layer::from_tile_layer_xml(pugi::xml_node layer_node, const resource_path &cwd) {
	auto &lvl(level::current());
	assert(layer_node.attribute("width").as_int() == lvl.width());
	assert(layer_node.attribute("height").as_int() == lvl.height());
	std::unique_ptr<layer> lyr(new layer(layer_node.attribute("name").value()));
	auto width(lvl.width());
	auto height(lvl.height());
	auto data_node(layer_node.select_node("./data").node());
	if(std::strcmp(data_node.attribute("encoding").value(), "csv") != 0) {
		throw std::runtime_error("Only 'csv' encoding is allowed");
	}
	auto data(data_node.child_value());
	std::istringstream data_ss(data);
	std::string t;
	for(auto j(0); j != height; ++j) {
		for(auto i(0); i != width; ++i) {
			std::getline(data_ss, t, ',');
			auto gid(std::atoi(t.c_str()));
			if(gid != 0) {
				const auto &spec(lvl.tilesets().spec(gid));
				auto &cc(spec.component<core_component>());
				glm::vec2 pos(float(i) - 0.5f + cc.width/2.0f, float(height - j) - 1.5f + cc.height/2.0f);
				lyr->create_entity(pos, spec);
			}
		}
		if(j != height-1) {
			std::getline(data_ss, t);
		}
	}
	return lyr;
}

std::unique_ptr<layer> layer::from_object_layer_xml(pugi::xml_node objectgroup_node, const resource_path &cwd) {
	auto &lvl(level::current());
	std::unique_ptr<layer> lyr(new layer(objectgroup_node.attribute("name").value()));
	for(auto object_xpn : objectgroup_node.select_nodes("./object")) {
		auto object_node(object_xpn.node());
		auto x(object_node.attribute("x").as_float() * pixels_to_meters);
		auto y(object_node.attribute("y").as_float() * pixels_to_meters);
		glm::vec2 P(x, lvl.height() - y);
		glm::vec2 Pc;
		boost::optional<entity_spec> spec;
		boost::optional<glm::vec2> scale;
		if(!object_node.attribute("gid").empty()) {
			auto gid_with_flags(object_node.attribute("gid").as_uint());
			const auto &ts_spec(lvl.tilesets().spec(gid_with_flags & ~tiled_gid_flags));
			spec.emplace(ts_spec);
			auto &cc(spec->component<core_component>());
			cc.flip_horizontal = gid_with_flags & tiled_gid_flip_horizontal;
			cc.flip_vertical = gid_with_flags & tiled_gid_flip_vertical;
			auto properties_xpn(object_node.select_node("./properties"));
			if(properties_xpn) {
				spec->load_properties_xml(properties_xpn.node(), cwd);
			}
			Pc = glm::vec2(P.x + cc.width/2.0f, P.y + cc.height/2.0f); // tile objects' (x, y) refers to the bottom-left point on the object
			const auto &ts_cc(ts_spec.component<core_component>());
			scale = glm::vec2(cc.width/ts_cc.width, cc.height/ts_cc.height);
		} else {
			spec = entity_spec::from_object_xml(object_node, cwd);
			auto &cc(spec->component<core_component>());
			Pc = glm::vec2(P.x + cc.width/2.0f, P.y - cc.height/2.0f); // non-tile objects' (x, y) refers to the top-left point on the object
		}
		auto angle(0.0f);
		{
			auto rotation_attr(object_node.attribute("rotation"));
			if(rotation_attr) {
				angle = -glm::radians(rotation_attr.as_float());
			}
		}
		auto D(glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(Pc - P, 0.0f, 1.0f));
		x = P.x + D.x - 0.5f;
		y = P.y + D.y - 0.5f;
		auto &e(lyr->create_entity({x, y}, *spec));
		auto &e_cc(e.component<core_component>());
		if(!object_node.attribute("name").empty()) {
			e_cc.name.emplace(object_node.attribute("name").value());
		}
		if(e.has_body()) {
			e.body()->SetTransform(e.body()->GetPosition(), angle);
			if(scale) {
				util::scale_body(e.body(), *scale);
			}
		}
	}
	return lyr;
}

layer::layer(const std::string &name) :
	index_(std::numeric_limits<std::size_t>::max()),
	name_(name)
{
}

std::size_t layer::index() const {
	return index_;
}

const std::string &layer::name() const {
	return name_;
}

boost::iterator_range<layer::entity_iterator> layer::entities() {
	return {
		entity_iterator(boost::make_transform_iterator<ptr_transforms::entity_to_entity>(entities_.begin())),
		entity_iterator(boost::make_transform_iterator<ptr_transforms::entity_to_entity>(entities_.end()))
	};
}

boost::iterator_range<layer::const_entity_iterator> layer::entities() const {
	return {
		const_entity_iterator(boost::make_transform_iterator<ptr_transforms::const_entity_to_entity>(entities_.begin())),
		const_entity_iterator(boost::make_transform_iterator<ptr_transforms::const_entity_to_entity>(entities_.end()))
	};
}

entity &layer::create_entity(const glm::vec2 &pos, const entity_spec &spec) {
	auto e(new entity(spec, *this));
	entities_.emplace_back(std::unique_ptr<entity>(e));
	e->translate(pos);
	return *e;
}

void layer::pre_step() {
	remove_all_marked_removed();
	bind_all_unbound();
}

void layer::step() {
	for(auto &e : entities_) {
		for(auto s : e->type().systems()) {
			s->step(*e);
		}
	}
}

void layer::post_step() {
	remove_all_marked_removed();
	bind_all_unbound();
}

void layer::z_indices(std::set<int> &out) const {
	for(const auto &e : entities_) {
		out.insert(e->z_index());
	}
}

// render the given z-index
void layer::render(int z_index) const {
	for(auto &e : entities_) {
		if(e->z_index() == z_index && e->visible()) {
			for(auto s : e->type().systems()) {
				if(s->visible(*e)) {
					s->render(*e);
				}
			}
		}
	}
}

boost::optional<entity &> layer::entity_by_name(const std::string &name) {
	return entity_by_name_<entity>(name);
}

boost::optional<const entity &> layer::entity_by_name(const std::string &name) const {
	return const_cast<layer *>(this)->entity_by_name_<const entity>(name);
}

void layer::entities_with_system(const std::string &name, std::vector<entity *> &out) {
	entities_with_system_(name, out);
}

void layer::entities_with_system(const std::string &name, std::vector<const entity *> &out) const {
	const_cast<layer *>(this)->entities_with_system_(name, out);
}

void layer::index__(std::size_t val) {
	index_ = val;
}

// Invokes add() for all entities who haven't had it called yet
void layer::bind_all_unbound() {
	for(auto &e : entities_) {
		if(!e->marked_added_()) {
			for(auto s : e->type().systems()) {
				s->add(*e);
			}
			e->mark_added_();
		}
	}
}

void layer::remove_all_marked_removed() {
	for(auto it(entities_.begin()); it != entities_.end(); ++it) {
		auto &e(**it);
		if(e.marked_removed_()) {
			auto itl(entities_.end() - 1);
			std::swap(*it, *itl);
			entities_.erase(itl);
			if(it == entities_.end()) {
				break;
			}
		}
	}
}

template <typename Entity>
boost::optional<Entity &> layer::entity_by_name_(const std::string &name) {
	for(auto &e : entities_) {
		auto &cc(e->component<core_component>());
		if(cc.name && *cc.name == name) {
			return *e;
		}
	}
	return {};
}

template boost::optional<entity &> layer::entity_by_name_<entity>(const std::string &);
template boost::optional<const entity &> layer::entity_by_name_<const entity>(const std::string &);

template <typename Entity>
void layer::entities_with_system_(const std::string &name, std::vector<Entity *> &out) {
	for(auto &e : entities_) {
		for(auto s : e->type().systems()) {
			if(s->name() == name) {
				out.emplace_back(e.get());
				break;
			}
		}
	}
}

template void layer::entities_with_system_<entity>(const std::string &, std::vector<entity *> &);
template void layer::entities_with_system_<const entity>(const std::string &, std::vector<const entity *> &);

const std::string &level::layer_name(const std::unique_ptr<layer> &lyr) {
	return lyr->name();
}

level &level::current() {
	assert(current_level_);
	struct visitor : boost::static_visitor<level &> {
		result_type operator()(std::reference_wrapper<level> lvl) const {
			return lvl;
		}

		result_type operator()(std::reference_wrapper<const level> lvl) const {
			PODGE_THROW_ERROR();
		}
	};
	return boost::apply_visitor(visitor(), *current_level_);
}

const level &level::const_current() {
	assert(current_level_);
	struct visitor : boost::static_visitor<const level &> {
		result_type operator()(std::reference_wrapper<level> lvl) const {
			return lvl.get();
		}

		result_type operator()(std::reference_wrapper<const level> lvl) const {
			return lvl.get();
		}
	};
	return boost::apply_visitor(visitor(), *current_level_);
}

void level::current(level &lvl) {
	current_level_ = std::reference_wrapper<level>(lvl);
}

void level::const_current(const level &lvl) {
	current_level_ = std::reference_wrapper<const level>(lvl);
}

void level::clear_current() {
	current_level_ = {};
}

level::current_level_type level::current_level_;

level::clear_dtor_current_level::~clear_dtor_current_level() {
	level::clear_current();
}

level::level(NVGcontext *vg, int width, int height, float dt) :
	vg_(vg),
	pool_(vg),
	world_(default_gravity),
	width_(width),
	height_(height),
	bg_color_(0.0f, 0.0f, 0.0f, 1.0f),
	camera_width_(default_camera_width),
	camera_height_(default_camera_height),
	time_(0.0f),
	dt_(dt),
	rng_(std::random_device{}())
{
}

level::level(NVGcontext *vg, pugi::xml_node tmx_node, float dt, const resource_path &cwd) :
	level(vg, tmx_node.select_node("/map").node().attribute("width").as_int(), tmx_node.select_node("/map").node().attribute("height").as_int(), dt)
{
	level::current(*this);
	BOOST_SCOPE_EXIT(void) {
		level::clear_current();
	} BOOST_SCOPE_EXIT_END

	auto map_node(tmx_node.select_node("/map").node());
	width_ = map_node.attribute("width").as_int();
	height_ = map_node.attribute("height").as_int();
	bg_color_ = parse_color(map_node.attribute("backgroundcolor").value());

	// read custom properties
	{
		auto properties_xpn(tmx_node.select_node("./properties"));
		if(properties_xpn) {
			load_properties_xml(properties_xpn.node(), cwd);
		}
	}

	// load tilesets
	for(auto ts_xpn : tmx_node.select_nodes("/map/tileset")) {
		auto ts_node(ts_xpn.node());
		auto firstgid(ts_node.attribute("firstgid").as_int());
		auto ts(std::make_shared<tileset>());
		pugi::xml_document tsx;
		auto tsx_path(resource_path(ts_node.attribute("source").value()).canonical(cwd));
		check_xml_parse_result(tsx_path, tsx.load_string(get_resource(tsx_path.str()).c_str()));
		ts->load_xml(tsx, tsx_path.parent());
		if(ts->size() > 0) {
			auto ivl(boost::icl::interval<int>::right_open(firstgid, firstgid + *ts->rbegin() + 1));
			tilesets_.insert(std::make_pair(ivl, ts));
		}
	}

	// load layers 
	for(auto lyr_xpn : tmx_node.select_nodes("/map/layer | /map/objectgroup")) {
		auto lyr_node(lyr_xpn.node());
		if(std::strcmp(lyr_node.name(), "layer") == 0) {
			add_layer(num_layers(), layer::from_tile_layer_xml(lyr_node, cwd));
		} else if(std::strcmp(lyr_node.name(), "objectgroup") == 0) {
			add_layer(num_layers(), layer::from_object_layer_xml(lyr_node, cwd));
		} else {
			PODGE_THROW_ERROR();
		}
	}

	world_.SetContactFilter(&registry);
	world_.SetContactListener(&registry);
}

level::~level() {
	level::current(*this);
	Mix_HaltChannel(-1); // halt all channels so the resource_pool destructor can free samples
}

NVGcontext *level::vg() const {
	return vg_;
}

tileset_map &level::tilesets() {
	return tilesets_;
}

const tileset_map &level::tilesets() const {
	return tilesets_;
}

resource_pool &level::pool() {
	return pool_;
}

const resource_pool &level::pool() const {
	return pool_;
}

b2World &level::world() {
	return world_;
}

const b2World &level::world() const {
	return world_;
}

layer &level::add_layer(std::size_t index, std::unique_ptr<layer> &&lyr) {
	auto &seq(layers_.get<0>());
	auto p(seq.emplace(seq.begin() + index, std::move(lyr)));
	assert(p.second);
	for(auto it(seq.begin()); it != seq.end(); ++it) {
		(*it)->index__(it - seq.begin());
	}
	return **p.first;
}

layer &level::layer_by_name(const std::string &name) {
	return **check_iterator(layers_.get<1>().find(name), layers_.get<1>().end());
}

const layer &level::layer_by_name(const std::string &name) const {
	return **check_iterator(layers_.get<1>().find(name), layers_.get<1>().end());
}

boost::iterator_range<level::layer_iterator> level::layers() {
	return {
		layer_iterator(boost::make_transform_iterator<ptr_transforms::layer_to_layer>(layers_.get<0>().begin())),
		layer_iterator(boost::make_transform_iterator<ptr_transforms::layer_to_layer>(layers_.get<0>().end()))
	};
}

boost::iterator_range<level::const_layer_iterator> level::layers() const {
	return {
		const_layer_iterator(boost::make_transform_iterator<ptr_transforms::const_layer_to_layer>(layers_.get<0>().begin())), 
		const_layer_iterator(boost::make_transform_iterator<ptr_transforms::const_layer_to_layer>(layers_.get<0>().end()))
	};
}

boost::optional<entity &> level::entity_by_name(const std::string &name) {
	for(auto &lyr : layers_) {
		auto e(lyr->entity_by_name(name));
		if(e) {
			return e;
		}
	}
	return {};
}

boost::optional<const entity &> level::entity_by_name(const std::string &name) const {
	for(const auto &lyr : layers_) {
		auto e(lyr->entity_by_name(name));
		if(e) {
			return *e;
		}
	}
	return {};
}

void level::entities_with_system(const std::string &name, std::vector<entity *> &out) {
	for(auto &lyr : layers_) {
		lyr->entities_with_system(name, out);
	}
}

void level::entities_with_system(const std::string &name, std::vector<const entity *> &out) const {
	for(const auto &lyr : layers_) {
		lyr->entities_with_system(name, out);
	}
}

std::size_t level::num_layers() const {
	return layers_.get<0>().size();
}

int level::width() const {
	return width_;
}

int level::height() const {
	return height_;
}

b2AABB level::bounds() const {
	b2AABB bounds;
	bounds.lowerBound.x = 0.0f;
	bounds.lowerBound.y = 0.0f;
	bounds.upperBound.x = width_;
	bounds.upperBound.y = height_;
	return bounds;
}

void level::bg_color(const glm::vec4 &val) {
	bg_color_ = val;
}

glm::vec4 level::bg_color() const {
	return bg_color_;
}

void level::camera_position(const glm::vec2 &val) {
	camera_position_ = val;
}

const glm::vec2 &level::camera_position() const {
	return camera_position_;
}

void level::camera_width(float val) {
	camera_width_ = val;
}

float level::camera_width() const {
	return camera_width_;
}

void level::camera_height(float val) {
	camera_height_ = val;
}

float level::camera_height() const {
	return camera_height_;
}

b2AABB level::camera_bounds() const {
	b2AABB bounds;
	bounds.lowerBound.x = camera_position_.x - camera_width_/2.0f;
	bounds.lowerBound.y = camera_position_.y - camera_height_/2.0f;
	bounds.upperBound.x = camera_position_.x + camera_width_/2.0f;
	bounds.upperBound.y = camera_position_.y + camera_height_/2.0f;
	return bounds;
}

float level::time() const {
	return time_;
}

float level::dt() const {
	return dt_;
}

void level::queue_input(const input &in) {
	input_queue_.emplace_back(in);
}

void level::step() {
	if(exit_state_) {
		return;
	}
	current(*this);
	BOOST_SCOPE_EXIT(void) {
		clear_current();
	} BOOST_SCOPE_EXIT_END
	for(auto &lyr : layers_) {
		lyr->pre_step();
	}
	dequeue_inputs_();
	world_.Step(dt_, 8, 3);
	for(auto &lyr : layers_) {
		lyr->step();
	}
	for(auto &lyr : layers_) {
		lyr->post_step();
	}
	time_ += dt_;
}

void level::render() const {
	const_current(*this);
	BOOST_SCOPE_EXIT(void) {
		clear_current();
	} BOOST_SCOPE_EXIT_END
	z_indices_buf_.clear();
	for(const auto &lyr : layers_) {
		lyr->z_indices(z_indices_buf_);
	}
	for(auto z_index : z_indices_buf_) {
		for(const auto &lyr : layers_) {
			lyr->render(z_index);
		}
	}
}

void level::exit(const level_exit &exit) {
	exit_state_.emplace(exit);
}

boost::optional<level_exit> level::exit_state() const {
	return exit_state_;
}

std::default_random_engine &level::rng() {
	return rng_;
}

void level::query_entities(const b2AABB &aabb, std::vector<entity *> &out) {
	query_entities_(aabb, out);
}

void level::query_entities(const b2AABB &aabb, std::vector<const entity *> &out) const {
	const_cast<level *>(this)->query_entities_(aabb, out);
}

template <typename Entity>
void level::query_entities_(const b2AABB &aabb, std::vector<Entity *> &out) {
	struct query_callback : b2QueryCallback {
		query_callback(std::vector<Entity *> &out) :
			out(out)
		{
		}

		bool ReportFixture(b2Fixture *fixture) {
			out.emplace_back(static_cast<Entity *>(fixture->GetBody()->GetUserData()));
			return true;
		}

		std::vector<Entity *> &out;
	};
	query_callback callback(out);
	world_.QueryAABB(&callback, aabb);
	std::sort(out.begin(), out.end(), [](Entity *a, Entity *b) {
		return std::make_pair(a->z_index(), a->lyr().index()) < std::make_pair(b->z_index(), b->lyr().index());
	});
}

template void level::query_entities_<entity>(const b2AABB &, std::vector<entity *> &);
template void level::query_entities_<const entity>(const b2AABB &, std::vector<const entity *> &);

// Note: Expects current level to be set
void level::dequeue_inputs_() {
	while(!input_queue_.empty()) {
		const auto &in(input_queue_.front());

		z_indices_buf_.clear();
		for(auto &lyr : layers()) {
			lyr.z_indices(z_indices_buf_);
		}

		auto exec_handlers([&](bool last) {
			for(auto it(z_indices_buf_.rbegin()); it != z_indices_buf_.rend(); ++it) {
				auto z_index(*it);
				for(auto &lyr : layers_) {
					for(auto &e : lyr->entities()) {
						if(e.z_index() != z_index) {
							continue;
						}
						auto handled(false);
						for(auto s : e.type().systems()) {
							if(last != s->should_handle_input_last()) {
								continue;
							}
							if(s->handle_input(e, in)) {
								handled = true;
								break;
							}
						}
						if(handled) {
							return true;
						}
					}
				}
			}
			return false;
		});

		// perform two passes of input handling: first input handlers with should_handle_input_last() == false, then the rest.
		if(!exec_handlers(false)) {
			exec_handlers(true);
		}

		input_queue_.pop_front();
	}
}

}
