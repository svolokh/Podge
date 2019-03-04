#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_CAMERA_ZONE_HPP_FWD
#define PODGE_SYSTEM_CAMERA_ZONE_HPP_FWD

namespace podge { namespace systems { namespace camera_zone {

PODGE_PUBLIC_COMPONENT(component) {
    component() :
        center(false),
        center_width(7.0f),
        fixed(false),
        tracking(false)
    {
    }

    void validate(const context &ctx) const {
        auto fail([]() {
            throw validation_error("only one of center, fixed, or tracking can be specified");
        });
        auto any(false);
        if(center) {
            any = true;
        }
        if(fixed) {
            if(any) {
                fail();
            } else {
                any = true;
            }
        } 
        if(tracking) {
            if(any) {
                fail();
            } else {
                any = true;
            }
        }
        if(!any) {
            throw validation_error("at least one of center, fixed, or tracking must be specified");
        }
    }

    BOOST_HANA_DEFINE_STRUCT(component,
        (bool, center),
        (float, center_width),
        (bool, fixed),
        (bool, tracking));
};
PODGE_REGISTER_COMPONENT(component);

} } }

#endif

#else

#ifndef PODGE_SYSTEM_CAMERA_ZONE_HPP
#define PODGE_SYSTEM_CAMERA_ZONE_HPP

namespace podge { namespace systems { namespace camera_zone {

PODGE_COMPONENT(private_component) {
    private_component() :
        podge(nullptr),
        camera(nullptr)
    {
    }

    BOOST_HANA_DEFINE_STRUCT(private_component,
        (entity *, podge),
        (entity *, camera),
        (b2AABB, zone_aabb));
};
PODGE_REGISTER_COMPONENT(private_component);

// added to podge
PODGE_COMPONENT(podge_component) {
    BOOST_HANA_DEFINE_STRUCT(podge_component,
        (boost::optional<entity *>, inside_zone),
        (boost::optional<entity *>, entering_zone));
};
PODGE_REGISTER_COMPONENT(podge_component);

struct system : entity_system {
    const char *name() const {
        return "camera_zone";
    }

    std::vector<std::type_index> components() const {
        return {
            typeid(component),
            typeid(private_component)
        };
    }

    void add(entity &e) const {
        auto &pc(e.component<private_component>());
        auto &lvl(level::current());
        {
            std::vector<entity *> podges;
            lvl.entities_with_system("podge", podges);
            if(podges.size() != 1) {
                PODGE_THROW_ERROR();
            }
            auto &p(*podges[0]);
            e.bind(p);
            pc.podge = &p;
            if(!p.has_component<podge_component>()) {
                p.add_component<podge_component>();
            }
        }
        {
            std::vector<entity *> cameras;
            lvl.entities_with_system("camera", cameras);
            if(cameras.size() != 1) {
                PODGE_THROW_ERROR();
            }
            auto &cam(*cameras[0]);
            e.bind(cam);
            pc.camera = &cam;
        }
        pc.zone_aabb = util::compute_body_bounds(e.body());
    }

private:
    // Calculates the width of the camera zone shape at a given position.
    // In the rare instance that the ray trace fails, returns nothing.
    boost::optional<float> width_at_pos(entity &e, const glm::vec2 &pos) const {
        auto &cc(e.component<core_component>());
        b2RayCastInput in;
        b2RayCastOutput out;
        auto r_p1(pos + glm::vec2(cc.width + 1.0f, 0.0f));
        auto r_p2(pos);
        in.p1 = to_b2Vec2(r_p1);
        in.p2 = to_b2Vec2(r_p2);
        in.maxFraction = 1.0f;
        auto shp(e.body()->GetFixtureList()->GetShape());
        if(shp->RayCast(&out, in, e.body()->GetTransform(), 0)) {
            auto r(r_p1 + out.fraction*(r_p2 - r_p1));
            auto l_p1(pos - glm::vec2(cc.width + 1.0f, 0.0f));
            auto l_p2(pos);
            in.p1 = to_b2Vec2(l_p1);
            in.p2 = to_b2Vec2(l_p2);
            if(shp->RayCast(&out, in, e.body()->GetTransform(), 0)) {
                auto l(l_p1 + out.fraction*(l_p2 - l_p1));
                return r.x - l.x;
            } 
        } 
        return boost::none;
    }

public:
    void step(entity &e) const {
        auto &pc(e.component<private_component>());
        auto &c(e.component<component>());
        auto col_podge(false);
        for(auto c(e.body()->GetContactList()); c != nullptr; c = c->next) {
            auto &o(util::entity_from_body(c->other));
            if(&o == pc.podge) {
                col_podge = true;
                break;
            }
        }
        auto &pgc(pc.podge->component<podge_component>());
        if(col_podge) {
            // if Podge is currently still partly in his previous camera zone, don't change yet
            auto active(false);
            if(pgc.inside_zone) {
                if(*pgc.inside_zone == &e) {
                    active = true;
                } else if(pgc.entering_zone) {
                    if(*pgc.entering_zone == &e) {
                        active = true;
                    }
                } else if(!pgc.entering_zone) {
                    pgc.entering_zone = &e;
                    active = true;
                }
            } else {
                if(pgc.entering_zone) {
                    if(*pgc.entering_zone == &e) {
                        pgc.inside_zone = *pgc.entering_zone;
                        pgc.entering_zone = boost::none;
                        active = true;
                    }
                } else {
                    pgc.inside_zone = &e;
                    active = true;
                }
            }
            if(active) {
                auto fail(false); // in rare cases, the camera position calculation may fail, when this happens simply avoid updating the camera for this frame
                glm::vec2 next_pos;
                float next_width;
                if(c.center) {
                    // camera centers on Podge, screen width is fixed
                    next_pos = to_vec2(pc.podge->body()->GetPosition());
                    next_width = c.center_width;
                } else if(c.fixed) {
                    // camera is fixed on the center of the camera_zone shape, screen width is the width of the shape
                    next_pos = to_vec2(e.body()->GetPosition());
                    next_width = pc.zone_aabb.upperBound.x - pc.zone_aabb.lowerBound.x;
                } else if(c.tracking) {
                    // camera tracks podge vertically, remains fixed horizontally (screen width is the width of the shape)
                    auto e_pos(e.body()->GetPosition());
                    auto p_pos(pc.podge->body()->GetPosition());
                    auto w(width_at_pos(e, to_vec2(p_pos)));
                    if(!w) {
                        fail = true;
                    } else {
                        next_pos = glm::vec2(e_pos.x, p_pos.y);
                        next_width = *w;
                    }
                } else {
                    PODGE_THROW_ERROR();
                }
                if(!fail) {
                    auto &cc(pc.camera->component<camera::component>());
                    cc.target_pos = next_pos;
                    cc.target_width = next_width;
                }
            }
        } else {
            if(pgc.inside_zone && *pgc.inside_zone == &e) {
                pgc.inside_zone = boost::none;
            }
            if(pgc.entering_zone && *pgc.entering_zone == &e) {
                pgc.entering_zone = boost::none;
            }
        }
    }
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

