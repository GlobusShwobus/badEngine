#pragma once

#include "GraphicsSys.h"

namespace badEngine {

	class SDLTextureDeleter {
	public:
		void operator()(SDL_Texture* t)noexcept {
			if (t)
				SDL_DestroyTexture(t);
		}
	};


	class TextureBase {

	protected:
		TextureBase() = default;
		virtual ~TextureBase() = default;

	public:

		SDL_Texture* const get()const noexcept {
			return mTexture.get();
		}

		bool isNullPtr()const noexcept {
			return mTexture == nullptr;
		}

	protected:
		std::unique_ptr<SDL_Texture, SDLTextureDeleter> mTexture;
	};

	class StaticTexture :public TextureBase {

	public:
		StaticTexture(SDL_Surface& surface, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.load_texture_static(&surface);
			assert(txtr != nullptr && "Texture is nullptr");
			mTexture.reset(txtr);
		}
		StaticTexture(std::string_view path, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.load_texture_static(path);
			assert(txtr != nullptr && "Texture is nullptr");
			mTexture.reset(txtr);
		}
	};

	class TargetTexture :public TextureBase {
	public:

		TargetTexture(Uint32 w, Uint32 h, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.create_texture_targetable(w, h);
			assert(txtr != nullptr && "Texture is nullptr");
			mTexture.reset(txtr);
		}
		TargetTexture(Uint32 w, Uint32 h, const GraphicsSys& gfx, SDL_Texture* copy_from, AABB* src = nullptr, AABB* dest = nullptr)
		{
			SDL_Texture* txtr = gfx.create_texture_targetable(w, h, copy_from, src, dest);
			assert(txtr != nullptr && "Texture is nullptr");
			mTexture.reset(txtr);
		}
	};
}