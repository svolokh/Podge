#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_WALL_HPP_FWD
#define PODGE_ENTITY_WALL_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace wall {

} } }

#endif

#else

#ifndef PODGE_ENTITY_WALL_HPP
#define PODGE_ENTITY_WALL_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace wall {

struct system : entity_system {
	const char *name() const {
		return "wall";
	}

	void init(entity &e) const {
		e.body()->SetType(b2_staticBody);
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

