#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_BUMP_BAT_HPP_FWD
#define PODGE_ENTITY_BUMP_BAT_HPP_FWD

namespace podge { namespace systems { namespace bump_bat {

static const float approach_dist(4.0f);
static const float approach_force(0.5f);
static const float speedup_dist(2.0f);
static const float speedup_force(2.0f);
static const float bump_force(100.0f);
static const float return_force(2.0f);
static const float snap_dist(0.25f);

struct state {
	glm::vec2 home;
	entity *podge;
	entity *bounds;
};

PODGE_PUBLIC_COMPONENT(component) {
	void validate(const context &ctx) const {
		if(ctx.is_map()) {
			if(bounds.empty()) {
				throw validation_error("the name of this bump bat's bounds is required");
			}
			if(!ctx.entity_exists(bounds)) {
				std::ostringstream oss;
				oss << "bounds '" << bounds << "' does not exist";
				throw validation_error(oss.str());
			}
		}
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(std::string, bounds));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_BUMP_BAT_HPP
#define PODGE_ENTITY_BUMP_BAT_HPP

namespace podge { namespace systems { namespace bump_bat {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(state, st));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "bump_bat";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(component),
			typeid(private_component),
			typeid(podge::obstacle_component)
		};
	}

	void init(entity &e) const {
		e.body()->SetGravityScale(0.0f);
		e.body()->SetFixedRotation(true);
		for(auto fixture(e.body()->GetFixtureList()); fixture != nullptr; fixture = fixture->GetNext()) {
			auto &fc(e.fixture_data(fixture).component<fixture_component>());
			fc.repulsive = true;
		}
	}

	void add(entity &e) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		auto &c(e.component<component>());
		std::vector<entity *> podges;
		lvl.entities_with_system("podge", podges);
		if(podges.size() != 1) {
			throw std::runtime_error("Expected exactly one Podge");
		}
		pc.st.home = to_vec2(e.body()->GetPosition());
		pc.st.podge = podges[0];
		pc.st.bounds = &*lvl.entity_by_name(c.bounds);
	}

private:
	bool in_bounds(entity &e, entity &bounds) const {
		for(auto c(e.body()->GetContactList()); c != nullptr; c = c->next) {
			auto &o(util::entity_from_body(c->other));
			if(&o == &bounds && c->contact->IsTouching()) {
				return true;
			}
		}
		return false;
	}

public:
	void step(entity &e) const {
		auto &pc(e.component<private_component>());
		auto &cc(e.component<core_component>());
		auto &st(pc.st);
		auto pos(to_vec2(e.body()->GetPosition()));
		glm::vec2 dest;
		float force;
		bool allow_snap;
		if(in_bounds(e, *st.bounds) && in_bounds(*st.podge, *st.bounds)) {
			dest = to_vec2(st.podge->body()->GetPosition());
			auto dist(glm::length(dest - pos));
			if(dist <= approach_dist) {
				// move towards Podge
				if(dist <= speedup_dist) {
					force = speedup_force;
				} else {
					force = approach_force;
				}
			} else {
				force = 0.0f;
			}
			allow_snap = false;
		} else {
			dest = st.home;
			force = return_force;
			allow_snap = true;
		}
		auto dist(glm::length(dest - pos));
		if(dist > 0.0f) {
			if(allow_snap && dist <= snap_dist) {
				e.body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				e.body()->SetTransform(to_b2Vec2(pos + 0.1f*(dest - pos)), e.body()->GetAngle());
			} else {
				auto dir(glm::normalize(dest - pos));
				e.body()->ApplyForceToCenter(to_b2Vec2(force*dir), true);
				auto vel(to_vec2(e.body()->GetLinearVelocity()));
				auto v(glm::length(vel));
				if(v > 0.0f && (dist > approach_dist || glm::dot(vel, dir) >= 0.0f)) {
					// stop moving in current direction if moving away from destination
					auto vdir(glm::normalize(vel));
					auto fd(to_b2Vec2(-0.5f*v*v*vdir));
					e.body()->ApplyForceToCenter(fd, true);
				}
			}
		}
		// apply drag
		util::apply_air_drag(e, cc.width);
	}
};
PODGE_REGISTER_SYSTEM(system);

struct bump_contact_handler : entity_contact_handler {
	entity_system_mask mask() const {
		return {
			{"bump_bat", "podge"},
			{"bump_bat", "bump_bat"}
		};
	}

	void pre_solve(entity_contact &contact) const {
		auto fb(to_b2Vec2(bump_force*to_vec2(contact.normal())));
		contact.entity_b().body()->ApplyForce(fb, contact.point(), true);
	}
};
PODGE_REGISTER_CONTACT_HANDLER(bump_contact_handler);

} } }

#endif

#endif

