#include "pch.h"
#include "Font.h"

namespace badWindow
{
	Font::Font(SDL_Texture* const texture, uint32_t columns_count, uint32_t rows_count)
		:mSprite(texture), mColumnsCount(columns_count)
	{
		//NOTE:: sprite will throw is texture is nullptr
		mGlyphW  = mSprite.mTextureW / columns_count;
		mGlyphH  = mSprite.mTextureH / rows_count;
		//assert that data is pixel perfect
		const uint32_t assert_width = mGlyphW * columns_count;
		const uint32_t assert_height = mGlyphH * rows_count;

		assert(assert_width == mSprite.mTextureW);
		assert(assert_height == mSprite.mTextureH);
		//set inital value of width and height of the texture to chunck. pos does not matter yet
		mSprite.set_source_size(mGlyphW, mGlyphH);
	}

	void Font::set_text(const std::string& text)noexcept
	{
		mGlyphs.clear();         
		if (text.empty())
			return;
		
		//AABB destinatioon = AABB(mDest.x, mDest.y, mDest.w, mDest.h);   //inital dest
		const float glyphW = mSprite.mTextureW;
		const float glyphH = mSprite.mTextureH;
		
		for (char c : text) {
			//newline
			if (c == '\n'){
				Glyph g{};
				g.src = { -1.f,-1.f,0.f,0.f }; // marker
				mGlyphs.push_back(g);
				continue;
			}
			//spacebar
			if (c == ' '){
				Glyph g{};
				g.src = { 0.f,0.f,0.f,0.f }; // invisible glyph
				mGlyphs.push_back(g);
				continue;
			}
			//for any printable character
			if (c >= ASCII_begin + 1 && c <= ASCII_end) {
				int glyphIndex = c - ASCII_begin;

				const uint32_t sx = glyphIndex % mColumnsCount;	 //unflatten 2D to 1D
				const uint32_t sy = glyphIndex / mColumnsCount;	 //unflatten 2D to 1D

				//create a glyph and only set its source
				Glyph g;

				g.src =
				{
					sx * mGlyphW,
					sy * mGlyphH,
					mGlyphW,
					mGlyphH
				};

				mGlyphs.push_back(g);
			}
		}
		rebuild_layout();
	}
}