#pragma once

#include <SDL3/SDL_video.h>
#include <string>
#include "Float2.h"
#include "Rect.h"
#include "Mat3.h"

namespace bad
{
	constexpr SDL_FPoint transform_point_to_sdl_fpoint(const bad::Point& point, const bad::Mat3& transformer) noexcept
	{
		return
		{
		transformer.a * point.x + transformer.b * point.y + transformer.tx,
		transformer.c * point.x + transformer.d * point.y + transformer.ty
		};
	}

	bool json_key_to_sdl_flags(const std::string& key, std::size_t& flags)noexcept;
}