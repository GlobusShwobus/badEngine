#include "pch.h"
#include "Canvas.h"

namespace badWindow
{
	Canvas make_canvas(Uint32 width, Uint32 height, SDL_Renderer* const renderer) {
		if (!renderer)
			return;

		//create texture
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