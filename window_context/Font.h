#pragma once

#include <string_view>
#include "Sequence.h"
#include "Sprite.h"

namespace badWindow
{
	// Font is an extension of Sprite. Instead of having a single source region of a texture, font provides multiple
	// for text drawing. Expects respect of ASCII character order. Only characters between ' ' and '~' are supported.
	class Font
	{
		static constexpr char ASCII_begin = ' ';
		static constexpr char ASCII_end = '~';
		using Letters = badCore::Sequence<badCore::AABB>;

	public:

		Font(SDL_Texture* texture, uint32_t columnsCount, uint32_t rowsCount);

		// collects pieces of the texture to draw as a text, if pos or scale is changed after set_text, text is not updated
		void set_text(std::string_view string)noexcept;

		//clears all text
		void clear()noexcept;

		constexpr auto begin()const noexcept
		{
			return mLetterPos.begin();
		}
		constexpr auto end()const noexcept
		{
			return mLetterPos.end();
		}
	private:
		Sprite mSprite;
		Letters mLetterPos;
		uint32_t mColumnsCount = 0;
	};
}