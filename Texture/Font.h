#pragma once

#include <string>
#include <vector>
#include "Sprite.h"

//TODO:: draw rotated?

namespace bad
{
	/// <summary> A handle for storing both source and destination of a Glyph. </summary>
	struct Glyph {
		SDL_FRect src;
		SDL_FRect dst;
	};

	/// <summary>
	/// A class that manages a texture that is built by dividing it into equal sections.
	/// The class is not able to actually promise what it draws further than doing dimension checks.
	/// Meaning any image may be passed as long as it meets the dimensional specfications.
	/// Manages all source and dest of the font internally.
	/// </summary>
	class Font final
	{
	public:

		/**
		* Creates the font class by dividing the given texture into equal parts using given parameters.
		* \param texture for texture
		* \param columns_count used to divide the width of the texture into equal parts
		* \param rows_count used to divide the height of the texture into equal parts
		* \throws in DEBUG the constructor asserts dimensions
		* \throws if texture is nullptr the Sprite member will throw.
		*/
		Font(SDL_Texture& texture, uint32_t columns_count, uint32_t rows_count);

		Font(const Font&) = default;
		Font(Font&&)noexcept = default;
		~Font()noexcept = default;

		Font() = delete;
		Font& operator=(const Font&) = delete;
		Font& operator=(Font&& rhs)noexcept = delete;

		/**
		* Sets the text to be drawn by setting the source glyph and then rebuilding the layout for relative destinations.
		*
		* \param text to parsed
		* \throws noexcept
		*/
		void set_text(const std::string& text)noexcept;

		/**
		* Sets the position of the text to be drawn and then rebuilding the layout for relative destinations.
		*
		* Does not do any checks for x or y.
		* \param x as the x position of the first glyph
		* \param y as the y position of the first glyph
		* \throws noexcept
		*/
		void set_position(float x, float y) noexcept;

		/**
		* Sets the scale of the text and then rebuilding the layout for relative destinations.
		*
		* Does not do any checks for scale param.
		* \param scale for scale per glyph
		* \throws noexcept
		*/
		void set_scale(float scale) noexcept;

		/**
		* Retruns the current value of scale.
		* \returns float scale
		* \throws noexcept
		*/
		inline float get_scale()const noexcept { return mScale; }

		/**
		* Draws the assigned texture on the screen handling all source and dest internally.
		*/
		bool draw(SDL_Renderer* const renderer) const noexcept;

	private:
		/// <summary> Stores currently set text. </summary>
		std::vector<Glyph> mGlyphs;

		/// <summary> Sprite that is used for drawing and dimension checking. </summary>
		Sprite mSprite;

		/// <summary> Caches the variable for unflattening a 2D grid to 1D (used in set_text) </summary>
		const uint32_t mColumnsCount;

		/// <summary> x position of the first letter </summary>
		float mPosX;

		/// <summary> y position of the first letter </summary>
		float mPosY;

		/// <summary> scale per gylph </summary>
		float mScale;

		/// <summary> width of a gylph. should not be modified </summary>
		float mGlyphW;

		/// <summary> height of a gylph. should not be modified </summary>
		float mGlyphH;

		static constexpr char ASCII_begin = ' ';
		static constexpr char ASCII_end = '~';

		/**
		* Rebuilds or more accurately sets accurate destinations of gylphs using pos x and y and scale.
		*
		* This is required as Font manages both source and dest of the drawn text.
		* \throws noexcept
		*/
		void rebuild_layout() noexcept;
	};
}