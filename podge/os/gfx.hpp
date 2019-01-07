#if !defined(OS_GFX_HPP_)
#define OS_GFX_HPP_

#include "../common.hpp"

#include <memory>

#include <SDL.h>
#include <nanovg.h>
#include <nuklear.h>
#include <glm/glm.hpp>

namespace podge {

struct gfx_context {
	virtual ~gfx_context() = default;

	virtual void set_current() = 0;
	virtual SDL_Window *window() = 0;
	virtual NVGcontext *nvg_context() = 0;
	virtual void nvg_begin(const glm::vec4 &bg_color) = 0;
	virtual void nvg_end() = 0;
	virtual struct nk_context *nk_context() = 0;
	virtual void nk_begin() = 0;
	virtual void nk_handle_event(SDL_Event *evt) = 0;
	virtual void nk_end(const glm::vec4 &bg_color) = 0;
};

std::unique_ptr<gfx_context> create_gfx_context();

}

#endif
