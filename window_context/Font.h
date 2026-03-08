#pragma once

#include <string_view>
#include <vector>
#include "Sprite.h"

namespace badWindow
{
	// Font is an extension of Sprite. Instead of having a single source region of a texture, font provides multiple
	// for text drawing. Expects respect of ASCII character order. Only characters between ' ' and '~' are supported.
	class Font final
	{
	public:

		Font(SDL_Texture* texture, uint32_t columnsCount, uint32_t rowsCount);

		// collects pieces of the texture to draw as a text, if pos or scale is changed after set_text, text is not updated
		void set_text(std::string_view string)noexcept;

		//clears all text
		void clear()noexcept;

		SDL_Texture* const get_texture()const noexcept;

		constexpr auto begin()const noexcept
		{
			return mLetters.begin();
		}
		constexpr auto end()const noexcept
		{
			return mLetters.end();
		}
	private:
		Sprite mSprite;
		std::vector<SDL_FRect> mLetters;
		uint32_t mColumnsCount = 0;

		static constexpr char ASCII_begin = ' ';
		static constexpr char ASCII_end = '~';
	};
}