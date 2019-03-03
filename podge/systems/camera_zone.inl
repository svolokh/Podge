#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_CAMERA_ZONE_HPP_FWD
#define PODGE_SYSTEM_CAMERA_ZONE_HPP_FWD

namespace podge { namespace systems { namespace camera_zone {

PODGE_PUBLIC_COMPONENT(component) {
    component() :
        center(false),
        center_width(5.0f),
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
        (boost::optional<entity *>, current_camera_zone));
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
        if(col_podge) {
            auto &pgc(pc.podge->component<podge_component>());
            // if Podge is currently still partly in his previous camera zone, don't change yet
            auto still_in_prev(false);
            if(pgc.current_camera_zone && *pgc.current_camera_zone != &e) {
                for(auto c(pc.podge->body()->GetContactList()); c != nullptr; c = c->next) {
                    auto &o(util::entity_from_body(c->other));
                    if(&o == *pgc.current_camera_zone) {
                        still_in_prev = true;
                        break;
                    }
                }
            }
            if(!still_in_prev) {
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
                    next_pos = glm::vec2(e_pos.x, p_pos.y);
                    next_width = pc.zone_aabb.upperBound.x - pc.zone_aabb.lowerBound.x;
                } else {
                    PODGE_THROW_ERROR();
                }
                auto &cc(pc.camera->component<camera::component>());
                cc.target_pos = next_pos;
                cc.target_width = next_width;
                pgc.current_camera_zone = &e;
            }
        }
    }
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif

