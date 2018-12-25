#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_BARREL_HPP_FWD
#define PODGE_ENTITY_BARREL_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace barrel {

struct activate_arg {
};

enum signals : signal_t {
	activate_
};

} } }

#endif

#else

#ifndef PODGE_ENTITY_BARREL_HPP
#define PODGE_ENTITY_BARREL_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace barrel {

PODGE_COMPONENT(private_component) {
	private_component() :
		do_activate(false)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(private_component,
		(bool, do_activate));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "barrel";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		e.handle_signal<activate_arg>(activate_, std::bind(&system::handle_activate, this, std::placeholders::_1, std::placeholders::_2));
		e.body()->SetFixedRotation(true);
		e.body()->SetActive(false);
		e.body()->SetGravityScale(0.5f);
	}

	void step(entity &e) const {
		auto &pc(e.component<private_component>());
		auto &lvl(level::current());
		auto camb(lvl.camera_bounds());
		auto bounds(compute_body_bounds(e.body()));
		if(bounds.upperBound.y < camb.lowerBound.y) {
			e.remove();
		}
		if(pc.do_activate) {
			e.body()->SetActive(true);
			pc.do_activate = false;
		}
	}

	void handle_activate(entity &e, const activate_arg &) const {
		auto &pc(e.component<private_component>());
		pc.do_activate = true;
	}
};
PODGE_REGISTER_SYSTEM(system);

// breaks the barrel when it hits something
struct contact_handler : entity_contact_handler {
	entity_system_mask mask() const {
		return {
			{"barrel"}
		};
	}

	void begin_contact(entity_contact &contact) const {
		contact.entity_a().remove();
	}
};
PODGE_REGISTER_CONTACT_HANDLER(contact_handler);

} } }

#endif

#endif

