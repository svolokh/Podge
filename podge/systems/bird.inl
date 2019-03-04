#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_BIRD_HPP_FWD
#define PODGE_SYSTEM_BIRD_HPP_FWD

namespace podge { namespace systems { namespace bird {

PODGE_PUBLIC_COMPONENT(component) {
    component() :
        time(5.0f),
        delay(0.0f)
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
        (float, time), // desired time to reach the end of the path (may end up being slightly more due to how bird turning works)
        (float, delay));  // time to delay before taking flight (useful if you want to have multiple birds fly along a path)
};
PODGE_REGISTER_COMPONENT(component);

PODGE_COMPONENT(internal_component) {
    internal_component() :
        active(false)
    {
    }

    BOOST_HANA_DEFINE_STRUCT(internal_component,
        (bool, active));
};
PODGE_REGISTER_COMPONENT(internal_component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_BIRD_HPP
#define PODGE_SYSTEM_BIRD_HPP

namespace podge { namespace systems { namespace bird {

PODGE_COMPONENT(private_component) {
    private_component() :
        start_timer(0.0f)
    {
    }

    BOOST_HANA_DEFINE_STRUCT(private_component,
        (float, start_timer),
        (std::size_t, index),
        (float, speed),
        (std::vector<glm::vec2>, path));
};
PODGE_REGISTER_COMPONENT(private_component);

struct system : entity_system {
    const char *name() const {
        return "bird";
    }

    std::vector<std::type_index> components() const {
        return {
            typeid(component),
            typeid(private_component),
            typeid(internal_component)
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
        assert(std::strcmp(path.type().name(), "bird_path") == 0);
        auto shp(path.body()->GetFixtureList()->GetShape());
        if(shp->GetType() != b2Shape::e_chain) {
            PODGE_THROW_ERROR();
        }
        auto chain(static_cast<const b2ChainShape *>(shp));
        const auto &xform(path.body()->GetTransform());
        for(auto i(0); i != chain->m_count; ++i) {
            auto pos(b2Mul(xform, chain->m_vertices[i]));
            pc.path.emplace_back(pos.x, pos.y);
        }
        assert(pc.path.size() > 1);
        e.body()->SetTransform(to_b2Vec2(pc.path[0]), e.body()->GetAngle());
        pc.index = 1;
        auto dist(0.0f);
        for(std::size_t i(0); i != pc.path.size() - 1; ++i) {
            const auto &v1(pc.path[i]);
            const auto &v2(pc.path[i + 1]);
            dist += glm::distance(v1, v2);
        }
        pc.speed = dist/c.time;
    }

    void step(entity &e) const {
        auto &lvl(level::current());
        auto &c(e.component<component>());
        auto &cc(e.component<core_component>());
        auto &pc(e.component<private_component>());
        auto &ic(e.component<internal_component>());
        if(!ic.active) {
            auto pos(e.body()->GetPosition());
            return;
        } else if(pc.start_timer < c.delay) {
            pc.start_timer += lvl.dt();
            return;
        }
        const auto angle_per_sec(glm::radians(120.0f));
        auto angle(e.body()->GetAngle());
        auto pos(to_vec2(e.body()->GetPosition()));
        if(pc.index >= pc.path.size()) {
            e.remove();
            return;
        } 
        auto max_angle_delta(lvl.dt()*angle_per_sec);
        const auto &target_pos(pc.path[pc.index]); 
        float target_angle;
        if(cc.flip_horizontal) {
            target_angle = std::atan2(-(target_pos.y - pos.y), -(target_pos.x - pos.x));
        } else {
            target_angle = std::atan2(target_pos.y - pos.y, target_pos.x - pos.x);
        }
        auto angle_delta(target_angle - angle);
        if(std::abs(angle_delta) >= max_angle_delta) {
            angle_delta = std::copysign(max_angle_delta, angle_delta);
        }
        angle += angle_delta;
        glm::vec2 v(std::cos(angle), std::sin(angle));
        if(cc.flip_horizontal) {
            v.x *= -1.0f;
            v.y *= -1.0f;
        }
        pos += pc.speed*v*lvl.dt();
        e.body()->SetTransform(to_b2Vec2(pos), angle);
        {
            // check if we need to increment the index
            auto v1(pc.path[pc.index - 1]); 
            auto v2(pc.path[pc.index]);
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

