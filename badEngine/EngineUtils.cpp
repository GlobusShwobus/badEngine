#include "pch.h"
#include "EngineUtils.h"
#include <assert.h>
namespace badEngine
{
	badCore::Mat3 sdl_window_matrix(SDL_Window* const window) noexcept
	{
		assert(window != nullptr);
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return badCore::Mat3::translation({ w * 0.5f, h * 0.5f }) * badCore::Mat3::scale(1.0f, -1.0f);
	}
}
