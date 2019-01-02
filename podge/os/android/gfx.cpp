#define NANOVG_GLES2_IMPLEMENTATION

#include "../../common.hpp"

#include <os/gfx.hpp>
#include <stb_image.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <nanovg_gl.h>

namespace podge {

struct android_gfx_context : gfx_context {
	android_gfx_context();
	~android_gfx_context();

	void set_current();
	SDL_Window *get_window() const;
	NVGcontext *create_nvg_context();
	void delete_nvg_context(NVGcontext *vg);
	void begin_frame(NVGcontext *vg, const glm::vec4 &bg_color);
	void end_frame(NVGcontext *vg);

private:
	SDL_Window *window;
	SDL_GLContext gl_ctx;
};

android_gfx_context::android_gfx_context() :
	window(nullptr),
	gl_ctx(nullptr)
{
	try {
		window = SDL_CreateWindow("Podge", 0, 0, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
		if(window == nullptr) {
			PODGE_THROW_SDL_ERROR();
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

		gl_ctx = SDL_GL_CreateContext(window);
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
		if(window != nullptr) {
			SDL_DestroyWindow(window);
		}
		throw;
	}
} 

android_gfx_context::~android_gfx_context() {
	SDL_GL_DeleteContext(gl_ctx);
	SDL_DestroyWindow(window);
}

void android_gfx_context::set_current() {
	if(SDL_GL_MakeCurrent(window, gl_ctx) < 0) {
		PODGE_THROW_SDL_ERROR();
	}
}

SDL_Window *android_gfx_context::get_window() const {
	return window;
}

NVGcontext *android_gfx_context::create_nvg_context() {
	auto flags(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#if defined(DEBUG)
	flags |= NVG_DEBUG;
#endif
	return nvgCreateGLES2(flags);
}

void android_gfx_context::delete_nvg_context(NVGcontext *vg) {
	nvgDeleteGLES2(vg);
}

void android_gfx_context::begin_frame(NVGcontext *vg, const glm::vec4 &bg_color) {
	int fbWidth, fbHeight;
	int winWidth, winHeight;
	SDL_GL_GetDrawableSize(window, &fbWidth, &fbHeight);
	SDL_GetWindowSize(window, &winWidth, &winHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	auto pxRatio(float(fbWidth)/winWidth);
	nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
}

void android_gfx_context::end_frame(NVGcontext *vg) { 
	nvgEndFrame(vg);
	SDL_GL_SwapWindow(window);
}

std::unique_ptr<gfx_context> create_gfx_context() {
	return std::unique_ptr<gfx_context>(new android_gfx_context());
}

}
