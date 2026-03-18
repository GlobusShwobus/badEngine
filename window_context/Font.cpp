#include "pch.h"
#include "Font.h"

namespace badWindow
{
	Font::Font(SDL_Texture* const texture, uint32_t columns_count, uint32_t rows_count)
		:mSprite(texture), mColumnsCount(columns_count), mPosX(0), mPosY(0), mScale(1.0f)
	{
		//NOTE:: sprite will throw is texture is nullptr
		const float sprite_w = mSprite.get_width();
		const float sprite_h = mSprite.get_height();
		mGlyphW  = sprite_w / columns_count;
		mGlyphH  = sprite_h / rows_count;
		//assert that data is pixel perfect
		const uint32_t assert_width = mGlyphW * columns_count;
		const uint32_t assert_height = mGlyphH * rows_count;

		assert(assert_width == sprite_w);
		assert(assert_height == sprite_h);
		//set inital value of width and height of the texture to chunck. pos does not matter yet
		mSprite.set_source_size(mGlyphW, mGlyphH);
	}

	void Font::set_text(const std::string& text)noexcept
	{
		mGlyphs.clear();         
		if (text.empty())
			return;
		
		//AABB destinatioon = AABB(mDest.x, mDest.y, mDest.w, mDest.h);   //inital dest
		const float glyphW = mSprite.get_width();;
		const float glyphH = mSprite.get_height();
		
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

	void Font::set_position(float x, float y) noexcept
	{
		mPosX = x;
		mPosY = y;
		rebuild_layout();
	}

	void Font::set_scale(float scale) noexcept
	{
		mScale = scale;
		rebuild_layout();
	}

	float Font::get_scale()const noexcept
	{
		return mScale;
	}

	bool Font::draw(SDL_Renderer* const renderer) const noexcept
	{
		auto txtr = mSprite.get_texture();
		for (const auto& g : mGlyphs) {
			if (g.src.w == 0.f)//spacebar
				continue;

			SDL_RenderTexture(renderer, txtr, &g.src, &g.dst);
		}

		return true;
	}

	void Font::rebuild_layout() noexcept
	{
		//trackers for positions of glyphs. initally pos x and y for the first letter
		float cursorX = mPosX;
		float cursorY = mPosY;

		//adjusted size of the glyphs based the base width height of a cell
		const float w = mGlyphW * mScale;
		const float h = mGlyphH * mScale;

		for (auto& g : mGlyphs){
			//new line glyph 
			if (g.src.x == -1.f)
			{
				cursorX = mPosX;
				cursorY += h;
				continue;
			}

			g.dst =
			{
				cursorX,
				cursorY,
				w,
				h
			};

			cursorX += w;
		}
	}
}