#define BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS 

#include "podge.hpp"
#include "os/gfx.hpp"
#include "os/resources.hpp"

#include <SDL_main.h>
#include <SDL.h>
#include <nuklear.h>
#include <boost/scope_exit.hpp>

namespace podge {

struct game {
    game();

    resource_path choose_level();
    level_exit play_level(const resource_path &tmx_path);

    std::unique_ptr<gfx_context> gctx;
};

game::game() :
    gctx(create_gfx_context())
{
}

resource_path game::choose_level() {
    auto pctx(gctx->new_nk_context());
    auto ctx(pctx.get());
    auto window(gctx->window());
    auto tmxs(list_resources("levels"));
    SDL_Event event;
    for(;;) {
        gctx->nk_begin(ctx);
        nk_input_begin(ctx);
        while(SDL_PollEvent(&event)) {
            gctx->nk_handle_event(ctx, &event);
        }
        nk_input_end(ctx);
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        if(nk_begin(ctx, "Level Select", nk_rect(16, 16, w - 32, h - 32), NK_WINDOW_TITLE | NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 30, 1);
            for(const auto &tmx : tmxs) {
                if(nk_button_label(ctx, tmx.c_str())) {
                    return resource_path("levels")/tmx;
                }
                nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);
            }
        }
        nk_end(ctx);
        gctx->nk_end(ctx, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    }
    PODGE_THROW_ERROR();
}

level_exit game::play_level(const resource_path &tmx_path) {
    pugi::xml_document tmx;
    auto res(tmx.load_string(get_resource(tmx_path.str()).c_str()));
    if(!res) {
        PODGE_THROW_ERROR();
    }

    auto pvg(gctx->new_nvg_context());
    auto vg(pvg.get());
    auto window(gctx->window());

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
        gctx->nvg_begin(vg, lvl.bg_color());
        nvgTranslate(vg, winWidth/2.0f, winHeight/2.0f);
        nvgScale(vg, winWidth/lvl.camera_width(), -winHeight/lvl.camera_height());
        nvgTranslate(vg, -campos.x, -campos.y);
        lvl.render();
        gctx->nvg_end(vg);
    }
    return *lvl.exit_state();
}

static void run() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        PODGE_THROW_SDL_ERROR();
    }
    BOOST_SCOPE_EXIT() {
        SDL_Quit();
    } BOOST_SCOPE_EXIT_END

    game g;
    auto window(g.gctx->window());
    for(;;) {
        auto tmx_path(g.choose_level());
        auto es(g.play_level(tmx_path));
        const char *msg;
        if(es.type() == typeid(level_exits::victory)) {
            msg = "Victory!";
        } else if(es.type() == typeid(level_exits::failure)) {
            msg = "Game Over!";
        } else {
            PODGE_THROW_ERROR();
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
