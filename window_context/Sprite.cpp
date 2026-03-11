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

	Sprite::Sprite(Sprite&& rhs)noexcept
		:mTexture(rhs.mTexture), mSource(rhs.mSource), mTextureW(rhs.mTextureW), mTextureH(rhs.mTextureH)
	{
		rhs.mTexture = nullptr;
	}
	Sprite& Sprite::operator=(Sprite&& rhs)noexcept
	{
		if (this != &rhs) {
			mTexture = rhs.mTexture;
			rhs.mTexture = nullptr;
			mSource = rhs.mSource;
			mTextureW = rhs.mTextureW;
			mTextureH = rhs.mTextureH;
		}
		return *this;
	}

	bool Sprite::draw(SDL_Renderer* const renderer, const SDL_FRect& dst)const noexcept
	{
		return draw(renderer, mSource, dst);
	}

	bool Sprite::draw(SDL_Renderer* const renderer, const SDL_FRect& src, const SDL_FRect& dst)const noexcept
	{
		return SDL_RenderTexture(renderer, mTexture, &src, &dst);
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

	const SDL_FRect& Sprite::get_source()const noexcept
	{
		return mSource;
	}

	float Sprite::get_width()const noexcept
	{
		return mTextureW;
	}

	float Sprite::get_height()const noexcept
	{
		return mTextureH;
	}
}