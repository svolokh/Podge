#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_BARREL_TRIGGER_HPP_FWD
#define PODGE_ENTITY_BARREL_TRIGGER_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace barrel_trigger {

PODGE_PUBLIC_COMPONENT(component) {
	void validate(const context &ctx) const {
		if(ctx.is_map()) {
			if(barrels.empty()) {
				throw validation_error("barrels must be specified");
			}
			std::vector<std::string> names;
			util::parse_names(barrels, names);
			for(const auto &name : names) {
				if(!ctx.entity_exists(name)) {
					std::ostringstream oss;
					oss << "barrel '" << name << "' does not exist";
					throw validation_error(oss.str());
				}
			}
		}
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(std::string, barrels));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_BARREL_TRIGGER_HPP
#define PODGE_ENTITY_BARREL_TRIGGER_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace barrel_trigger {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(std::vector<entity *>, barrels));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "barrel_trigger";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(component),
			typeid(private_component)
		};
	}

	void add(entity &e) const {
		auto &lvl(level::current());
		auto &c(e.component<component>());
		auto &pc(e.component<private_component>());
		std::vector<std::string> names;
		util::parse_names(c.barrels, names);
		for(const auto &name : names) {
			auto &e(*lvl.entity_with_name(name));
			pc.barrels.emplace_back(&e);
		}
	}
};
PODGE_REGISTER_SYSTEM(system);

struct contact_handler : entity_contact_handler {
	entity_system_mask mask() const {
		return {
			{"podge", "barrel_trigger"}
		};
	}

	void begin_contact(entity_contact &contact) const {
		auto &t(contact.entity_b());
		auto &pc(t.component<private_component>());
		for(auto e : pc.barrels) {
			e->signal(barrel::activate_, barrel::activate_arg());
		}
		t.remove();
	}
};
PODGE_REGISTER_CONTACT_HANDLER(contact_handler);

} } }

#endif

#endif
