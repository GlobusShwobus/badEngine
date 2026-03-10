#include "pch.h"
#include "Sprite.h"
#include <assert.h>
#include <stdexcept>

namespace badWindow 
{
	Sprite::Sprite(SDL_Texture* const texture)
		:mTexture(texture)
	{
		if (!mTexture)
			throw std::runtime_error("Sprite initalized with nullptr");

		SDL_GetTextureSize(mTexture, &mTextureW, &mTextureH);
		mSource = SDL_FRect(0, 0, mTextureW, mTextureH);
	}

	void Sprite::set_source_pos(float x, float y)noexcept
	{
		assert(x >= 0 && y >= 0);
		assert(x + mSource.w <= mTextureW);
		assert(y + mSource.h <= mTextureH);

		mSource.x = x;
		mSource.y = y;
	}

	void Sprite::set_source_size(float w, float h)noexcept
	{
		assert(w > 0 && h > 0);
		assert(mSource.x + w <= mTextureW);
		assert(mSource.y + h <= mTextureH);

		mSource.w = w;
		mSource.h = h;
	}

	void Sprite::set_source(const SDL_FRect& aabb)noexcept
	{
		assert(aabb.x >= 0 && aabb.y >= 0);
		assert(aabb.w >  0 && aabb.h >  0);
		assert(aabb.x + aabb.w <= mTextureW);
		assert(aabb.y + aabb.h <= mTextureH);
		mSource = aabb;
	}
}