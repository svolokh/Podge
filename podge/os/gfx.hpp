#if !defined(OS_GFX_HPP_)
#define OS_GFX_HPP_

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT

#include "../common.hpp"

#include <memory>

#include <SDL.h>
#include <nanovg.h>
#include <nuklear.h>
#include <glm/glm.hpp>

namespace podge {

struct nvg_context_deleter {
	void operator()(NVGcontext *ctx) const;
};

struct nk_context_deleter {
	void operator()(struct nk_context *ctx) const;
};

typedef std::unique_ptr<NVGcontext, nvg_context_deleter> nvg_context_ptr;
typedef std::unique_ptr<struct nk_context, nk_context_deleter> nk_context_ptr;

struct gfx_context {
	virtual ~gfx_context() = default;

	virtual void set_current() = 0;
	virtual SDL_Window *window() = 0;
	virtual nvg_context_ptr new_nvg_context() = 0;
	virtual void nvg_begin(NVGcontext *vg, const glm::vec4 &bg_color) = 0;
	virtual void nvg_end(NVGcontext *vg) = 0;
	virtual nk_context_ptr new_nk_context() = 0;
	virtual void nk_begin(struct nk_context *ctx) = 0;
	virtual void nk_handle_event(struct nk_context *ctx, SDL_Event *evt) = 0;
	virtual void nk_end(struct nk_context *ctx, const glm::vec4 &bg_color) = 0;
};

std::unique_ptr<gfx_context> create_gfx_context();

}

#endif
