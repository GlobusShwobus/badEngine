#pragma once

#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	struct CanvasDeleter {
		void operator()(SDL_Texture* c) {
			if (c)
				SDL_DestroyTexture(c);
		}
	};

	using Canvas = std::unique_ptr<SDL_Texture, CanvasDeleter>;
	
	Canvas make_canvas(Uint32 width, Uint32 height, SDL_Renderer* const renderer);
}
