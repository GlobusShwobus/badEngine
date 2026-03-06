#pragma once

#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	struct TextureDeleter {
		void operator()(SDL_Texture* t) {
			if (t)
				SDL_DestroyTexture(t);
		}
	};

	using Texture = std::unique_ptr<SDL_Texture, TextureDeleter>;
}

/*
REMINDERS:

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

*/