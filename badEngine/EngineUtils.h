#pragma once

#include <SDL3/SDL_video.h>
#include <string>
#include "Float2.h"
#include "Rect.h"
#include "Mat3.h"

namespace bad
{
	constexpr SDL_FPoint transform_point_to_sdl_fpoint(const bad::Point& point, const bad::Mat3& t) noexcept
	{
		return
		{
		t.m00 * point.x + t.m01 * point.y + t.m02,
		t.m10 * point.x + t.m11 * point.y + t.m12
		};
	}

	bool json_key_to_sdl_flags(const std::string& key, std::size_t& flags)noexcept;
}