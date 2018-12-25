#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_SPIKE_TRIGGER_HPP_FWD
#define PODGE_ENTITY_SPIKE_TRIGGER_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace spike_trigger {

PODGE_COMPONENT(component) {
	enum { is_public = true };

	BOOST_HANA_DEFINE_STRUCT(component,
		(std::string, spikes));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_SPIKE_TRIGGER_HPP
#define PODGE_ENTITY_SPIKE_TRIGGER_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace spike_trigger {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(std::vector<entity *>, spikes));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "spike_trigger";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(component),
			typeid(private_component)
		};
	}

	void add(entity &e) const {
		auto &c(e.component<component>());
		auto &pc(e.component<private_component>());
		auto &lvl(level::current());
		std::vector<std::string> spike_names;
		util::parse_names(c.spikes, spike_names);
		for(const auto &name : spike_names) {
			auto &spike(*lvl.entity_by_name(name));
			e.bind(spike);
			pc.spikes.push_back(&spike);
		}
	}
};
PODGE_REGISTER_SYSTEM(system);

struct activate_handler : entity_contact_handler {
	entity_system_mask mask() const {
		return {
			{"podge", "spike_trigger"}
		};
	}

	void begin_contact(entity_contact &contact) const {
		auto &e(contact.entity_b());
		auto &pc(e.component<private_component>());
		for(auto s : pc.spikes) {
			s->signal(spike::activate_, spike::activate_arg());
		}
		e.remove();
	}
};
PODGE_REGISTER_CONTACT_HANDLER(activate_handler);

} } }

#endif

#endif

