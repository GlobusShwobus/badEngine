#include "pch.h"
#include "Texture.h"
#include <SDL3_image/SDL_image.h>
#include <assert.h>

namespace badWindow
{
	Texture make_texture(SDL_Renderer* const renderer, SDL_Surface* const surface)
	{
		assert(renderer != nullptr);
		assert(surface != nullptr);
		return Texture{ SDL_CreateTextureFromSurface(renderer, surface) };
	}

	Texture make_texture(SDL_Renderer* const renderer, const char* path)
	{
		assert(renderer != nullptr);
		assert(path != nullptr);

		return Texture{ IMG_LoadTexture(renderer, path) };
	}
}