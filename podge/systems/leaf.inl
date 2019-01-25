#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_LEAF_HPP_FWD
#define PODGE_SYSTEM_LEAF_HPP_FWD

namespace podge { namespace systems { namespace leaf {

static const float hit_time(0.25f);

struct state {
	glm::vec2 anchor;
	float anchor_dist;
	int hits;
	int counter;
	boost::optional<float> t;
};

PODGE_PUBLIC_COMPONENT(component) {
	component() :
		ccw(false),
		hits(3)
	{
	}

	void validate(const context &ctx) const {
		if(hits <= 0) {
			throw validation_error("hits must be at least 1");
		}
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(bool, ccw),
		(int, hits));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_LEAF_HPP
#define PODGE_SYSTEM_LEAF_HPP

namespace podge { namespace systems { namespace leaf {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component, 
		(state, st),
		(std::vector<Mix_Chunk *>, samples));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "leaf";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(component),
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		auto &lvl(level::current());
		auto &pc(e.component<private_component>());
		resource_path dir("audio/tap/leaf");
		for(auto name : {"1.ogg", "2.ogg", "3.ogg"}) {
			pc.samples.push_back(lvl.pool().load_sample(dir/name));
		}
		e.body()->SetType(b2_staticBody);
	}

	void add(entity &e) const {
		auto &c(e.component<component>());
		auto &cc(e.component<core_component>());
		auto &pc(e.component<private_component>());
		auto pos(to_vec2(e.body()->GetPosition()));
		if(e.body()->GetAngle() != 0.0f) {
			throw std::runtime_error("Initial rotation on leaves is not supported");
		}
		if(cc.flip_horizontal) {
			pc.st.anchor = glm::vec2(pos.x + cc.width/2.0f, pos.y);
		} else {
			pc.st.anchor = glm::vec2(pos.x - cc.width/2.0f, pos.y);
		}
		pc.st.anchor_dist = glm::length(pc.st.anchor - pos);
		pc.st.hits = c.hits; 
		pc.st.counter = pc.st.hits;
		pc.st.t = boost::none;
	}

	void step(entity &e) const {
		auto &lvl(level::current());
		auto &c(e.component<component>());
		auto &cc(e.component<core_component>());
		auto &pc(e.component<private_component>());
		if(pc.st.t) {
			float t;
			*pc.st.t += 1.0f/hit_time*lvl.dt();
			if(*pc.st.t >= 1.0f) {
				t = 1.0f;
				pc.st.t = boost::none;
			} else {
				t = *pc.st.t;
			}
			auto x((std::atan((2.0f*t - 1.0f)*std::tan(1.0f))+1.0f)/2.0f);
			auto perc(1.0f - ((pc.st.counter - x)/pc.st.hits));
			glm::vec2 pos;
			float angle = c.ccw ? glm::radians(90.0f*perc) : glm::radians(-90.0f*perc);
			if(cc.flip_horizontal) {
				pos = pc.st.anchor + glm::vec2(std::cos(glm::radians(180.0f) + angle), std::sin(glm::radians(180.0f) + angle))*pc.st.anchor_dist;
			} else {
				pos = pc.st.anchor + glm::vec2(std::cos(angle), std::sin(angle))*pc.st.anchor_dist;
			}
			e.body()->SetTransform(to_b2Vec2(pos), angle);
			if(pc.st.t == boost::none) {
				pc.st.counter -= 1;
			}
		}
	}

	bool handle_input(entity &e, const input &in) const {
		auto &lvl(level::current());
		if(in.type == input::DOWN) {
			auto &pc(e.component<private_component>());
			if(!pc.st.t && pc.st.counter > 0 && util::point_in_body(e.body(), b2Vec2(in.x, in.y))) {
				pc.st.t.emplace(0.0f);
				std::uniform_int_distribution<> dist(0, pc.samples.size()-1);
				auto index(dist(lvl.rng()));
				auto sample(pc.samples[index]);
				Mix_PlayChannel(-1, sample, 0);
				return true;
			}
		}
		return false;
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

