#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_image/SDL_image.h"

namespace badEngine {

	template <typename T, void(*func)(T*)>
	struct SDLDeleter {
		void operator()(T* ptr)const noexcept {
			if (ptr)
				func(ptr);
		}
	};
}