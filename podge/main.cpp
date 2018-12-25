#include "podge.hpp"
#include "os/gfx.hpp"
#include "os/resources.hpp"

#include <SDL.h>
#include <boost/scope_exit.hpp>

namespace podge {

static void run() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        THROW_SDL_ERROR();
    }
    BOOST_SCOPE_EXIT(void) {
        SDL_Quit();
    } BOOST_SCOPE_EXIT_END

    auto gctx(create_gfx_context());
    gctx->set_current();

    auto window(gctx->get_window());

    resource_path tmx_path("levels/demo.tmx");
    pugi::xml_document tmx;
	auto res(tmx.load_string(get_resource(tmx_path.str()).c_str()));
	if(!res) {
		THROW_ERROR();
	}

    for(;;) {
        auto vg(gctx->create_nvg_context());
        BOOST_SCOPE_EXIT(&gctx, vg) {
            gctx->delete_nvg_context(vg);
        } BOOST_SCOPE_EXIT_END

        level lvl(vg, tmx, 1.0f/60.0f, tmx_path.parent());
        auto update_camera_dims([&lvl, &window]() {
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            float cam_width, cam_height;
            if(w < h) {
                // portrait
                cam_width = lvl.camera_height()*w/h;
                cam_height = lvl.camera_height();
            } else {
                // landscape
                cam_width = lvl.camera_width();
                cam_height = lvl.camera_width()*h/w;
            }
            lvl.camera_width(cam_width);
            lvl.camera_height(cam_height);
        });
        update_camera_dims();

        auto finger_event_to_input([&](SDL_TouchFingerEvent &evt) -> input {
            input in;
            in.finger_id = evt.fingerId;
            in.x = lvl.camera_position().x + lvl.camera_width() * (evt.x - 0.5f);
            in.y = lvl.camera_position().y + lvl.camera_height() * -(evt.y - 0.5f);
            switch(evt.type) {
                case SDL_FINGERDOWN:
                    in.type = input::DOWN;
                    break;
                case SDL_FINGERMOTION:
                    in.type = input::DRAG;
                    in.dx = lvl.camera_width() * evt.dx;
                    in.dy = -lvl.camera_height() * evt.dy;
                    break;
                case SDL_FINGERUP:
                    in.type = input::UP;
                    break;
            }
            return in;
        });

        auto accum(0.0f);
        auto last_frame_time(clock::now());
        SDL_Event event;
        while(!lvl.exit_state()) {
            while(SDL_PollEvent(&event)) {
                switch(event.type) {
                    case SDL_WINDOWEVENT:
                        update_camera_dims();
                        break;
                    case SDL_FINGERDOWN:
                    case SDL_FINGERMOTION:
                    case SDL_FINGERUP:
                        lvl.queue_input(finger_event_to_input(event.tfinger));
                        break;
                }
            }
            auto first_frame(true);
            auto now(clock::now());
            accum += std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(now - last_frame_time).count();
            accum = std::min(accum, 0.5f); // avoid the "spiral of death", only allow the game to run at a certain slowness
            last_frame_time = now;
            while(accum >= lvl.dt()) {
                lvl.step();
                accum -= lvl.dt();
            }
            // force a step on the first frame to enforce the invariant that at least one step will always have taken place before a render
            if(first_frame) { 
                lvl.step();
                accum -= lvl.dt();
                first_frame = false;
            }
            int winWidth, winHeight;
            SDL_GetWindowSize(window, &winWidth, &winHeight);
            auto campos(lvl.camera_position());
            gctx->begin_frame(vg, lvl.bg_color());
            nvgTranslate(vg, winWidth/2.0f, winHeight/2.0f);
            nvgScale(vg, winWidth/lvl.camera_width(), -winHeight/lvl.camera_height());
            nvgTranslate(vg, -campos.x, -campos.y);
            lvl.render();
            gctx->end_frame(vg);
        }

        const char *msg;
        if(lvl.exit_state()->type() == typeid(level_exits::victory)) {
            msg = "Victory!";
        } else if(lvl.exit_state()->type() == typeid(level_exits::failure)) {
            msg = "Game Over!";
        } else {
            THROW_ERROR();
        }
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Podge", msg, window);
    }
}

}

extern "C" {

int main(int argc, char **argv) {
    try {
        podge::run();
    } catch(const std::exception &e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s", e.what());
        return 1;
    }
	return 0;
}

}
