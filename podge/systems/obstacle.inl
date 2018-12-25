#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_OBSTACLE_HPP_FWD
#define PODGE_ENTITY_OBSTACLE_HPP_FWD

#include <podge.hpp>
#include <string>
#include <glm/vec2.hpp>
#include <systems/podge.inl>

namespace podge { namespace systems { namespace obstacle {

struct obstacle_state {
	int z0;
};

struct orbit_state {
	entity *anchor;
	float theta0;
	float d;
};

struct oscillate_state {
	b2Vec2 p0;
};

struct pulsate_state {
	float w0;
	float h0;
};

struct smash_state {
	glm::vec2 initial_center;
	glm::vec2 dir; // the direction to the anchor
	float initial_dist; // initial distance from pos to the anchor
	entity *anchor;
};

struct spin_state {
	float theta0;
};

PODGE_COMPONENT(component) {
	enum { is_public = true };

	component() :
		orbit(false),
		orbit_ccw(false),
		orbit_period(10.0f),
		oscillate(false),
		oscillate_angle(0.0f),
		oscillate_distance(1.0f),
		oscillate_period(10.0f),
		oscillate_phase(0.0f),
		pulsate(false),
		pulsate_phase(0.0f),
		pulsate_time_advanced(1.0f),
		pulsate_time_receded(1.0f),
		pulsate_time_transition(0.5f),
		smash(false),
		smash_fall_time(0.5f),
		smash_hover_time(5.0f),
		smash_phase(0.0f),
		smash_recede_time(1.0f),
		smash_sit_time(1.0f),
		spin(false),
		spin_ccw(false),
		spin_period(10.0f)
	{
	}

	void validate(const context &ctx) const {
		if(orbit) {
			if(orbit_anchor.empty()) {
				throw validation_error("orbit_anchor must be defined");
			}
			if(!ctx.entity_exists(orbit_anchor)) {
				std::ostringstream oss;
				oss << "orbit anchor '" << orbit_anchor << "' does not exist";
				throw validation_error(oss.str());
			}
		}
		if(smash) {
			if(smash_anchor.empty()) {
				throw validation_error("smash_anchor must be defined");
			}
			if(!ctx.entity_exists(smash_anchor)) {
				std::ostringstream oss;
				oss << "smash anchor '" << smash_anchor << "' does not exist";
				throw validation_error(oss.str());
			}
		}
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(bool, orbit),
		(std::string, orbit_anchor),
		(bool, orbit_ccw),
		(float, orbit_period),
		(bool, oscillate),
		(float, oscillate_angle),
		(float, oscillate_distance),
		(float, oscillate_period),
		(float, oscillate_phase),
		(bool, pulsate),
		(float, pulsate_phase),
		(float, pulsate_time_advanced),
		(float, pulsate_time_receded),
		(float, pulsate_time_transition),
		(bool, smash),
		(std::string, smash_anchor),
		(float, smash_fall_time),
		(float, smash_hover_time),
		(float, smash_phase),
		(float, smash_recede_time),
		(float, smash_sit_time),
		(bool, spin),
		(bool, spin_ccw),
		(float, spin_period));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_OBSTACLE_HPP
#define PODGE_ENTITY_OBSTACLE_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace obstacle {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(obstacle_state, obstacle_st),
		(boost::optional<orbit_state>, orbit_st),
		(boost::optional<oscillate_state>, oscillate_st),
		(boost::optional<pulsate_state>, pulsate_st),
		(boost::optional<smash_state>, smash_st),
		(boost::optional<spin_state>, spin_st));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "obstacle";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(basic::component),
			typeid(component),
			typeid(podge::obstacle_component),
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		e.body()->SetType(b2_staticBody);
	}

	void add(entity &e) const {
		auto &lvl(level::current());
		auto &c(e.component<component>());
		auto &pc(e.component<private_component>());
		auto &cc(e.component<core_component>());
		pc.obstacle_st.z0 = e.z_index();
		if(c.orbit) {
			pc.orbit_st.emplace();
			auto &st(*pc.orbit_st);
			st.anchor = &*lvl.entity_by_name(c.orbit_anchor);
			e.bind(*st.anchor);
			auto pa(st.anchor->body()->GetPosition());
			auto pe(e.body()->GetPosition());
			st.theta0 = std::atan2(pe.y - pa.y, pe.x - pa.x);
			st.d = glm::length(glm::vec2(pe.x - pa.x, pe.y - pa.y));
		}
		if(c.oscillate) {
			pc.oscillate_st.emplace();
			auto &st(*pc.oscillate_st);
			st.p0 = e.body()->GetPosition();
		}
		if(c.pulsate) {
			pc.pulsate_st.emplace();
			auto &st(*pc.pulsate_st);
			st.w0 = cc.width;
			st.h0 = cc.height; 
		}
		if(c.smash) {
			pc.smash_st.emplace();
			auto &st(*pc.smash_st);
			st.anchor = &*lvl.entity_by_name(c.smash_anchor);
			e.bind(*st.anchor);
			auto pa(st.anchor->body()->GetPosition());
			auto pb(util::ray_cast(pa, e.body()));
			if(!pb) {
				throw std::runtime_error("Failed to cast ray from smash anchor to obstacle");
			}
			auto dir(to_vec2(*pb) - to_vec2(pa));
			st.initial_center = to_vec2(e.body()->GetPosition());
			st.dir = -glm::normalize(dir);
			st.initial_dist = glm::length(dir);
		}
		if(c.spin) {
			pc.spin_st.emplace();
			auto &st(*pc.spin_st);
			st.theta0 = e.body()->GetAngle();
		}
	}

	void step(entity &e) const {
		auto &lvl(level::current());
		bool fatal = false;
		bool collides = true;
		int z_offset = 0;
		auto &pc(e.component<private_component>());
		auto &c(e.component<component>());
		auto &bc(e.component<basic::component>());
		auto &cc(e.component<core_component>());
		auto &poc(e.component<podge::obstacle_component>());
		if(c.orbit) {
			auto &st(*pc.orbit_st);
			auto pa(st.anchor->body()->GetPosition());
			auto T(c.orbit_period);
			auto t(lvl.time());
			b2Vec2 pos;
			if(c.orbit_ccw) {
				pos.x = pa.x + st.d*std::cos(st.theta0 + 2.0f*b2_pi/T*t);
				pos.y = pa.y + st.d*std::sin(st.theta0 + 2.0f*b2_pi/T*t);
			} else {
				pos.x = pa.x + st.d*std::cos(st.theta0 - 2.0f*b2_pi/T*t);
				pos.y = pa.y + st.d*std::sin(st.theta0 - 2.0f*b2_pi/T*t);
			}
			e.body()->SetTransform(pos, e.body()->GetAngle());
		}
		if(c.oscillate) {
			auto &st(*pc.oscillate_st);
			auto T(c.oscillate_period);
			auto r(c.oscillate_distance);
			auto t0(c.oscillate_phase);
			const auto s(0.05f);
			auto div(-2.0f*std::acos(-s + 1.0f)/b2_pi + 1.0f);
			auto x((lvl.time() + t0)/T);
			auto d(r*(1 - 2*std::acos((1 - s)*std::sin(2*b2_pi*x))/b2_pi)/div);
			auto theta(c.oscillate_angle*b2_pi/180.0f);
			b2Vec2 dir(std::cos(theta), std::sin(theta));
			b2Vec2 pos(st.p0.x + d*dir.x, st.p0.y  + d*dir.y);
			e.body()->SetTransform(pos, e.body()->GetAngle());
		}
		if(c.pulsate) { 
			auto &st(*pc.pulsate_st);
			auto phase(c.pulsate_phase);
			auto tr(c.pulsate_time_receded);
			auto ta(c.pulsate_time_advanced);
			auto tt(c.pulsate_time_transition);
			auto t(std::fmod(lvl.time() + phase, ta + tr + 2.0f*tt));
			float s;
			if(t < tr) { // receded
				s = 0.0f;
				collides = false;
				z_offset -= 1;
			} else if(t < tr + tt) { // advancing
				s = (t - tr)/tt;
				if(s > 0.5f) {
					fatal = true;
					collides = true;
				} else {
					collides = false;
				}
				z_offset -= 1;
			} else if(t < tr + tt + ta) { // advanced
				s = 1.0f;
			} else { // t < tr + tt + ta + tt (receding)
				s = 1.0f - (t - tr - tt - ta)/tt;
				collides = s > 0.5f;
				z_offset -= 1;
			}
			bc.opacity = 0.5f + 0.5f*s;
			cc.width = st.w0 * (0.75f + 0.25f*s);
			cc.height = st.h0 * (0.75f + 0.25f*s);
		}
		if(c.smash) {
			auto &st(*pc.smash_st);
			auto th(c.smash_hover_time);
			auto tf(c.smash_fall_time);
			auto ts(c.smash_sit_time);
			auto tr(c.smash_recede_time);
			auto phase(c.smash_phase);
			auto t(std::fmod(lvl.time() + phase, th + tf + ts + tr));
			float d;
			if(t < th) { 
				// hovering
				d = 1.0f;
			} else if(t < th + tf) {
				// falling
				auto x((t - th)/tf);
				d = 1.0f - x*x*x*x;
				if(d < 0.5f) {
					fatal = true;
				}
			} else if(t < th + tf + ts) {
				// sitting
				d = 0.0f;
			} else { // t < th + tf + ts + tr
				// receding
				auto x((t - th - tf - ts)/tr);
				d = x;
			}
			auto c(st.initial_center + (1.0f - d)*st.initial_dist*st.dir);
			e.body()->SetTransform(to_b2Vec2(c), e.body()->GetAngle());
		}
		if(c.spin) {
			auto &st(*pc.spin_st);
			auto theta(2.0f*b2_pi*std::fmod(lvl.time(), c.spin_period)/c.spin_period);
			if(!c.spin_ccw) {
				theta *= -1.0f;
			}
			e.body()->SetTransform(e.body()->GetPosition(), st.theta0 + theta);
		}
		{
			auto &st(pc.obstacle_st);
			poc.damage_multiplier = fatal ? 10.0f : 1.0f;
			b2Filter filter;
			if(!collides) {
				filter.maskBits = 0;
			} 
			for(auto fixture(e.body()->GetFixtureList()); fixture != nullptr; fixture = fixture->GetNext()) {
				fixture->SetFilterData(filter);
			}
			e.z_index(st.z0 + z_offset);
		}
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

