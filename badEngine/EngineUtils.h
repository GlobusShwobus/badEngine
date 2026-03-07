#pragma once

#include <SDL3/SDL_video.h>
#include "Matrix3.h"

namespace badEngine
{
	badCore::Mat3 sdl_window_matrix(SDL_Window* const window) noexcept;

	constexpr SDL_FPoint find_screen_relative(const badCore::float2& point, const badCore::Mat3& transformer) noexcept
	{
		return
		{
		transformer.a * point.x + transformer.b * point.y + transformer.tx,
		transformer.c * point.x + transformer.d * point.y + transformer.ty
		};
	}
}