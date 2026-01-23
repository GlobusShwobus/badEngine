#pragma once

#include "Texture.h"
#include "AABB.h"

namespace badWindow {

	// a base class for all static texture types. only manages the reference of a texture and source info. is not the owner
	// destination, scale, rotation is all external
	class Sprite final
	{
	public:

		explicit Sprite(const Texture& texture)noexcept;
		
		void set_source_pos(float x, float y)noexcept;

		void set_source_size(float w, float h)noexcept;

		void set_source(const badCore::AABB& aabb)noexcept;

		const badCore::AABB& get_source()const noexcept;

		float get_width()const noexcept;

		float get_height()const noexcept;
		
		SDL_Texture* const get_texture()const noexcept;

	private:
		SDL_Texture* const mTexture = nullptr;
		badCore::AABB mSource;
		float mTextureW;
		float mTextureH;
	};
}
