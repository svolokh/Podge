#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_TRIGGER_HPP_FWD
#define PODGE_ENTITY_TRIGGER_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace sensor {

} } }

#endif

#else

#ifndef PODGE_ENTITY_TRIGGER_HPP
#define PODGE_ENTITY_TRIGGER_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace sensor {

struct system : entity_system {
	const char *name() const {
		return "sensor";
	}

	void init(entity &e) const {
		auto &cc(e.component<core_component>());
		e.body()->SetType(b2_staticBody);
		for(const auto &s : cc.collision_shapes) {
			e.body()->CreateFixture(s->shape.get(), 1.0f)->SetSensor(true);
		}
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

