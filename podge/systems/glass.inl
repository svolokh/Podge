#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_GLASS_HPP_FWD
#define PODGE_ENTITY_GLASS_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace glass {

} } }

#endif

#else

#ifndef PODGE_ENTITY_GLASS_HPP
#define PODGE_ENTITY_GLASS_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace glass {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component, 
		(std::vector<Mix_Chunk *>, glass_taps));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "glass";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		resource_path dir("audio/tap/glass");
		for(auto name : {"1.ogg", "2.ogg", "3.ogg", "4.ogg"}) {
			pc.glass_taps.push_back(lvl.pool().load_sample(dir/name));
		}
		e.body()->SetType(b2_staticBody);
		e.body()->SetActive(false);
	}

	void render(const entity &e) const {
		const auto &lvl(level::const_current());
		const auto &cc(e.component<core_component>());
		auto vg(lvl.vg());
		nvgSave(vg);
		auto pos(e.body()->GetPosition());
		auto paint(nvgLinearGradient(vg, pos.x - 1.5f, pos.x - 1.5f, pos.x + 1.5f, pos.y + 1.5f, nvgRGBf(0.8f, 0.8f, 0.8f), nvgRGBf(0.2f, 0.2f, 0.2f)));
		nvgStrokeWidth(vg, 0.2f);
		nvgStrokeColor(vg, nvgRGBf(0.4f, 0.4f, 0.4f));
		nvgFillPaint(vg, paint);
		util::apply_transform(vg, e.body()->GetTransform());
		for(const auto &cs : cc.collision_shapes) {
			auto shp(cs->shape.get());
			assert(shp->GetChildCount() == 1);
			util::draw_shape(vg, cs->shape.get(), 0);
			nvgGlobalAlpha(vg, 0.5f);
			nvgFill(vg);
			nvgGlobalAlpha(vg, 1.0f);
			nvgStroke(vg);
		}
		nvgRestore(vg);
	}

	bool handle_input(entity &e, const input &in) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		auto &cc(e.component<core_component>());
		const auto &xf(e.body()->GetTransform());
		b2Vec2 pt(in.x, in.y);
		for(const auto &cs : cc.collision_shapes) {
			auto shp(cs->shape.get());
			if(shp->TestPoint(xf, pt)) {
				if(in.type == input::DOWN) {
					std::uniform_int_distribution<> dist(0, pc.glass_taps.size()-1);
					auto sample(pc.glass_taps[dist(lvl.rng())]);
					Mix_PlayChannel(-1, sample, 0);
				}
				return true;
			}
		}
		return false;
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

