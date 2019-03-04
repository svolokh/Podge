#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_CAMERA_HPP_FWD
#define PODGE_ENTITY_CAMERA_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace camera {

PODGE_COMPONENT(component) {
	BOOST_HANA_DEFINE_STRUCT(component,
		(boost::optional<glm::vec2>, target_pos),
		(boost::optional<float>, target_width));
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
		(boost::optional<glm::vec2>, current_pos),
		(boost::optional<float>, current_width));
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

	void step(entity &e) const {
		auto &lvl(level::current());
		auto &c(e.component<component>());
		auto &pc(e.component<private_component>());
		if(c.target_pos) {
			if(!pc.current_pos) {
				pc.current_pos = *c.target_pos;
			} else {
				pc.current_pos = 0.05f*(*c.target_pos) + 0.95f*(*pc.current_pos);
			}
		}
		if(c.target_width) {
			if(!pc.current_width) {
				pc.current_width = *c.target_width;
			} else {
				pc.current_width = 0.05f*(*c.target_width) + 0.95f*(*pc.current_width);
			}
		}
		if(pc.current_pos) {
			lvl.camera_position(*pc.current_pos);
		}
		if(pc.current_width) {
			auto aspect(lvl.camera_height()/lvl.camera_width()); 
			auto w(*pc.current_width);
			auto h(w*aspect);
			lvl.camera_width(w);
			lvl.camera_height(h);
		}
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

