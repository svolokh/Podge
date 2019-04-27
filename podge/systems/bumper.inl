#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_BUMPER_HPP_FWD
#define PODGE_SYSTEM_BUMPER_HPP_FWD

namespace podge { namespace systems { namespace bumper {

PODGE_PUBLIC_COMPONENT(public_component) {
    public_component() :
        bump_force(0.0f)
    {
    }

    BOOST_HANA_DEFINE_STRUCT(public_component,
            (float, bump_force));
};
PODGE_REGISTER_COMPONENT(public_component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_BUMPER_HPP
#define PODGE_SYSTEM_BUMPER_HPP

namespace podge { namespace systems { namespace bumper {

struct system : entity_system {
    const char *name() const {
        return "bumper";
    }

    std::vector<std::type_index> components() const {
        return {
            typeid(public_component)
        };
    }

    void step(entity &e) const {
        auto &pc(e.component<public_component>());
        for(auto c(e.body()->GetContactList()); c != nullptr; c = c->next) {
            if(c->contact->IsTouching()) {
                entity_contact ec(c->contact, false);
                auto f(to_b2Vec2(pc.bump_force*to_vec2(ec.normal())));
                ec.entity_b().body()->ApplyForce(f, ec.point(), true);
            }
        }
    }
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

