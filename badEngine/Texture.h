#pragma once

#include <memory>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "SDLCleanUp.h"

namespace badEngine {

	// Texture is a static SDL_Texture type used for most drawable textures

	class Texture {

		using Texture_type = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;

	public:

		Texture() = default;

		Texture(SDL_Surface* surface, SDL_Renderer* renderer)
		{
			if (surface && renderer) {
				SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, surface);
				mTexture.reset(txtr);
			}
		}

		Texture(const char* path, SDL_Renderer* renderer)
		{
			if (path && renderer) {
				SDL_Texture* txtr = IMG_LoadTexture(renderer, path);
				mTexture.reset(txtr);
			}
		}

		SDL_Texture* const get()const noexcept
		{
			return mTexture.get();
		}

	private:
		Texture_type mTexture;
	};
}