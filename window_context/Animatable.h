#pragma once

#include "vector.h"
#include "Sprite.h"
#include "Sequence.h"

namespace badWindow
{
	// animatable object. manages the source texture by outputing a single source at a time by traversing via time steps
	class Animation final
	{
		using Frames = badCore::Sequence<badCore::float2>;
	public:
		Animation(const Texture& texture, uint16_t frameWidth, uint16_t frameHeight, uint16_t* nColumns = nullptr, uint16_t* nRows = nullptr);

		// update step
		void update(float step)noexcept;

		// set interval in which a frame should be changed
		void set_hold_time(float length)noexcept;

		// set the line of animation to run
		uint16_t get_lines_count()const noexcept;

		//get the count for lines total
		void set_line(uint16_t line)noexcept;

		const badCore::AABB& get_source()const noexcept;

		SDL_Texture* const get_texture()const noexcept;

	private:
		Sprite mSprite;
		Frames mFrames;

		float mFrameLength = 0.08f;
		float mCurrFrameDuration = 0.0f;

		uint16_t mColumnsN = 0;
		uint16_t mCurrentColumn = 0;

		uint16_t mRowsN = 0;
		uint16_t mCurrentRow = 0;
	};
}