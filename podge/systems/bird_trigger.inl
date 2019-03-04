#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_BIRD_TRIGGER_HPP_FWD
#define PODGE_SYSTEM_BIRD_TRIGGER_HPP_FWD

namespace podge { namespace systems { namespace bird_trigger {

PODGE_PUBLIC_COMPONENT(public_component) {
    void validate(const context &ctx) const {
        if(ctx.is_map()) {
            std::vector<std::string> bird_names;
            util::parse_names(birds, bird_names);
            for(const auto &name : bird_names) {
                if(!ctx.entity_exists(name)) {
                    std::ostringstream oss;
                    oss << "bird '" << name << "' does not exist";
                    throw validation_error(oss.str());
                }
            }
        }
    }

    BOOST_HANA_DEFINE_STRUCT(public_component,
        (std::string, birds));
};
PODGE_REGISTER_COMPONENT(public_component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_BIRD_TRIGGER_HPP
#define PODGE_SYSTEM_BIRD_TRIGGER_HPP

namespace podge { namespace systems { namespace bird_trigger {

PODGE_COMPONENT(component) {
    BOOST_HANA_DEFINE_STRUCT(component,
        (std::vector<entity *>, birds));
};
PODGE_REGISTER_COMPONENT(component);

struct system : entity_system {
    const char *name() const {
        return "bird_trigger";
    }

    std::vector<std::type_index> components() const {
        return {
            typeid(public_component),
            typeid(component)
        };
    }

    void add(entity &e) const {
        auto &lvl(level::current());
        auto &pc(e.component<public_component>());
        auto &c(e.component<component>());
        std::vector<std::string> bird_names;
        util::parse_names(pc.birds, bird_names);
        for(const auto &name : bird_names) {
            auto &bird(*lvl.entity_with_name(name));
            c.birds.emplace_back(&bird);
            e.bind(bird);
        }
    }
};
PODGE_REGISTER_SYSTEM(system);

struct contact_handler : entity_contact_handler {
    entity_system_mask mask() const {
        return {
            {"bird_trigger", "podge"}
        };
    }

    void begin_contact(entity_contact &contact) const {
        entity_contact_handler::begin_contact(contact);
        auto &lvl(level::current());
        auto &e(contact.entity_a());
        auto &c(e.component<component>());
        for(auto bird : c.birds) {
            auto &ic(bird->component<bird::internal_component>());
            ic.active = true;
        }
        e.remove();
    }
};
PODGE_REGISTER_CONTACT_HANDLER(contact_handler);

} } }

#endif

#endif

