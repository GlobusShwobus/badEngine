#pragma once

#include <string>
#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	struct TextureDeleter {
		void operator()(SDL_Texture* t)
		{
			if (t)
				SDL_DestroyTexture(t);
		}
	};

	using Texture = std::unique_ptr<SDL_Texture, TextureDeleter>;

	Texture make_texture(SDL_Renderer* const renderer, SDL_Surface* const surface) noexcept;

	Texture make_texture(SDL_Renderer* const renderer, const std::string& path) noexcept;
}