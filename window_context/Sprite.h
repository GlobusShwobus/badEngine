#pragma once

#include <SDL3/SDL_render.h>

namespace badWindow
{
	// Sprite stores a non owning ptr to an SDL_Texture
	// Sprite only manages the source control, the region of the texture to be drawn.
	struct Sprite 
	{
		explicit Sprite(SDL_Texture* texture);

		Sprite() = delete;
		
		void set_source_pos(float x, float y)noexcept;

		void set_source_size(float w, float h)noexcept;

		void set_source(const SDL_FRect& aabb)noexcept;

		SDL_Texture* const mTexture = nullptr;
		SDL_FRect mSource;
		float mTextureW;
		float mTextureH;
	};
}
