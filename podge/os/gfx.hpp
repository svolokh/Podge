#if !defined(OS_GFX_HPP_)
#define OS_GFX_HPP_

#include "../common.hpp"

#include <memory>

#include <SDL.h>
#include <nanovg.h>
#include <glm/glm.hpp>

namespace podge {

struct gfx_context {
	virtual ~gfx_context() = default;

	virtual void set_current() = 0;
	virtual SDL_Window *get_window() const = 0;
	virtual NVGcontext *create_nvg_context() = 0;
	virtual void delete_nvg_context(NVGcontext *vg) = 0;
	virtual void begin_frame(NVGcontext *vg, const glm::vec4 &bg_color) = 0;
	virtual void end_frame(NVGcontext *vg) = 0;
};

std::unique_ptr<gfx_context> create_gfx_context();

}

#endif
