#pragma once

#include "Sprite.h"
#include <SDL3/SDL_rect.h>
#include <vector>

namespace badWindow
{
	// Extension of sprite. Manages the source control by updating the source on a given time interval.
	// Subdivide the whole image supporting multiple different animations, or only specific region. Does not support different sized frames.
	class Animation final
	{
	public:
		Animation(SDL_Texture* texture, uint16_t frameWidth, uint16_t frameHeight, uint16_t* nColumns = nullptr, uint16_t* nRows = nullptr);

		// update step
		void update(float step)noexcept;

		// set interval in which a frame should be changed
		void set_hold_time(float length)noexcept;

		// set the line of animation to run
		uint16_t get_lines_count()const noexcept;

		//get the count for lines total
		void set_line(uint16_t line)noexcept;

		const SDL_FRect& get_source()const noexcept;

		SDL_Texture* const get_texture()const noexcept;

	private:
		class Sprite mSprite;
		std::vector<SDL_FPoint> mFrames;

		float mFrameLength = 0.08f;
		float mCurrFrameDuration = 0.0f;

		uint16_t mColumnsN = 0;
		uint16_t mCurrentColumn = 0;

		uint16_t mRowsN = 0;
		uint16_t mCurrentRow = 0;
	};
}