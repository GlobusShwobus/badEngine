#pragma once
#include <SDL3/SDL_render.h>

namespace bad
{
	/// <summary>
	/// A class for handling a region of a texture.
	/// Sprite stores a non owning ptr to a texture. Copy semantics disabled, move custom.
	/// </summary>
	class Sprite final
	{
	public:
		/**
		* Caches texture, texture width and height and initalized the initial source region to size of the texture
		* \param texture as texture
		*/
		explicit Sprite(SDL_Texture& texture);

		Sprite(const Sprite&) = default;
		Sprite(Sprite&& rhs) noexcept = default;
		~Sprite()noexcept = default;

		Sprite() = delete;
		Sprite& operator=(const Sprite&) = delete;
		Sprite& operator=(Sprite&&)noexcept = delete;

		/**
		* Sets the x and y members of mSource
		* \param float x for x
		* \param float y for y
		* \throws In DEBUG asserts logical validity for x and y
		* \throws noexcept
		*/
		void set_source_pos(float x, float y)noexcept;

		/**
		* Sets the w and h members of mSource
		* \param float w for width
		* \param float h for height
		* \throws in DEBUG asserts logical validity for w and h
		* \throws noexcept
		*/
		void set_source_size(float w, float h)noexcept;

		/**
		* Sets the given rect as source rect
		* \param SDL_FRect for mSource
		* \throws in DEBUG asserts logical validity given rect
		* \throws noexcept
		*/
		void set_source(const SDL_FRect& aabb)noexcept;

		/**
		* \brief this method should never be used to modify the texture (delete or otherwise)
		* \return retruns SDL_Texture pointer
		*/
		SDL_Texture& get_texture()const noexcept { return mTexture; }

		/// <returns>mSource rectangle</returns>
		inline const SDL_FRect& get_source()const noexcept { return mSource; }

		/// <returns>float width of the texture</returns>
		inline float get_width()const noexcept { return mTextureW; }

		/// <returns>float height of the texture</returns>
		inline float get_height()const noexcept { return mTextureH; }

	private:

		/// <summary> Non owning pointer of a SDL_Texture. It is expected the lifetime of texture is handled externally. </summary>
		SDL_Texture& mTexture;

		/// <summary> A rectangle representing the area of the texture that is used for drawing. </summary>
		SDL_FRect mSource;

		/// <summary> Width of the texture </summary>
		float mTextureW;

		/// <summary> Height of the texture. </summary>
		float mTextureH;
	};
}