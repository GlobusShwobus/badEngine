#pragma once
#include <cstddef>
#include <string>
#include "SDL3/SDL_render.h"
#include <SDL3/SDL_rect.h>

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


	struct RenderCommand
	{
		SDL_Texture* texture;
		SDL_FRect* source;
		SDL_FRect* dest;
	};
}