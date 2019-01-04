#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_BASIC_HPP_FWD
#define PODGE_ENTITY_BASIC_HPP_FWD

#include <podge.hpp>
#include <json.hpp>

namespace nm = nlohmann;

namespace podge { namespace systems { namespace basic {

PODGE_PUBLIC_COMPONENT(public_component) {
	void validate(const context &ctx) const {
		if(keyframes.empty()) {
			throw validation_error("'keyframes' must be defined");
		}
		if(!resource_exists(keyframes.str())) {
			throw validation_error("'keyframes' refers to a non-existent file");
		}
	}

	BOOST_HANA_DEFINE_STRUCT(public_component,
		(resource_path, keyframes));
};
PODGE_REGISTER_COMPONENT(public_component);

PODGE_COMPONENT(component) {
	component() :
		keyframe("default"),
		opacity(1.0f)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(std::string, keyframe),
		(float, opacity));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_BASIC_HPP
#define PODGE_ENTITY_BASIC_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace basic {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(std::map<std::string, int>, keyframes));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "basic";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(public_component),
			typeid(component),
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		auto &lvl(level::current());
		auto &pubc(e.component<public_component>());
		auto &pc(e.component<private_component>());
		auto &cc(e.component<core_component>());

		// load keyframes
		auto json_kf(lvl.pool().json(pubc.keyframes));
		auto dir(pubc.keyframes.parent());
		for(auto it(json_kf.begin()); it != json_kf.end(); ++it) {
			resource_path path(it.value().get<std::string>());
			pc.keyframes.emplace(it.key(), lvl.pool().image(path.canonical(dir).str()));
		}

		// set up collision
		if(cc.collision_shapes.empty()) {
			e.body()->SetType(b2_staticBody);
		}
		for(auto &shp : cc.collision_shapes) {
			auto fixture(e.body()->CreateFixture(shp->shape.get(), 1.0f));
			e.fixture_data(fixture) = shp->properties;
		}
	}

	bool visible(const entity &e) const {
		const auto &lvl(level::const_current());
		auto angle(e.body()->GetAngle());
		const auto &pos(e.body()->GetPosition());
		auto &cc(e.component<core_component>());
		auto w(cc.width);
		auto h(cc.height);
		if(angle != 0.0f) {
			auto c(std::cos(angle));
			auto s(std::sin(angle));
			w = w + std::abs(w - w*c + h*s);
			h = h + std::abs(h - w*s - h*c);
		}
		auto w1(lvl.camera_width());
		auto x1(lvl.camera_position().x - w1/2.0f);
		auto h1(lvl.camera_height());
		auto y1(lvl.camera_position().y - h1/2.0f);
		auto w2(w);
		auto x2(pos.x - w2/2.0f);
		auto h2(h);
		auto y2(pos.y - h2/2.0f);
		return x1 - w2 <= x2 && x2 <= x1 + w1 && y1 - h2 <= y2 && y2 <= y1 + h1;
	}

	void render(const entity &e) const {
		const auto &lvl(level::const_current());
		const auto &c(e.component<component>());
		const auto &pc(e.component<private_component>());
		const auto &cc(e.component<core_component>());
		auto vg(lvl.vg());
		auto img(check_iterator(pc.keyframes.find(c.keyframe), pc.keyframes.end())->second);
		auto w(cc.width);
		auto h(cc.height);
		auto angle(e.body()->GetAngle());
		const auto &pos(e.body()->GetPosition());
		auto paint(nvgImagePattern(vg, pos.x - w/2.0f, pos.y - h/2.0f, w, h, 0.0f, img, 1.0f));
		nvgBeginPath(vg);
		nvgSave(vg);
		nvgTranslate(vg, pos.x, pos.y);
		nvgRotate(vg, angle);
		if(cc.flip_vertical) {
			nvgScale(vg, 1.0f, -1.0f);
		}
		if(cc.flip_horizontal) {
			nvgScale(vg, -1.0f, 1.0f);
		}
		nvgTranslate(vg, -pos.x, -pos.y);
		const auto offs(0.002f); // small offset to prevent gaps between adjacent images
		nvgRect(vg, pos.x - w/2.0f - offs, pos.y - h/2.0f - offs, w + 2.0f*offs, h + 2.0f*offs);
		nvgGlobalAlpha(vg, c.opacity);
		nvgFillPaint(vg, paint);
		nvgFill(vg);
		nvgRestore(vg);
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

