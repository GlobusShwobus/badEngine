#pragma once

#include <memory>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <string_view>
#include <cassert>
#include "SDLCleanUp.h"

namespace badEngine {

	//owner type of an SDL_Texture. if created with an SDL_Texture*, know that this type WILL take ownership.
	class Texture {

		using Texture_type = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;

	public:

		explicit Texture(SDL_Texture* texture)noexcept
		{
			assert(texture != nullptr);
			mTexture.reset(texture);
		}

		Texture(SDL_Surface* surface, SDL_Renderer* renderer)
		{
			SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, surface);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}
		Texture(std::string_view path, SDL_Renderer* renderer)
		{
			SDL_Texture* txtr = IMG_LoadTexture(renderer, path.data());
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}

		SDL_Texture* const get()const noexcept
		{
			return mTexture.get();
		}

	private:
		Texture_type mTexture;
	};
}