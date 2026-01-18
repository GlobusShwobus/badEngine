#pragma once
#include <cstddef>
#include <SDL3/SDL_video.h>
#include <string>
namespace badEngine {

	template <typename T, void(*func)(T*)>
	struct SDLDeleter {
		void operator()(T* ptr)const noexcept {
			if (ptr)
				func(ptr);
		}
	};

	bool MapSDL_Flags_to_size_t_bitwise(const std::string& key, std::size_t& flags)noexcept;
}