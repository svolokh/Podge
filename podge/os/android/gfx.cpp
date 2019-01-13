#define NANOVG_GLES2_IMPLEMENTATION
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_IMPLEMENTATION
#define NK_SDL_GLES2_IMPLEMENTATION

#include "../../common.hpp"

#include <os/gfx.hpp>
#include <os/resources.hpp>
#include <stb_image.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <nanovg_gl.h>
#include <map>
#include <SDL.h>

// Hack macro to allow Podge and its dependencies to assume 96 DPI (primarily for the sake of nuklear)
#define SDL_GL_GetDrawableSize(WINDOW, W, H) Podge_SDL_GL_GetDrawableSize(WINDOW, W, H)

static std::map<SDL_Window *, std::pair<int, int>> podge_fb_sizes;

static void Podge_SDL_GL_GetDrawableSize(SDL_Window *window, int *w, int *h) {
	auto it(podge_fb_sizes.find(window));
	assert(it != podge_fb_sizes.end());
	auto dim(it->second);
	*w = dim.first;
	*h = dim.second;
}

#include <nuklear_sdl_gles2.h>

namespace podge {

static struct nk_context *current_nk_context(nullptr);

void nvg_context_deleter::operator()(NVGcontext *ctx) const {
	nvgDeleteGLES2(ctx);
}

void nk_context_deleter::operator()(struct nk_context *ctx) const {
	// nk_sdl_shutdown(); FIXME this destroys the entire SDL context
	current_nk_context = nullptr;
}

struct android_gfx_context : gfx_context {
	android_gfx_context();
	~android_gfx_context();

	void set_current();
	SDL_Window *window();
	nvg_context_ptr new_nvg_context();
	void nvg_begin(NVGcontext *ctx, const glm::vec4 &bg_color);
	void nvg_end(NVGcontext *ctx);
	nk_context_ptr new_nk_context();
	void nk_begin(struct nk_context *ctx);
	void nk_handle_event(struct nk_context *ctx, SDL_Event *evt);
	void nk_end(struct nk_context *ctx, const glm::vec4 &bg_color);

private:
	SDL_Window *sdl_window;
	SDL_GLContext gl_ctx;
};

android_gfx_context::android_gfx_context() :
	sdl_window(nullptr),
	gl_ctx(nullptr)
{
	try {
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			PODGE_THROW_SDL_ERROR();
		}

		sdl_window = SDL_CreateWindow("Podge", 0, 0, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
		if(sdl_window == nullptr) {
			PODGE_THROW_SDL_ERROR();
		}

		// set up high-DPI rendering
		{
			float hdpi;
			if(SDL_GetDisplayDPI(0, nullptr, &hdpi, nullptr) < 0) {
				PODGE_THROW_SDL_ERROR();
			}
			auto scale(96.0f/hdpi);
			int w, h;
			SDL_GetWindowSize(sdl_window, &w, &h);
			podge_fb_sizes.emplace(sdl_window, std::make_pair(w, h));
			w = int(w * scale);
			h = int(h * scale);
			SDL_SetWindowSize(sdl_window, w, h);
		}

		if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2) < 0) {
			PODGE_THROW_SDL_ERROR();
		}

		if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0) < 0) {
			PODGE_THROW_SDL_ERROR();
		}

		if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES) < 0) {
			PODGE_THROW_SDL_ERROR();
		}

		gl_ctx = SDL_GL_CreateContext(sdl_window);
		if(!gl_ctx) {
			PODGE_THROW_SDL_ERROR();
		}

		if(SDL_GL_SetSwapInterval(1) < 0) {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to enable v-sync");
		}
	} catch(...) {
		if(gl_ctx != nullptr) {
			SDL_GL_DeleteContext(gl_ctx);
		}
		if(sdl_window != nullptr) {
			SDL_DestroyWindow(sdl_window);
		}
		throw;
	}
} 

android_gfx_context::~android_gfx_context() {
	podge_fb_sizes.erase(sdl_window);
	SDL_GL_DeleteContext(gl_ctx);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

void android_gfx_context::set_current() {
	if(SDL_GL_MakeCurrent(sdl_window, gl_ctx) < 0) {
		PODGE_THROW_SDL_ERROR();
	}
}

SDL_Window *android_gfx_context::window() {
	return sdl_window;
}

nvg_context_ptr android_gfx_context::new_nvg_context() {
	auto flags(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#if defined(DEBUG)
	flags |= NVG_DEBUG;
#endif
	return nvg_context_ptr(nvgCreateGLES2(flags));
}

void android_gfx_context::nvg_begin(NVGcontext *ctx, const glm::vec4 &bg_color) {
	int fbWidth, fbHeight;
	int winWidth, winHeight;
	SDL_GL_GetDrawableSize(sdl_window, &fbWidth, &fbHeight);
	SDL_GetWindowSize(sdl_window, &winWidth, &winHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	auto pxRatio(float(fbWidth)/winWidth);
	nvgBeginFrame(ctx, winWidth, winHeight, pxRatio);
}

void android_gfx_context::nvg_end(NVGcontext *ctx) { 
	nvgEndFrame(ctx);
	SDL_GL_SwapWindow(sdl_window);
}

nk_context_ptr android_gfx_context::new_nk_context() {
	assert(current_nk_context == nullptr); // only one SDL nuklear context can be present at a time due to globals
	nk_context_ptr ptr(nk_sdl_init(sdl_window));
	auto ctx(ptr.get());
	current_nk_context = ctx;
	struct nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);
	auto roboto_ttf(get_resource("fonts/Roboto-Regular.ttf"));
	auto roboto(nk_font_atlas_add_from_memory(atlas, const_cast<char *>(roboto_ttf.data()), roboto_ttf.size(), 16, nullptr));
	nk_sdl_font_stash_end();
	nk_style_set_font(ctx, &roboto->handle);
	nk_style_hide_cursor(ctx);
	return ptr;
}

void android_gfx_context::nk_begin(struct nk_context *ctx) {
	assert(ctx == current_nk_context);
	ctx->delta_time_seconds = SDL_GetTicks()/1000.0f;

	// move the "mouse" off-screen at the start of each frame (there is no persistent cursor on a touch screen)
	ctx->input.mouse.pos = nk_vec2(-1.0f, -1.0f);
	ctx->input.mouse.prev = nk_vec2(-1.0f, -1.0f);
}

void android_gfx_context::nk_handle_event(struct nk_context *ctx, SDL_Event *evt) {
	assert(ctx == current_nk_context);
	nk_sdl_handle_event(evt);
}

void android_gfx_context::nk_end(struct nk_context *ctx, const glm::vec4 &bg_color) {
	assert(ctx == current_nk_context);
	glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
	SDL_GL_SwapWindow(sdl_window);
}

std::unique_ptr<gfx_context> create_gfx_context() {
	return std::unique_ptr<gfx_context>(new android_gfx_context());
}

}
