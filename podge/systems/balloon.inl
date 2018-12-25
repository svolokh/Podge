#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_BALLOON_HPP_FWD
#define PODGE_ENTITY_BALLOON_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace balloon {

} } }

#endif

#else

#ifndef PODGE_ENTITY_BALLOON_HPP
#define PODGE_ENTITY_BALLOON_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace balloon {

struct system : entity_system {
	const char *name() const {
		return "balloon";
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

