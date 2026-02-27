#pragma once

#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	// Static texture type. Owns SDL_Texture and shares raw SDL_Texture pointer.
	// SDL_Texture should never be deleted externally and Texture should outlive the use case.
	class Texture final
	{
		struct TDeleter {
			void operator()(SDL_Texture* t) {
				if (t)
					SDL_DestroyTexture(t);
			}
		};

	public:

		Texture(SDL_Surface* surface, SDL_Renderer* renderer);

		Texture(const char* path, SDL_Renderer* renderer);

		SDL_Texture* const get()const noexcept;

	private:

		std::unique_ptr<SDL_Texture, TDeleter> mTexture;
	};
}