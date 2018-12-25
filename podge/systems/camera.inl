#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_CAMERA_HPP_FWD
#define PODGE_ENTITY_CAMERA_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace camera {

PODGE_COMPONENT(component) {
	BOOST_HANA_DEFINE_STRUCT(component,
		(entity *, podge));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_CAMERA_HPP
#define PODGE_ENTITY_CAMERA_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace camera {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(glm::vec2, pos));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "camera";
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
		pc.pos = to_vec2(c.podge->body()->GetPosition());
	}

	void step(entity &e) const {
		auto &c(e.component<component>());
		auto &pc(e.component<private_component>());
		auto &lvl(level::current());
		pc.pos = 0.1f*to_vec2(c.podge->body()->GetPosition()) + 0.9f*pc.pos;
		lvl.camera_position(pc.pos);
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

