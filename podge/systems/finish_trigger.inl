#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_FINISH_TRIGGER_HPP_FWD
#define PODGE_ENTITY_FINISH_TRIGGER_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace finish_trigger {

} } }

#endif

#else

#ifndef PODGE_ENTITY_FINISH_TRIGGER_HPP
#define PODGE_ENTITY_FINISH_TRIGGER_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace finish_trigger {

struct system : entity_system {
	const char *name() const {
		return "finish_trigger";
	}
};
PODGE_REGISTER_SYSTEM(system);

struct contact_handler : entity_contact_handler {
	entity_system_mask mask() const {
		return {
			{"finish_trigger", "podge"}
		};
	}

	void begin_contact(entity_contact &contact) const {
		entity_contact_handler::begin_contact(contact);
		auto &lvl(level::current());
		lvl.exit(level_exits::victory());
	}
};
PODGE_REGISTER_CONTACT_HANDLER(contact_handler);

} } }

#endif

#endif

