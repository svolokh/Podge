#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_ENTITY_PODGE_HPP_FWD
#define PODGE_ENTITY_PODGE_HPP_FWD

#include <podge.hpp>

namespace podge { namespace systems { namespace podge {

static const auto buoyant_force(0.25f);

enum signals : signal_t {
	hit_,
	shove_
};

struct hit_arg {
	b2Vec2 direction_from;
	int amount;
};

// podge attributes
PODGE_COMPONENT(component) {
	component() :
		invuln(0.0f),
		hp(2)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(float, invuln),
		(int, hp));
};
PODGE_REGISTER_COMPONENT(component);

// component attached to other entities Podge collides with
PODGE_COMPONENT(obstacle_component) {
	obstacle_component() :
		extra_damage(0)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(obstacle_component,
		(int, extra_damage) // amount of extra damage Podge should take from colliding with the other entity
	); 
};
PODGE_REGISTER_COMPONENT(obstacle_component);

} } }

#endif

#else

#ifndef PODGE_ENTITY_PODGE_HPP
#define PODGE_ENTITY_PODGE_HPP

#include <podge.hpp>

namespace podge { namespace systems { namespace podge {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(std::vector<Mix_Chunk *>, pops));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "podge";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(basic::component),
			typeid(component),
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		auto &bc(e.component<basic::component>());
		bc.keyframe = "hp2";
		{
			resource_path popdir("audio/bubble/pop");
			for(auto name : {"1.wav", "2.wav", "3.wav"}) {
				pc.pops.emplace_back(lvl.pool().load_sample(popdir/name));
			}
		}
		e.body()->SetFixedRotation(true);
		e.body()->SetGravityScale(0.0f);
		e.handle_signal<b2Vec2>(shove_, [](entity &e, const b2Vec2 &force) {
			e.body()->ApplyForce(force, e.body()->GetPosition(), true);
		});
		e.handle_signal<hit_arg>(hit_, [](entity &e, const hit_arg &info) {
			auto &lvl(level::current());
			auto &pc(e.component<private_component>());
			auto &bc(e.component<basic::component>());
			auto &c(e.component<component>());
			if(!lvl.demo_mode) {
				c.hp -= info.amount;
			}
			if(info.amount > 0) {
				auto sample(pc.pops[std::uniform_int_distribution<>(0, pc.pops.size()-1)(lvl.rng())]);
				Mix_PlayChannel(-1, sample, 0);
				lvl.num_hits += 1;
			}
			if(c.hp <= 0) {
				auto &lvl(level::current());
				lvl.exit(level_exits::failure());
			} else {
				switch(c.hp) {
					case 2:
						bc.keyframe = "hp2";
						break;
					case 1:
						bc.keyframe = "hp1";
						break;
					default:
						PODGE_THROW_ERROR();
				}
				if(info.amount > 0) {
					c.invuln += 3.0f;
					b2Vec2 force(15.0f * info.direction_from.x, 15.0f * info.direction_from.y);
					e.signal(shove_, force);
				}
			}
		});
		entity_spec camera_spec(registry.type("camera"));
		auto &cam(e.lyr().create_entity({0.0f, 0.0f}, std::move(camera_spec)));
		cam.bind(e);
		e.z_index(-1);
	}

	void step(entity &e) const {
		auto &cc(e.component<core_component>());
		auto &c(e.component<component>());
		// invincibility blinking
		auto &lvl(level::current());
		if(c.invuln > 0.0f) {
			bool visible;
			c.invuln -= lvl.dt();
			if(c.invuln <= 0.0f) {
				c.invuln = 0.0f;
				visible = true;
			} else {
				visible = int(c.invuln*1000) % 100 < 50;
			}
			e.visible(visible);
		}
		// buoyancy
		{
			b2Vec2 fb(0.0f, buoyant_force);
			e.body()->ApplyForce(fb, e.body()->GetPosition(), true);
		}
		// drag
		util::apply_air_drag(e, cc.width);
		// slightly pull away from damaging surfaces
		{
			static const auto num_rays(16);
			static const auto max_dist(1.5f); // maximum distance at which the force comes into effect
			static const auto strength(0.25f); // strength of pull away force
			struct callback : b2RayCastCallback {
				callback(entity &e, boost::optional<float> &dist) :
					e(e),
					dist(dist)
				{
				}

				float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
					entity &o(*static_cast<entity *>(fixture->GetBody()->GetUserData()));
					if(&o == &e) {
						return 1;
					}
					auto &fc(o.fixture_data(fixture).component<fixture_component>());
					if(fc.repulsive) {
						dist = glm::length(to_vec2(point) - to_vec2(e.body()->GetPosition()));
					}
					return 0;
				}

				entity &e;
				boost::optional<float> &dist;
			};
			auto step(360.0f/num_rays);
			std::vector<std::pair<glm::vec2, float>> dists; // {ray direction, weight}
			for(auto angle(0.0f); angle < 360.0f; angle += step) {
				boost::optional<float> dist;
				callback cb(e, dist);
				glm::vec2 dir(std::cos(glm::radians(angle)), std::sin(glm::radians(angle)));
				b2Vec2 pt1(e.body()->GetPosition());
				b2Vec2 pt2(pt1.x + max_dist*dir.x, pt1.y + max_dist*dir.y);
				lvl.world().RayCast(&cb, pt1, pt2);
				if(dist) {
					dists.emplace_back(dir, std::numeric_limits<float>::epsilon() + max_dist - *dist);
				}
			}
			if(!dists.empty()) {
				auto total(0.0f);
				for(const auto &p : dists) {
					total += p.second;
				}
				glm::vec2 force_dir(0.0f, 0.0f);
				for(const auto &p : dists) {
					force_dir += -p.second/total*p.first;
				}
				force_dir = glm::normalize(force_dir);
				e.body()->ApplyForceToCenter(to_b2Vec2(strength*force_dir), true);
			}
		}
	}

	bool should_handle_input_last() const {
		return true;
	}

	bool handle_input(entity &e, const input &in) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		auto &cc(e.component<core_component>());
		switch(in.type) {
			case input::DOWN:
			{
				auto in_pos(glm::vec2(in.x, in.y));
				auto hit(util::ray_cast(to_b2Vec2(in_pos), e.body()));
				if(hit) {
					auto hit_pos(to_vec2(*hit));
					auto len(glm::length(hit_pos - in_pos));
					auto perc(glm::clamp(1.0f - len/(lvl.camera_height()/2.0f - cc.height/2.0f), 0.25f, 1.0f));
					auto strength(150.0f*perc);
					auto dir(glm::normalize(hit_pos - in_pos));
					auto force(to_b2Vec2(strength*dir));
					e.body()->ApplyForce(force, to_b2Vec2(hit_pos), true);
				}
				break;
			}
			default:
				break;
		}
		return false;
	}
};
PODGE_REGISTER_SYSTEM(system);

struct damage_contact_handler : entity_contact_handler {
	entity_system_mask mask() const {
		return {
			{"podge"}
		};
	}

	void pre_solve(entity_contact &contact) const {
		auto &p(contact.entity_a());
		auto &obst(contact.entity_b());
		auto &pc(p.component<component>());
		auto &fc(obst.fixture_data(contact.fixture_b()).component<fixture_component>());
		if(pc.invuln == 0.0f) {
			int extra;
			if(obst.has_component<obstacle_component>()) {
				auto &oc(obst.component<obstacle_component>());
				extra = oc.extra_damage;
			} else {
				extra = 0;
			}
			auto damage(fc.damage + extra);
			if(damage > 0) {
				hit_arg info;
				info.amount = fc.damage + extra;
				info.direction_from = -contact.normal();
				p.signal(hit_, info);
			}
		}
	}
};
PODGE_REGISTER_CONTACT_HANDLER(damage_contact_handler);

} } }

#endif

#endif

