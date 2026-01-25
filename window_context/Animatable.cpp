#include "pch.h"
#include "Animatable.h"

namespace badWindow
{
	Animation::Animation(SDL_Texture* texture, uint16_t frameWidth, uint16_t frameHeight, uint16_t* nColumns, uint16_t* nRows)
		:mSprite(texture)
	{
		float textureW, textureH;
		SDL_GetTextureSize(mSprite.get_texture(), &textureW, &textureH);
		//set values for iteration, internally frames are stored as 2D array
		uint16_t columnCount = (nColumns != nullptr) ? *nColumns : static_cast<uint16_t>(textureW / frameWidth);
		uint16_t rowCount = (nRows != nullptr) ? *nRows : static_cast<uint16_t>(textureH / frameHeight);

		//check if the entire demand is within the control block
		assert(textureW >= (columnCount * frameWidth) && textureH >= (rowCount * frameHeight));

		//setup a 2D array
		mFrames.set_capacity(static_cast<std::size_t>(rowCount * columnCount));
		for (uint16_t y = 0; y < rowCount; ++y) {
			for (uint16_t x = 0; x < columnCount; ++x) {
				mFrames.emplace_back(x * frameWidth, y * frameHeight);
			}
		}

		mColumnsN = columnCount;
		mRowsN = rowCount;

		mSprite.set_source_size(static_cast<float>(frameWidth), static_cast<float>(frameHeight));
	}

	void Animation::update(float step)noexcept {
		//add to the time counter
		mCurrFrameDuration += step;

		if (mCurrFrameDuration >= mFrameLength) {

			//while if counter is more than hold time
			while (mCurrFrameDuration >= mFrameLength) {
				++mCurrentColumn;					    //next frame
				if (mCurrentColumn >= mColumnsN)        //if frame reached the end
					mCurrentColumn = 0;				    //reset

				mCurrFrameDuration -= mFrameLength;         //subtract 1 update cycle worth of time
			}

			//set source position [y*width+x]
			const auto& frame = mFrames[static_cast<std::size_t>(mCurrentRow * mColumnsN + mCurrentColumn)];
			mSprite.set_source_pos(frame.x, frame.y);
		}
	}

	void Animation::set_hold_time(float length)noexcept
	{
		assert(length > CORE_EPSILON);
		mFrameLength = length;
	}

	uint16_t Animation::get_lines_count()const noexcept
	{
		return mRowsN;
	}

	void Animation::set_line(uint16_t line)noexcept 
	{
		assert(line < mRowsN);
		mCurrentRow = line;
	}

	const badCore::AABB& Animation::get_source()const noexcept
	{
		return mSprite.get_source();
	}

	SDL_Texture* const Animation::get_texture()const noexcept
	{
		return mSprite.get_texture();
	}
}