#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_SPIKE_HPP_FWD
#define PODGE_ENTITY_SPIKE_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace spike {

enum signals : signal_t {
	activate_
};

struct activate_arg {
};

PODGE_PUBLIC_COMPONENT(component) {
	component() :
		speed(10.0f)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(float, speed));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_SPIKE_HPP
#define PODGE_ENTITY_SPIKE_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace spike {

PODGE_COMPONENT(private_component) {
	private_component() :
		activated(false)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(private_component,
		(bool, activated));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "spike";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(component),
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		e.body()->SetType(b2_staticBody);
		e.handle_signal<activate_arg>(activate_, std::bind(&system::handle_activate, this, std::placeholders::_1, std::placeholders::_2));
	}

	void step(entity &e) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		auto &c(e.component<component>());
		if(pc.activated) {
			auto angle(e.body()->GetAngle());
			glm::vec2 v(std::cos(angle), std::sin(angle));
			auto p(to_vec2(e.body()->GetPosition()) + lvl.dt()*c.speed*v);
			e.body()->SetTransform(to_b2Vec2(p), e.body()->GetAngle());
			if(!lvl.bounds().Contains(util::compute_body_bounds(e.body()))) {
				e.remove();
			}
		}
	}

protected:
	void handle_activate(entity &e, const activate_arg &arg) const {
		auto &pc(e.component<private_component>());
		pc.activated = true;
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

