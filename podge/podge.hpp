#ifndef PODGE_HPP_
#define PODGE_HPP_

#include "common.hpp"

#include <memory>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <functional>
#include <vector>
#include <initializer_list>
#include <typeindex>

#include <Box2D/Box2D.h>
#include <pugixml.hpp>
#include <nanovg.h>
#include <json.hpp>
#include <glm/glm.hpp>
#include <boost/any.hpp>
#include <boost/hana.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/icl/interval.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/functional/hash.hpp>

#define CONCAT_(LEFT, MIDDLE, RIGHT) LEFT ## MIDDLE ## RIGHT
#define CONCAT(LEFT, MIDDLE, RIGHT) CONCAT_(LEFT, MIDDLE, RIGHT)

namespace podge {

namespace nm = nlohmann;
namespace hana = boost::hana;

struct entity;
struct entity_type;
struct layer;
struct level;

typedef std::size_t category_t;
typedef std::size_t signal_t;

b2Vec2 to_b2Vec2(const glm::vec2 &v);
glm::vec2 to_vec2(const b2Vec2 &v);
void to_nvg_xform(const glm::mat3 &m, float *xform);

template <typename T>
struct vec2_with_extra {
	b2Vec2 v;
	T extra;
};

struct resource_path {
public:
	resource_path();
	resource_path(const std::string &path);
	resource_path(const resource_path &) = default;

private:
	resource_path(const std::vector<std::string> &parts);

public:
	resource_path parent() const;
	resource_path canonical(const resource_path &base) const;
	resource_path operator/(const resource_path &path) const;
	std::string str() const;
	bool empty() const;

private:
	std::vector<std::string> parts_;
};

typedef std::chrono::high_resolution_clock clock;

template <typename Iterator>
inline Iterator check_iterator(Iterator it, Iterator last) {
	assert(it != last);
	return it;
}

template <typename T>
struct type_holder {
	typedef T type;
};

template <typename T>
struct make_type_holder {
	typedef type_holder<T> type;
};

struct resource_pool {
	resource_pool(NVGcontext *vg);
	~resource_pool();

	const nm::json &json(const resource_path &path) const;
	int image(const resource_path &pah) const;

private:
	NVGcontext *vg_;
	mutable std::map<std::string, nm::json> jsons_;
	mutable std::map<std::string, int> images_;
};

struct validation_error {
	validation_error(std::string msg);

	std::string msg;
};

struct object_component {
	struct context {
		virtual ~context() = default;

		virtual bool entity_exists(const std::string &name) const = 0;
	};

	virtual ~object_component() = default;
	virtual void validate(const context &ctx) const;
	virtual std::unique_ptr<object_component> clone() const = 0;
};

namespace detail {

template <typename Component>
struct object_component_impl : object_component {
	enum { is_public = false };

	std::unique_ptr<object_component> clone() const {
		return std::unique_ptr<object_component>(new Component(*static_cast<const Component *>(this)));
	}
};

}

struct object {
	object() = default;
	object(const object &o);
	object(object &&o);

	object &operator =(const object &o);
	object &operator =(object &&o);

	void load_properties_xml(pugi::xml_node properties_node, const resource_path &cwd);

	object_component &component(std::type_index cidx);
	const object_component &component(std::type_index cidx) const;
	bool has_component(std::type_index cidx) const;

	template <typename Component>
	Component &component() {
		return static_cast<Component &>(component(typeid(Component)));
	}

	template <typename Component>
	const Component &component() const {
		return static_cast<const Component &>(component(typeid(Component)));
	}

	template <typename Component>
	bool has_component() const {
		return has_component(typeid(Component));
	}

	template <typename Component>
	void add_component() {
		auto res(components_.emplace(typeid(Component), new Component()));
		assert(res.second);
	}

private:
	std::unordered_map<std::type_index, std::unique_ptr<object_component>> components_;
};

struct entity;
struct input;

struct entity_system {
	// The name of the entity system (required).
	virtual const char *name() const = 0;
	
	// Components required by this system.
	virtual std::vector<std::type_index> components() const;

	// Initialize the entity. After the engine calls this the entity must be in a valid state.
	virtual void init(entity &e) const;

	// Called when the entity has been added to the world (this is where the entity should bind to other entities in its world if it needs to).  This method is guaranteed to be called one time and before a call to step(). You must NOT create new entities here.
	virtual void add(entity &e) const;

	// Perform a single step of logic on the entity.
	virtual void step(entity &e) const;

	// Systems with this flag will have their handle_input() called after all other systems are given a chance to handle the input (regardless of order of appearance on the screen).
	virtual bool should_handle_input_last() const;

	// Handle an input. Input handlers are called in order of appearance on the screen (front-most first). Systems with should_handle_input_last() will have their inputs handlers called last (regardless of order of appearance on the screen).
	// Return true if no more input handlers should be called, otherwise false.
	virtual bool handle_input(entity &e, const input &in) const;

	// Whether render() should be called for this entity.
	virtual bool visible(const entity &e) const;

	// Render the entity.
	virtual void render(const entity &e) const;
};

struct entity_type {
	entity_type(const char *name, bool has_body, std::vector<const entity_system *> systems);

	const char *name() const;
	bool has_body() const;
	const std::vector<const entity_system *> &systems() const;

private:
	const char *const name_;
	bool has_body_;
	const std::vector<const entity_system *> systems_;
};

struct entity_spec : object {
	static entity_spec from_tile_xml(pugi::xml_node tile_node, pugi::xml_node tileset_node, const resource_path &cwd);
	static entity_spec from_object_xml(pugi::xml_node object_node, const resource_path &cwd);

	entity_spec(const entity_type &type);
	virtual ~entity_spec() = default;

	const entity_type &type() const;
	virtual void z_index(int val);
	int z_index() const;
	virtual void visible(bool val);
	bool visible() const;

protected:
	const entity_type *type_;
	int z_index_;
	bool visible_;
};

struct entity : entity_spec {
private:
	struct extra_body_data {
		std::vector<std::unique_ptr<object>> fixture_data;
	};

public:
	entity(const entity_spec &spec, layer &lyr);
	entity(const entity &) = delete;
	~entity();

	bool has_body() const;

	b2Body *body();
	const b2Body *body() const;
	object &fixture_data(b2Fixture *fixture);
	const object &fixture_data(b2Fixture *fixture) const;

	void translate(const glm::vec2 &offset);
	layer &lyr();
	const layer &lyr() const;
	using entity_spec::z_index;
	using entity_spec::visible;
	void bind(entity &e); // Unidirectionally bind the lifetime of this entity to the lifetime of e. If entity e is removed, this entity will be removed as well.
	void unbind(entity &e); // Unbind the lifetime of this entity from e.
	void remove();

	template <typename ArgT>
	void signal(signal_t sig, const ArgT &arg) {
		auto it(signal_handlers_.find(sig));
		if(it != signal_handlers_.end()) {
			for(auto &&f : it->second) {
				f(*this, boost::any(arg));
			}
		}
	}

	// Called by entity types to register signal handlers on the entity.
	template <typename ArgT, typename Callable>
	void handle_signal(signal_t sig, Callable &&f) {
		signal_handlers_[sig].emplace_back([f](entity &e, const boost::any &arg) {
			f(e, boost::any_cast<const ArgT &>(arg));
		});
	}

	// internal 
	bool marked_removed_() const;
	void mark_added_();
	bool marked_added_() const;

private:
	layer &lyr_;
	boost::optional<b2Body *> body_;
	std::set<entity *> incoming_binds_; // when this entity is removed, all entities in this set will be removed
	std::set<entity *> outgoing_binds_; // when this entity is removed, for each entity e in this set, this entity will be removed from e's incoming_binds_
	std::map<signal_t, std::vector<std::function<void(entity &, const boost::any &)>>> signal_handlers_;
	boost::optional<extra_body_data> extra_body_data_;
	bool added_; // whether entity_type::add has been called on this entity yet
	bool remove_;
};

struct input {
	enum type {
		DOWN,
		DRAG,
		UP
	};

	int64_t finger_id;
	type type;
	float x;
	float y;
	float dx;
	float dy;
};

struct entity_type_map : std::map<std::string, std::unique_ptr<entity_type>> {
	template <typename Callable>
	entity_type_map(const Callable &f) {
		f([this](entity_type *inst) {
			assert(this->find(inst->name()) == this->end());
			this->emplace(inst->name(), inst);
		});
	}
};

struct entity_contact_world_manifold {
	b2Vec2 normal;
	b2Vec2 points[b2_maxManifoldPoints];
	float32 separations[b2_maxManifoldPoints];
	int32 point_count;
};

// Rather than use the Box2D contact structures directly, we wrap them to guarantee the ordering of entities A and B
struct entity_contact {
	entity_contact(b2Contact *contact, bool swap);

	entity &entity_a();
	entity &entity_b();

	entity_contact_world_manifold world_manifold() const;
	b2Vec2 normal() const;
	b2Vec2 point() const;
	bool touching() const;
	void enabled(bool flag);
	bool enabled() const;
	b2Fixture *fixture_a();
	b2Fixture *fixture_b();
	void friction(float32 val);
	float32 friction() const;
	void reset_friction();
	void restitution(float32 val);
	float32 restitution() const;
	void reset_restitution();
	void tangent_speed(float32 speed);
	float32 tangent_speed() const;

private:
	b2Contact *contact_;
	bool swap_; // whether entities A and B should be swapped in the b2Contact instance.
};

struct entity_contact_handler {
	typedef std::vector<std::vector<const char *>> entity_system_mask;

	virtual ~entity_contact_handler() = default;

	virtual entity_system_mask mask() const = 0;

	virtual bool should_collide(entity &a, entity &b) const;

	virtual void begin_contact(entity_contact &contact) const;
	virtual void end_contact(entity &a, entity &b) const;

	virtual void pre_solve(entity_contact &contact) const;
	virtual void post_solve(entity_contact &contact, const b2ContactImpulse *impulse) const;
};

struct registry : b2ContactFilter, b2ContactListener {
private:
	using component_adder = std::function<void(object &)>;
	using component_property_setter = std::function<void(const std::string &, boost::any, object_component &)>;

public:
	void add_component_to(object &obj, std::type_index cidx) const;

	bool exists_public_component_property(const std::string &prop) const;

	// Finds the component (with is_public() == true) having the given property, then sets that property to the given value.
	template <typename T>
	void set_public_component_property(object &obj, const std::string &prop, T val) const {
		auto it(prop_to_public_component_.find(prop));
		assert(it != prop_to_public_component_.end());
		auto cidx(it->second);
		auto jt(public_component_setters_.find(cidx));
		assert(jt != public_component_setters_.end());
		auto &c(obj.component(cidx));
		jt->second(prop, boost::any(std::move(val)), c);
	}

	const entity_type &type(const char *name) const;
	const entity_system &system(const char *name) const;

	bool ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB);

	void BeginContact(b2Contact *contact);
	void EndContact(b2Contact *contact);
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);

private:
	template <typename Callable>
	void iterate_handlers(entity &a, entity &b, Callable &&f) {
		for(auto sA : a.type().systems()) {
			auto it(wildcard_contact_handlers_.find(boost::string_view(sA->name())));
			if(it != wildcard_contact_handlers_.end()) {
				for(auto &handler : it->second) {
					f(*handler, false);
				}
			}
		}
		for(auto sB : b.type().systems()) {
			auto name(sB->name());
			auto it(wildcard_contact_handlers_.find(boost::string_view(sB->name())));
			if(it != wildcard_contact_handlers_.end()) {
				for(auto &handler : it->second) {
					f(*handler, true);
				}
			}
		}
		for(auto sA : a.type().systems()) {
			for(auto sB : b.type().systems()) {
				{
					auto p(std::make_pair(boost::string_view(sA->name()), boost::string_view(sB->name())));
					auto it(exact_contact_handlers_.find(p));
					if(it != exact_contact_handlers_.end()) {
						for(auto &handler : it->second) {
							f(*handler, false);
						}
					}
				}
				{
					auto p(std::make_pair(boost::string_view(sB->name()), boost::string_view(sA->name())));
					auto it(exact_contact_handlers_.find(p));
					if(it != exact_contact_handlers_.end()) {
						for(auto &handler : it->second) {
							f(*handler, true);
						}
					}
				}
			}
		}
	}

private:
	std::map<std::type_index, component_adder> component_adders_;
	std::map<std::type_index, component_property_setter> public_component_setters_;
	std::map<std::string, std::type_index> prop_to_public_component_; 
	std::map<boost::string_view, std::unique_ptr<entity_system>> systems_;
	std::map<boost::string_view, entity_type> types_;
	std::map<boost::string_view, std::vector<std::shared_ptr<entity_contact_handler>>> wildcard_contact_handlers_;
	std::map<std::pair<boost::string_view, boost::string_view>, std::vector<std::shared_ptr<entity_contact_handler>>> exact_contact_handlers_;

	friend struct registry_init;
};

struct registry_init {
	struct register_component_fn {
		template <typename Component>
		void do_register(registry &reg) const {
			struct adder {
				void operator()(object &obj) {
					obj.add_component<Component>();
				}
			};

			struct property_setter {
				property_setter() {
					constexpr auto accessors(hana::accessors<Component>());
					hana::for_each(accessors, [this](auto p) {
						constexpr auto name(hana::first(p));
						constexpr auto accessor(hana::second(p));
						setters.emplace(std::string(decltype(name)::c_str()), [accessor](boost::any val, object_component &c_) {
							auto &c(static_cast<Component &>(c_));
							auto &v(accessor(c));
							v = boost::any_cast<typename std::decay<decltype(v)>::type &&>(val);
						});
					});
				}

				void operator()(const std::string &prop, boost::any val, object_component &c) {
					auto it(setters.find(prop));
					assert(it != setters.end());
					it->second(std::move(val), c);
				}

			private:
				std::map<std::string, std::function<void(boost::any, object_component &)>> setters;
			};

			std::type_index cidx(typeid(Component));
			auto r1(reg.component_adders_.emplace(cidx, adder()));
			assert(r1.second);

			if(Component::is_public) {
				auto r2(reg.public_component_setters_.emplace(cidx, property_setter()));
				assert(r2.second);

				constexpr auto accessors(hana::accessors<Component>());
				hana::for_each(accessors, [&reg, cidx](auto p) {
					constexpr auto name(hana::first(p));
					constexpr auto accessor(hana::second(p));
					auto res(reg.prop_to_public_component_.emplace(decltype(name)::c_str(), cidx));
					assert(res.second);
				});
			}
		}
	};

	struct register_system_fn {
		void operator()(entity_system *s) const;
	};

	struct register_contact_handler_fn {
		void operator()(entity_contact_handler *handler) const;
	};
	
	struct register_type_fn {
		void operator()(const char *name, bool has_body, std::initializer_list<const char *> systems) const;
	};

	struct component_tag {};
	struct system_tag {};
	struct contact_handler_tag {};
	struct type_tag {};

	template <typename Callable>
	registry_init(component_tag, const Callable &f) {
		f(register_component_fn());
	}

	template <typename Callable>
	registry_init(system_tag, const Callable &f) {
		f(register_system_fn());
	}

	template <typename Callable>
	registry_init(contact_handler_tag, const Callable &f) {
		f(register_contact_handler_fn());
	}

	template <typename Callable>
	registry_init(type_tag, const Callable &f) {
		f(register_type_fn());
	}
};

struct tileset {
private:
	struct iter_ : boost::type_erasure::placeholder {};
	typedef boost::type_erasure::forward_iterator<iter_, unsigned int> id_iterator_requirements;

public:
	typedef boost::type_erasure::any<id_iterator_requirements, iter_> id_iterator;

	tileset() = default;
	tileset(const tileset &) = delete;

	void add_entity_spec(unsigned int id, const entity_spec &spec);
	const entity_spec &spec(unsigned int id) const;
	id_iterator begin() const;
	id_iterator end() const;
	id_iterator rbegin() const;
	id_iterator rend() const;
	std::size_t size() const;
	void load_xml(pugi::xml_node tsx_node, const resource_path &cwd);

private:
	std::map<unsigned int, entity_spec> specs_;
	friend struct tileset_map;
};

// use a shared_ptr here since boost::icl::interval_map doesn't support non-copyable values.
struct tileset_map : boost::icl::interval_map<int, std::shared_ptr<tileset>> {
	using interval_map::interval_map;

	const entity_spec &spec(int gid) const;
};

struct layer {
private:
	struct iter_ : boost::type_erasure::placeholder {};
	typedef boost::type_erasure::forward_iterator<iter_, entity &> entity_iterator_requirements;
	typedef boost::type_erasure::forward_iterator<iter_, const entity &> const_entity_iterator_requirements;

public:
	typedef boost::type_erasure::any<entity_iterator_requirements, iter_> entity_iterator;
	typedef boost::type_erasure::any<const_entity_iterator_requirements, iter_> const_entity_iterator;

	static std::unique_ptr<layer> from_tile_layer_xml(pugi::xml_node layer_node, const resource_path &cwd);
	static std::unique_ptr<layer> from_object_layer_xml(pugi::xml_node objectgroup_node, const resource_path &cwd);

public:
	layer(const std::string &name);
	
	std::size_t index() const;
	const std::string &name() const;
	boost::iterator_range<entity_iterator> entities();
	boost::iterator_range<const_entity_iterator> entities() const;
	entity &create_entity(const glm::vec2 &pos, const entity_spec &spec);
	void pre_step();
	void step();
	void post_step();
	void z_indices(std::set<int> &out) const;
	void render(int z_index) const;

	boost::optional<entity &> entity_by_name(const std::string &name);
	boost::optional<const entity &> entity_by_name(const std::string &name) const;

	void entities_with_system(const std::string &name, std::vector<entity *> &out);
	void entities_with_system(const std::string &name, std::vector<const entity *> &out) const;

	// internal
	void index__(std::size_t val);

private:
	entity &create_entity_internal(const glm::vec2 &pos, const entity_spec &spec);
	void bind_all_unbound(); 
	void remove_all_marked_removed();

	template <typename Entity>
	boost::optional<Entity &> entity_by_name_(const std::string &name);

	template <typename Entity>
	void entities_with_system_(const std::string &name, std::vector<Entity *> &out);

	std::size_t index_;
	std::string name_;
	std::vector<std::unique_ptr<entity>> entities_;
};

// Types of level exits and data associated with them
namespace level_exits {

struct failure {
};

struct victory {
};

}

typedef boost::variant<
	level_exits::failure,
	level_exits::victory> level_exit;

struct level : object {
private:
	struct iter_ : boost::type_erasure::placeholder {};
	typedef boost::type_erasure::forward_iterator<iter_, layer &> layer_iterator_requirements;
	typedef boost::type_erasure::forward_iterator<iter_, const layer &> const_layer_iterator_requirements;
	typedef boost::optional<boost::variant<std::reference_wrapper<level>, std::reference_wrapper<const level>>> current_level_type;

	static const std::string &layer_name(const std::unique_ptr<layer> &lyr); 
	typedef boost::multi_index::multi_index_container<
			std::unique_ptr<layer>,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access<>,
				boost::multi_index::ordered_unique<
					boost::multi_index::global_fun<const std::unique_ptr<layer> &, const std::string &, &level::layer_name>>>
		> layers_container;

	struct clear_dtor_current_level {
		~clear_dtor_current_level();
	};

public:
	typedef boost::type_erasure::any<layer_iterator_requirements, iter_> layer_iterator;
	typedef boost::type_erasure::any<const_layer_iterator_requirements, iter_> const_layer_iterator;

	static level &current();
	static const level &const_current();

private:
	static void current(level &lvl);
	static void const_current(const level &lvl);
	static void clear_current();

	static current_level_type current_level_;

public:
	level(NVGcontext *vg, int width, int height, float dt);
	level(NVGcontext *vg, pugi::xml_node tmx_node, float dt, const resource_path &cwd);
	~level();

	NVGcontext *vg() const;
	tileset_map &tilesets();
	const tileset_map &tilesets() const;
	resource_pool &pool();
	const resource_pool &pool() const;
	b2World &world();
	const b2World &world() const;
	layer &add_layer(std::size_t index, std::unique_ptr<layer> &&lyr);
	layer &layer_by_name(const std::string &name);
	const layer &layer_by_name(const std::string &name) const;
	boost::iterator_range<layer_iterator> layers();
	boost::iterator_range<const_layer_iterator> layers() const;
	boost::optional<entity &> entity_by_name(const std::string &name);
	boost::optional<const entity &> entity_by_name(const std::string &name) const;
	void entities_with_system(const std::string &name, std::vector<entity *> &out);
	void entities_with_system(const std::string &name, std::vector<const entity *> &out) const;
	std::size_t num_layers() const;
	int width() const;
	int height() const;
	b2AABB bounds() const;
	void bg_color(const glm::vec4 &val);
	glm::vec4 bg_color() const;
	void camera_position(const glm::vec2 &val);
	const glm::vec2 &camera_position() const;
	void camera_width(float val);
	float camera_width() const;
	void camera_height(float val);
	float camera_height() const;
	b2AABB camera_bounds() const;
	float time() const;
	float dt() const;
	void queue_input(const input &in); // Queues an input to be handled on the next call to step()
	void step();
	void render() const;
	void exit(const level_exit &exit);
	boost::optional<level_exit> exit_state() const;

public:
	void query_entities(const b2AABB &aabb, std::vector<entity *> &out); // Queries for entities in the level: entities are ordered by render order (i.e. visibility order), first entity would be rendered in front, second behind that, etc.
	void query_entities(const b2AABB &aabb, std::vector<const entity *> &out) const; 

private:
	template <typename Entity>
	void query_entities_(const b2AABB &aabb, std::vector<Entity *> &out); 

	void dequeue_inputs_();

	clear_dtor_current_level cdcl_; // this should always be first
	NVGcontext *vg_;
	std::deque<input> input_queue_;
	tileset_map tilesets_;
	resource_pool pool_;
	b2World world_;
	layers_container layers_;
	glm::vec2 camera_position_;
	int width_;
	int height_;
	glm::vec4 bg_color_;
	float camera_width_;
	float camera_height_;
	float time_;
	float dt_;
	boost::optional<level_exit> exit_state_;

	mutable std::set<int> z_indices_buf_;
};

}

#endif /* PODGE_HPP_ */