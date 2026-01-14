#pragma once

#include "SDLCleanUp.h"
#include "GraphicsSys.h"

//TODO: WRONG SHIT
//1) TextureBase is meant to be the origin, the truth of what a texture is. it owns the memory
//2) TextureBase should be loaded up and stored once
//3) All users (object types) of texture base actually use SDL_Texture which use a raw pointer as a reference
//4) That pointer can be naively copied, thus rule of 0 applies. That pointer should never be deleted manually.
//5) That pointer can become dangling in case TextureBase gets deleted before user. This however should never happen
//6) 
//TODO:: texture collection, and sprites get created via texture collection

namespace badEngine {

	class TextureBase {

		using Texture = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;

	public:

		SDL_Texture* const get()const noexcept {
			return mTexture.get();
		}
		
		virtual ~TextureBase() = default;

	protected:

		TextureBase() = default;

		Texture mTexture;
	};

	class StaticTexture :public TextureBase {

	public:
		StaticTexture(SDL_Surface& surface, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.create_texture_static(&surface);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}
		StaticTexture(std::string_view path, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.create_texture_static(path);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}
	};

	class TargetTexture :public TextureBase {
	public:

		TargetTexture(Uint32 w, Uint32 h, const GraphicsSys& gfx)
		{
			SDL_Texture* txtr = gfx.create_texture_targetable(w, h);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}
		TargetTexture(Uint32 w, Uint32 h, const GraphicsSys& gfx, SDL_Texture* copy_from, AABB* src = nullptr, AABB* dest = nullptr)
		{
			SDL_Texture* txtr = gfx.create_texture_targetable(w, h, copy_from, src, dest);
			assert(txtr != nullptr);
			mTexture.reset(txtr);
		}
	};
}