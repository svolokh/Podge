#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_AMBIENT_SOURCE_HPP_FWD
#define PODGE_SYSTEM_AMBIENT_SOURCE_HPP_FWD

namespace podge { namespace systems { namespace ambient_source {

PODGE_PUBLIC_COMPONENT(public_component) {
	public_component() :
		ambient_sound(""),
		ambient_radius(10.0f)
	{
	}

	void validate(const context &ctx) const {
		if(!ambient_sound.empty()) {
			if(!resource_exists(ambient_sound)) {
				throw validation_error("the file specified by 'ambient_sound' does not exist");
			}
		}
	}

	BOOST_HANA_DEFINE_STRUCT(public_component,
		(std::string, ambient_sound),
		(float, ambient_radius));
};
PODGE_REGISTER_COMPONENT(public_component);

PODGE_COMPONENT(component) {
	component() :
		active(false)
	{
	}

	BOOST_HANA_DEFINE_STRUCT(component,
		(bool, active));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_AMBIENT_SOURCE_HPP
#define PODGE_SYSTEM_AMBIENT_SOURCE_HPP

namespace podge { namespace systems { namespace ambient_source {

PODGE_COMPONENT(private_component) {
	BOOST_HANA_DEFINE_STRUCT(private_component,
		(boost::optional<Mix_Chunk *>, sample),
		(std::vector<entity *>, podges_buf),
		(std::shared_ptr<mix_channel>, channel));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
	const char *name() const {
		return "ambient_source";
	}

	std::vector<std::type_index> components() const {
		return {
			typeid(public_component),
			typeid(component),
			typeid(private_component)
		};
	}

	void init(entity &e) const {
		auto &lvl(level::current());
		auto &pubc(e.component<public_component>());
		auto &privc(e.component<private_component>());
		privc.channel = std::make_shared<mix_channel>();
		if(!pubc.ambient_sound.empty()) {
			privc.sample.emplace(lvl.pool().load_sample(pubc.ambient_sound));
		}
	}

	void step(entity &e) const {
		auto &lvl(level::current());
		auto &pubc(e.component<public_component>());
		auto &privc(e.component<private_component>());
		if(!*privc.sample) {
			return;
		}
		std::vector<entity *> podges_buf;
		lvl.entities_with_system("podge", podges_buf);
		if(podges_buf.size() != 1) {
			PODGE_THROW_ERROR();
		}
		auto &p(*podges_buf[0]);
		auto e_pos(to_vec2(e.body()->GetPosition()));
		auto p_pos(to_vec2(p.body()->GetPosition()));
		auto dsq(glm::distance2(p_pos, e_pos));
		auto rsq(pubc.ambient_radius*pubc.ambient_radius);
		if(dsq > rsq) {
			privc.channel->clear();
		} else {
			auto volume{int(dsq/rsq*128)};
			if(!*privc.channel) {
				*privc.channel = mix_channel::next();
				if(*privc.channel) { // if we failed to get a channel this time, too bad, we'll get a channel another time
					Mix_Volume(privc.channel->id(), volume);
					Mix_PlayChannel(privc.channel->id(), *privc.sample, -1);
				}
			} else {
				Mix_Volume(privc.channel->id(), volume);
			}
		}
	}
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

