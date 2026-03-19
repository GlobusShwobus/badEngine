#include "pch.h"
#include "EngineUtils.h"
#include <assert.h>

bad::Mat3 bad::sdl_window_matrix(SDL_Window* const window) noexcept
{
	assert(window != nullptr);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return Mat3::translation({ w * 0.5f, h * 0.5f }) * Mat3::scale(1.0f, -1.0f);
}

bool bad::json_key_to_sdl_flags(const std::string& key, std::size_t& flags)noexcept
{
	bool good_flag = false;

	if (key == "SDL_WINDOW_OPENGL") {
		flags |= SDL_WINDOW_OPENGL;
		good_flag = true;
	}
	if (key == "SDL_WINDOW_RESIZABLE") {
		flags |= SDL_WINDOW_RESIZABLE;
		good_flag = true;
	}
	//other flags here

	return good_flag;
}