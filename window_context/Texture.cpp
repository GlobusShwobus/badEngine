#include "pch.h"
#include "Texture.h"

namespace badEngine {
	Texture::Texture(SDL_Surface* surface, SDL_Renderer* renderer)
	{
		if (surface && renderer) {
			SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, surface);
			mTexture.reset(txtr);
		}
	}

	Texture::Texture(const char* path, SDL_Renderer* renderer)
	{
		if (path && renderer) {
			SDL_Texture* txtr = IMG_LoadTexture(renderer, path);
			mTexture.reset(txtr);
		}
	}

	SDL_Texture* const Texture::get()const noexcept
	{
		return mTexture.get();
	}
}