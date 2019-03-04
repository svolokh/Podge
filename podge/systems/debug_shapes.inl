#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_DEBUG_SHAPES_HPP_FWD
#define PODGE_ENTITY_DEBUG_SHAPES_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace debug_shapes {

} } }

#endif

#else

#ifndef PODGE_ENTITY_DEBUG_SHAPES_HPP
#define PODGE_ENTITY_DEBUG_SHAPES_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace debug_shapes {

struct system : entity_system {
	const char *name() const {
		return "debug_shapes";
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
		nvgFillColor(vg, nvgRGBf(1.0f, 0.0f, 0.0f));
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
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

