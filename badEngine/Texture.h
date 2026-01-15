#pragma once

#include "SDLCleanUp.h"
#include "GraphicsSys.h"

namespace badEngine {

	//owner type of an SDL_Texture. if created with an SDL_Texture*, know that this type WILL take ownership.
	class Texture {

		using Texture_type = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;

	public:
		Texture(SDL_Texture* texture)
		{
			assert(texture != nullptr);
			mTexture.reset(texture);
		}
		Texture(SDL_Surface& surface, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.create_texture_static(&surface);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}
		Texture(std::string_view path, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.create_texture_static(path);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}

		SDL_Texture* const get()const noexcept {
			return mTexture.get();
		}
	private:
		Texture_type mTexture;
	};
}