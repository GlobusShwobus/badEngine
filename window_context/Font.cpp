#include "pch.h"
#include "Font.h"

namespace badWindow
{
	Font::Font(SDL_Texture* texture, uint32_t columnsCount, uint32_t rowsCount)
		:mSprite(texture),
		mColumnsCount(columnsCount)
	{
		float textureW, textureH;
		SDL_GetTextureSize(mSprite.get_texture(), &textureW, &textureH);

		int mGlyphWidth = static_cast<unsigned int>(textureW / columnsCount);
		int mGlyphHeight = static_cast<unsigned int>(textureH / rowsCount);
		//assert that data is pixel perfect
		assert(mGlyphWidth * columnsCount == textureW);
		assert(mGlyphHeight * rowsCount == textureH);
		mSprite.set_source_size(static_cast<float>(mGlyphWidth), static_cast<float>(mGlyphHeight));
	}

	void Font::set_text(std::string_view string)noexcept
	{
		clear();         
		if (string.empty()) {
			return;
		}

		//AABB destinatioon = AABB(mDest.x, mDest.y, mDest.w, mDest.h);   //inital dest
		const float glyphW = mSprite.get_width();
		const float glyphH = mSprite.get_height();

		for (char c : string) {
			//for any printable character
			if (c >= ASCII_begin + 1 && c <= ASCII_end) {
				int glyphIndex = c - ASCII_begin;             //convert char to its index in the texture atlas (0 based)

				const uint32_t sourceX = glyphIndex % mColumnsCount; //unflatten 2D to 1D
				const uint32_t sourceY = glyphIndex / mColumnsCount; //unflatten 2D to 1D
				mLetterPos.emplace_back(
					badCore::AABB(
						static_cast<float>(sourceX * glyphW), //source x
						static_cast<float>(sourceY * glyphH), //source y
						static_cast<float>(glyphW),           //source w
						static_cast<float>(glyphH)            //source h
					)
				);
			}
		}
	}

	void Font::clear()noexcept
	{
		mLetterPos.clear();
	}
}