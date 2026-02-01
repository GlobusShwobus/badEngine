#pragma once
#include <cstddef>
#include <string>
#include "SDL3/SDL_render.h"
#include <SDL3/SDL_rect.h>

#include <bit>
#include "vector.h"
#include "AABB.h"
#include "Matrix3.h"

namespace badWindow
{
	template <typename T, void(*func)(T*)>
	struct SDLDeleter {
		void operator()(T* ptr)const noexcept {
			if (ptr)
				func(ptr);
		}
	};

	bool MapSDL_Flags_to_size_t_bitwise(const std::string& key, std::size_t& flags)noexcept;

	constexpr SDL_FRect sdl_cast(const badCore::AABB& aabb)noexcept
	{
		return std::bit_cast<SDL_FRect>(aabb);
	}

	constexpr SDL_FPoint sdl_cast(const badCore::float2& point)noexcept
	{
		return std::bit_cast<SDL_FPoint>(point);
	}

	constexpr SDL_FPoint sdl_transform(const badCore::float2& point, const badCore::Mat3& transform)noexcept
	{
		return
		{
		transform.a * point.x + transform.b * point.y + transform.tx,
		transform.c * point.x + transform.d * point.y + transform.ty
		};
	}

}