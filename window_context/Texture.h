#pragma once

#include <memory>
#include <SDL3/SDL_render.h>
#include "mySDL_utils.h"

namespace badWindow
{
	// Static texture type. Owns SDL_Texture and shares raw SDL_Texture pointer.
	// SDL_Texture should never be deleted externally and Texture should outlive the use case.
	class Texture final
	{
		using Texture_type = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;

	public:

		Texture(SDL_Surface* surface, SDL_Renderer* renderer);

		Texture(const char* path, SDL_Renderer* renderer);

		SDL_Texture* const get()const noexcept;

	private:
		Texture_type mTexture;
	};
}