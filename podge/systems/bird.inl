#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_BIRD_HPP_FWD
#define PODGE_SYSTEM_BIRD_HPP_FWD

namespace podge { namespace systems { namespace bird {

PODGE_PUBLIC_COMPONENT(component) {
    component() :
        time(5.0f)
    {
    }

    void validate(const context &ctx) const {
        if(ctx.is_map()) {
            if(path.empty()) {
                throw validation_error("must specify path");
            }
            if(!ctx.entity_exists(path)) {
                std::ostringstream oss;
                oss << "path '" << path << "' does not exist";
                throw validation_error(oss.str());
            }
        }
    }

    BOOST_HANA_DEFINE_STRUCT(component,
        (std::string, path),
        (float, time)); // desired time to reach the end of the path (may end up being slightly more due to how bird turning works)
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_BIRD_HPP
#define PODGE_SYSTEM_BIRD_HPP

namespace podge { namespace systems { namespace bird {

PODGE_COMPONENT(private_component) {
    BOOST_HANA_DEFINE_STRUCT(private_component,
        (std::size_t, index),
        (float, speed),
        (const b2ChainShape *, path));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
    const char *name() const {
        return "bird";
    }

    std::vector<std::type_index> components() const {
        return {
            typeid(component),
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
        auto &path(*lvl.entity_with_name(c.path));
        auto shp(path.body()->GetFixtureList()->GetShape());
        if(shp->GetType() != b2Shape::e_chain) {
            PODGE_THROW_ERROR();
        }
        pc.path = static_cast<const b2ChainShape *>(shp);
        assert(pc.path->m_count > 1);
        e.body()->SetTransform(pc.path->m_vertices[0], e.body()->GetAngle());
        pc.index = 1;
        auto dist(0.0f);
        for(std::size_t i(0); i != pc.path->m_count - 1; ++i) {
            const auto &v1(pc.path->m_vertices[i]);
            const auto &v2(pc.path->m_vertices[i + 1]);
            dist += glm::distance(to_vec2(v1), to_vec2(v2));
        }
        pc.speed = dist/c.time;
    }

    void step(entity &e) const {
        auto &lvl(level::current());
        auto &pc(e.component<private_component>());
        const auto angle_per_sec(glm::radians(90.0f));
        auto angle(e.body()->GetAngle());
        auto pos(to_vec2(e.body()->GetPosition()));
        if(pc.index >= pc.path->m_count) {
            e.remove();
            return;
        } 
        auto max_angle_delta(lvl.dt()*angle_per_sec);
        const auto &target_pos(to_vec2(pc.path->m_vertices[pc.index]));
        auto target_angle(std::atan2(target_pos.y - pos.y, target_pos.x - pos.x));
        auto angle_delta(target_angle - angle);
        if(std::abs(angle_delta) >= max_angle_delta) {
            angle_delta = std::copysign(max_angle_delta, angle_delta);
        }
        angle += angle_delta;
        glm::vec2 v(std::cos(angle), std::sin(angle));
        pos += pc.speed*v;
        e.body()->SetTransform(to_b2Vec2(pos), e.body()->GetAngle());
        {
            // check if we need to increment the index
            auto v1(to_vec2(pc.path->m_vertices[pc.index - 1]));
            auto v2(to_vec2(pc.path->m_vertices[pc.index]));
            auto v(v2 - v1);
            auto x(pos - v1);
            auto l(glm::length(v));
            if(glm::dot(x, v/l) >= l) {
                ++pc.index;
            }
        }
    }
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

