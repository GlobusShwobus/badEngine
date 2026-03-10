#pragma once
#include <SDL3/SDL_render.h>

namespace badWindow
{
	class Sprite final
	{
	public:
		explicit Sprite(SDL_Texture* const texture);
		Sprite() = delete;

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;
		Sprite(Sprite&& rhs)noexcept
			:mTexture(rhs.mTexture), mSource(rhs.mSource), mTextureW(rhs.mTextureW), mTextureH(rhs.mTextureH)
		{
			rhs.mTexture = nullptr;
		}
		Sprite& operator=(Sprite&& rhs)noexcept
		{
			if (this != &rhs) {
				mTexture = rhs.mTexture;
				mSource = rhs.mSource;
				mTextureW = rhs.mTextureW;
				mTextureH = rhs.mTextureH;
			}
			return *this;
		}
		~Sprite()noexcept = default;

		void set_source_pos(float x, float y)noexcept;

		void set_source_size(float w, float h)noexcept;

		void set_source(const SDL_FRect& aabb)noexcept;

		bool draw(SDL_Renderer* const renderer, const SDL_FRect& dest)const noexcept
		{
			//NOTE:: if drawing fails, let SDL fail for SDL_GetError
			return SDL_RenderTexture(renderer, mTexture, &mSource, &dest);
		}

		SDL_FRect mSource;
		float mTextureW;
		float mTextureH;

	private:

		SDL_Texture* mTexture = nullptr;//not an owner so move assignment doesnt require cleanup of this
	};
}