#pragma once

#include <string>
#include <vector>
#include "Sprite.h"

namespace badWindow
{
	struct Glyph {
		SDL_FRect src;
		SDL_FRect dst;
	};

	class Font final
	{
	public:

		Font(SDL_Texture* const texture, uint32_t columns_count, uint32_t rows_count);

		void set_text(const std::string& text)noexcept;

		void set_position(float x, float y) noexcept
		{
			mPosX = x;
			mPosY = y;
			rebuild_layout();
		}

		void set_scale(float scale) noexcept
		{
			mScale = scale;
			rebuild_layout();
		}

		float get_scale()const noexcept
		{
			return mScale;
		}

		bool draw(SDL_Renderer* const renderer) const noexcept
		{
			for (const auto& g : mGlyphs){
				if (g.src.w == 0.f)//spacebar
					continue;

				mSprite.draw(renderer, g.src, g.dst);
			}

			return true;
		}

	private:
		std::vector<Glyph> mGlyphs;
		Sprite mSprite;

		uint32_t mColumnsCount = 0;

		float mPosX = 0.f;
		float mPosY = 0.f;

		float mScale = 1.f;

		float mGlyphW = 0.f;
		float mGlyphH = 0.f;

		static constexpr char ASCII_begin = ' ';
		static constexpr char ASCII_end = '~';


		void rebuild_layout() noexcept
		{
			float cursorX = mPosX;
			float cursorY = mPosY;

			const float w = mGlyphW * mScale;
			const float h = mGlyphH * mScale;

			for (auto& g : mGlyphs)
			{
				// newline marker
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
	};
}