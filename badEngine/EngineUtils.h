#pragma once

#include <SDL3/SDL_video.h>
#include <string>
#include "Float2.h"
#include "Rect.h"
#include "Matrix3.h"

namespace badEngine
{
	badCore::Mat3 sdl_window_matrix(SDL_Window* const window) noexcept;

	constexpr SDL_FRect rect_to_sdl_frect(const badCore::Rect& rect)noexcept
	{
		return {
			rect.min.x,
			rect.min.y,
			rect.get_width(),
			rect.get_height()
		};
	}

	constexpr SDL_FPoint logical_to_visual_point(const badCore::float2& point, const badCore::Mat3& transformer) noexcept
	{
		return
		{
		transformer.a * point.x + transformer.b * point.y + transformer.tx,
		transformer.c * point.x + transformer.d * point.y + transformer.ty
		};
	}

	bool json_key_to_sdl_flags(const std::string& key, std::size_t& flags)noexcept;
}