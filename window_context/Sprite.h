#pragma once

#include <SDL3/SDL_render.h>
#include "Texture.h"
#include "AABB.h"

namespace badWindow {

	// Sprite stores a reference to an SDL_Texture. SDL_Texture lifetime is managed externally.
	// Sprite only manages the source control, the region of the texture to be drawn.
	class Sprite final
	{
	public:

		explicit Sprite(SDL_Texture* texture)noexcept;
		
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
