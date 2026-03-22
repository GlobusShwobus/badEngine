#include "pch.h"
#include "EngineUtils.h"
#include <assert.h>

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