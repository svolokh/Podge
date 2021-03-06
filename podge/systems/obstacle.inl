#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_OBSTACLE_HPP_FWD
#define PODGE_ENTITY_OBSTACLE_HPP_FWD

#include <podge.hpp>
#include <string>
#include <glm/vec2.hpp>
#include <systems/podge.inl>

namespace podge { namespace systems { namespace obstacle {

PODGE_PUBLIC_COMPONENT(component) {
	component() :
		orbit(false),
		orbit_ccw(false),
		orbit_period(10.0f),
		oscillate(false),
		oscillate_angle(0.0f),
		oscillate_distance(1.0f),
		oscillate_period(10.0f),
		oscillate_phase(0.0f),
		follow_path(false),
		follow_path_shape(""),
		follow_path_ccw(false),
		follow_path_period(10.0f),
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
		if(ctx.is_map()) {
			if(follow_path) {
				if(follow_path_shape.empty()) {
					throw validation_error("follow_path_shape must be defined");
				}
				if(!ctx.entity_exists(follow_path_shape)) {
					std::ostringstream oss;
					oss << "follow_path_shape object '" << follow_path_shape << "' does not exist";
					throw validation_error(oss.str());
				}
			}
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
		(bool, follow_path),
		(std::string, follow_path_shape),
		(bool, follow_path_ccw),
		(float, follow_path_period),
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

struct follow_path_state {
	struct circle {
		glm::vec2 center;
		float radius;
	};

	struct edge {
		glm::vec2 p0;
		glm::vec2 p1;
		float weight;
	};

	struct polyline {
		std::vector<edge> edges;
	};

	typedef boost::variant<circle, polyline> path_type;

	path_type path;
	float phase_perc; // phase as a percentage of the period
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

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(bool, inactive),
		(obstacle_state, obstacle_st),
		(boost::optional<orbit_state>, orbit_st),
		(boost::optional<oscillate_state>, oscillate_st),
		(boost::optional<follow_path_state>, follow_path_st),
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
		auto active(false);
		pc.obstacle_st.z0 = e.z_index();
		if(c.follow_path) {
			active = true;
			pc.follow_path_st.emplace();
			auto &st(*pc.follow_path_st);
			auto &path(*lvl.entity_with_name(c.follow_path_shape));
			if(std::strcmp(path.type().name(), "path_shape") != 0) {
				PODGE_THROW_ERROR();
			}
			const auto &shape(*path.component<core_component>().collision_shapes[0]->shape);
			boost::optional<const b2Vec2 *> verts;
			std::size_t verts_count;
			boost::optional<float> phase_perc;
			switch(shape.GetType()) {
				case b2Shape::e_circle: {
					const auto &shp(static_cast<const b2CircleShape &>(shape));
					follow_path_state::circle circ;
					circ.center = to_vec2(b2Mul(path.body()->GetTransform(), shp.m_p));
					circ.radius = shp.m_radius;
					auto p(to_vec2(e.body()->GetPosition()));
					auto v(p - circ.center);
					if(glm::length2(v) < std::numeric_limits<float>::epsilon()) {
						phase_perc = 0.0f;
					} else {
						auto theta0(std::atan2f(v.y, v.x));
						if(theta0 < 0) {
							theta0 += 2.0f*b2_pi;
						}
						phase_perc = theta0/(2.0f*b2_pi);
					}
					st.path = std::move(circ);
					break;
				}
				case b2Shape::e_chain: {
					const auto &shp(static_cast<const b2ChainShape &>(shape));
					verts = shp.m_vertices;
					verts_count = shp.m_count;
					break;
				}
				case b2Shape::e_polygon: {
					const auto &shp(static_cast<const b2PolygonShape &>(shape)); 
					verts = shp.m_vertices;
					verts_count = shp.m_count;
					break;
				}
				default:
					throw std::runtime_error("unexpected fixture type");
			}
			if(verts) {
				if(verts_count < 2) {
					throw std::runtime_error("expected at least 2 points in polyline");
				}
				auto vs(*verts);
				const auto &xf(path.body()->GetTransform());
				// chain or polygon
				follow_path_state::polyline pl;
				for(auto i(0); i != verts_count; ++i) {
					auto i0(i);
					auto i1((i0 + 1) % verts_count);
					auto v0(to_vec2(b2Mul(xf, vs[i0])));
					auto v1(to_vec2(b2Mul(xf, vs[i1])));
					follow_path_state::edge e{v0, v1};
					pl.edges.emplace_back(std::move(e));
				}
				auto tot(0.0f);
				for(const auto &e : pl.edges) {
					tot += glm::distance(e.p0, e.p1);
				}
				for(auto &e : pl.edges) {
					e.weight = glm::distance(e.p0, e.p1)/tot;
				}
				{
					// compute i0, s0
					int i0;
					float s0;
					assert(pl.edges.size() > 0);
					auto min_dist2(std::numeric_limits<float>::infinity());
					auto p(to_vec2(e.body()->GetPosition()));
					for(std::size_t i(0), n(pl.edges.size()); i != n; ++i) {
						const auto &e(pl.edges[i]);
						auto l(e.p1 - e.p0);
						auto x(p - e.p0);
						auto s(glm::dot(x, l)/glm::length(l));
						if(s >= 0.0f && s <= 1.0f) {
							auto xp(s*l);
							auto dist2(glm::distance2(x, xp));
							if(dist2 < min_dist2) {
								min_dist2 = dist2;
								i0 = i;
								s0 = s;
							}
						}
					}
					// compute phase_perc from i0, s0
					phase_perc = (float(i0) + s0)/pl.edges.size();
				}
				st.path = std::move(pl);
			}
			st.phase_perc = *phase_perc;
		}
		if(c.orbit) {
			active = true;
			pc.orbit_st.emplace();
			auto &st(*pc.orbit_st);
			st.anchor = &*lvl.entity_with_name(c.orbit_anchor);
			e.bind(*st.anchor);
			auto pa(st.anchor->body()->GetPosition());
			auto pe(e.body()->GetPosition());
			st.theta0 = std::atan2(pe.y - pa.y, pe.x - pa.x);
			st.d = glm::length(glm::vec2(pe.x - pa.x, pe.y - pa.y));
		}
		if(c.oscillate) {
			active = true;
			pc.oscillate_st.emplace();
			auto &st(*pc.oscillate_st);
			st.p0 = e.body()->GetPosition();
		}
		if(c.pulsate) {
			active = true;
			pc.pulsate_st.emplace();
			auto &st(*pc.pulsate_st);
			st.w0 = cc.width;
			st.h0 = cc.height; 
		}
		if(c.smash) {
			active = true;
			pc.smash_st.emplace();
			auto &st(*pc.smash_st);
			st.anchor = &*lvl.entity_with_name(c.smash_anchor);
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
			active = true;
			pc.spin_st.emplace();
			auto &st(*pc.spin_st);
			st.theta0 = e.body()->GetAngle();
		}
		pc.inactive = !active;
	}

	void step(entity &e) const {
		auto &pc(e.component<private_component>());
		if(pc.inactive) {
			return;
		}

		auto &lvl(level::current());
		bool fatal = false;
		bool collides = true;
		int z_offset = 0;
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
		if(c.follow_path) {
			auto &st(*pc.follow_path_st);
			auto T(c.follow_path_period);
			float t;
			if(c.follow_path_ccw) {
				t = std::fmod(st.phase_perc*T + lvl.time(), T);
			} else {
				t = std::fmod(st.phase_perc*T - lvl.time(), T);
				if(t < 0.0f) {
					t += T;
				}
			}
			boost::optional<glm::vec2> p;
			if(st.path.type() == typeid(follow_path_state::circle)) {
				auto &circ(boost::get<follow_path_state::circle>(st.path));
				auto theta(2.0f*b2_pi*t/T);
				auto v(circ.radius*glm::vec2(std::cos(theta), std::sin(theta)));
				p = circ.center + v;
			} else if(st.path.type() == typeid(follow_path_state::polyline)) {
				auto &pl(boost::get<follow_path_state::polyline>(st.path));
				float t1(0.0f);
				for(std::size_t i(0), n(pl.edges.size()); i != n; ++i) {
					const auto &e(pl.edges[i]);
					auto t0(t1);
					t1 += T*e.weight;
					if(t < t1) {
						auto s((t - t0)/(t1 - t0));
						p = e.p0 + s*(e.p1 - e.p0);
						break;
					}
				}
				if(!p) {
					PODGE_THROW_ERROR();
				}
			} else {
				PODGE_THROW_ERROR();
			}
			e.body()->SetTransform(to_b2Vec2(*p), e.body()->GetAngle());
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
			poc.extra_damage = fatal ? 1 : 0;
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

