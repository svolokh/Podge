#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_PATH_HPP_FWD
#define PODGE_SYSTEM_PATH_HPP_FWD

namespace podge { namespace systems { namespace path {

} } }

#endif

#else

#ifndef PODGE_SYSTEM_PATH_HPP
#define PODGE_SYSTEM_PATH_HPP

namespace podge { namespace systems { namespace path {

struct system : entity_system {
    const char *name() const {
	return "path";
    }

    void init(entity &e) const {
	auto &cc(e.component<core_component>());
	e.body()->SetType(b2_staticBody);
	for(const auto &s : cc.collision_shapes) {
	    e.body()->CreateFixture(s->shape.get(), 1.0f);
	}
	e.body()->SetActive(false);
    }
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

