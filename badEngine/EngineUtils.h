#pragma once

#include <SDL3/SDL_video.h>
#include <string>
#include "Float2.h"
#include "Rect.h"
#include "Mat3.h"

namespace bad
{
	bad::Mat3 sdl_window_matrix(SDL_Window* const window) noexcept;

	bool json_key_to_sdl_flags(const std::string& key, std::size_t& flags)noexcept;
}