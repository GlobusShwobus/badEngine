#include "pch.h"
#include "Canvas.h"

namespace badWindow
{
	Canvas::Canvas(Uint32 width, Uint32 height, SDL_Renderer* const renderer)
	{
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
		assert(texture != nullptr);

		//set blend mode to blend to read alpha channels as well
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		mTexture.reset(texture);
	}

	bool Canvas::start_drawing(SDL_Renderer* const renderer)const noexcept
	{
		return SDL_SetRenderTarget(renderer, mTexture.get());
	}

	bool Canvas::end_drawing(SDL_Renderer* const renderer)const noexcept
	{
		return SDL_SetRenderTarget(renderer, nullptr);
	}

	SDL_Texture* const Canvas::get()const noexcept {
		return mTexture.get();
	}
}