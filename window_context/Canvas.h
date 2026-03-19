#pragma once

#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	/// <summary> Functor deleter for SDL_Texture </summary>
	struct CanvasDeleter {
		void operator()(SDL_Texture* c) 
		{
			if (c)
				SDL_DestroyTexture(c);
		}
	};

	/// <summary> Wrapper for SDL_Texture using a unique_ptr with a custom functor deleter </summary>
	using Canvas = std::unique_ptr<SDL_Texture, CanvasDeleter>;
	
	/**
	* Create a blank texture for a rendering context.
	* For detailed documentation consult SDL_CreateTexture.
	* 
	* The texture created is targetable, meaning it can be drawn onto and it stores that drawing permanently.
	* 
	* The pixel format used is RGBA8888.
	* 
	* This function also sets the blend mode for the texture to also capture alpha channels.
	* If this is not desured it must be disabled manually.
	* 
	* \param renderer for rendering context
	* \param width of the created texture
	* \param height of the desired texture
	* \returns Canvas on success or nullptr on failure; call SDL_GetError() for more info
	* \throws noexcept
	*/
	inline Canvas make_canvas(SDL_Renderer* const renderer, Uint32 width, Uint32 height) noexcept
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
