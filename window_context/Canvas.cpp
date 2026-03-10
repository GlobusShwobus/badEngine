#include "pch.h"
#include "Canvas.h"

namespace badWindow
{
	Canvas make_canvas(SDL_Renderer* const renderer, Uint32 width, Uint32 height) noexcept
	{
		//NOTE: don't check renderer existance. if it fails, let SDL_CreateTexture fail for a SDL_GetError
		SDL_Texture* texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
		);

		if (!texture) {
			return Canvas{ nullptr };
		}

		//set blend mode to blend to read alpha channels as well
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		return Canvas{ texture };
	}
}