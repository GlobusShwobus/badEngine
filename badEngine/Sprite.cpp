#include "Sprite.h"
#include <assert.h>
namespace badEngine {

	Sprite::Sprite(const Texture& texture)noexcept
	{
		assert(texture.get() != nullptr);
		mTexture = texture.get();
		float w, h;
		SDL_GetTextureSize(mTexture, &w, &h);
		mSource = AABB(0, 0, w, h);
		mDest = AABB(0, 0, w, h);
	}

	void Sprite::set_scale(float scale) noexcept
	{
		assert(scale > BAD_EPSILON);
		mScale = scale;
		//use base size of texture not already scaled dest
		mDest.w = mSource.w * mScale;
		mDest.h = mSource.h * mScale;
	}

	float Sprite::get_scale()const noexcept
	{
		return mScale;
	}

	void Sprite::set_pos(const float2& pos)noexcept
	{
		mDest.x = pos.x;
		mDest.y = pos.y;
	}

	BasicSprite::BasicSprite(const Texture& texture)
		:Sprite(texture)
	{}

	//####################################################################################
	Animation::Animation(const Texture& texture, uint16_t frameWidth, uint16_t frameHeight, uint16_t* nColumns, uint16_t* nRows)
		:Sprite(texture)
	{
		float textureW, textureH;
		SDL_GetTextureSize(mTexture, &textureW, &textureH);
		//set values for iteration, internally frames are stored as 2D array
		uint16_t columnCount = (nColumns != nullptr) ? *nColumns : static_cast<uint16_t>(textureW / frameWidth);
		uint16_t rowCount = (nRows != nullptr) ? *nRows : static_cast<uint16_t>(textureH / frameHeight);

		//check if the entire demand is within the control block
		assert(textureW >= (columnCount * frameWidth) && textureH >= (rowCount * frameHeight));

		//setup a 2D array
		for (uint16_t y = 0; y < rowCount; ++y) {
			for (uint16_t x = 0; x < columnCount; ++x) {
				mFrames.emplace_back(x * frameWidth, y * frameHeight);
			}
		}

		mColumnsN = columnCount;
		mRowsN = rowCount;

		const auto& firstFrame = mFrames.front();
		mSource = AABB(
			firstFrame.x,
			firstFrame.y,
			static_cast<float>(frameWidth),
			static_cast<float>(frameHeight)
		);
		mDest = mSource;
	}
	void Animation::update(float step)noexcept {
		//add to the time counter
		mCurrentFrameTime += step;

		if (mCurrentFrameTime >= mHoldTime) {

			//while if counter is more than hold time
			while (mCurrentFrameTime >= mHoldTime) {
				++mCurrentColumn;					    //next frame
				if (mCurrentColumn >= mColumnsN)        //if frame reached the end
					mCurrentColumn = 0;				    //reset

				mCurrentFrameTime -= mHoldTime;         //subtract 1 update cycle worth of time
			}

			//set source position [y*width+x]
			const auto& frame = mFrames[static_cast<std::size_t>(mCurrentRow * mColumnsN + mCurrentColumn)];
			mSource.x = frame.x;
			mSource.y = frame.y;
		}
	}
	void Animation::set_hold_time(float time)noexcept {
		assert(time > BAD_EPSILON);
		mHoldTime = time;
	}
	uint16_t Animation::get_lines_count()const noexcept {
		return mRowsN;
	}
	void Animation::set_line(uint16_t line)noexcept {
		assert(line < mRowsN);
		mCurrentRow = line;
	}
	//#########################################################################################

	Font::Font(const Texture& texture, uint32_t columnsCount, uint32_t rowsCount)
		:Sprite(texture),
		mColumnsCount(columnsCount)
	{
		float textureW, textureH;
		SDL_GetTextureSize(mTexture, &textureW, &textureH);

		int mGlyphWidth = static_cast<unsigned int>(textureW / columnsCount);
		int mGlyphHeight = static_cast<unsigned int>(textureH / rowsCount);
		//assert that data is pixel perfect
		assert(mGlyphWidth * columnsCount == textureW);
		assert(mGlyphHeight * rowsCount == textureH);
		mSource = AABB(0, 0, static_cast<float>(mGlyphWidth), static_cast<float>(mGlyphHeight));
		mDest = AABB(0, 0, static_cast<float>(mGlyphWidth) * mScale, static_cast<float>(mGlyphHeight) * mScale);
	}

	void Font::set_text(std::string_view string)noexcept
	{
		clear();           //clear text, memory buffer stays

		AABB destinatioon = AABB(mDest.x, mDest.y, mDest.w, mDest.h);   //inital dest
		const float glyphW = mSource.w;
		const float glyphH = mSource.h;

		for (char c : string) {
			//if new line character then set cursor to the beginning which is pos.x and go down by height (scaled) then continue to next char
			if (c == '\n') {
				destinatioon.x = mDest.x;
				destinatioon.y += destinatioon.h;
				continue;
			}
			//if spacebar (' ') character, then move by width (scaled) then continue to next char
			if (c == first_ASCII_character) {
				destinatioon.x += destinatioon.w;
				continue;
			}
			//for any printable character
			if (c >= first_ASCII_character + 1 && c <= last_ASCII_character) {
				int glyphIndex = c - first_ASCII_character;  //convert char to its index in the texture atlas (0 based)
			
				const uint32_t sourceX = glyphIndex % mColumnsCount; //unflatten 2D to 1D
				const uint32_t sourceY = glyphIndex / mColumnsCount; //unflatten 2D to 1D
				mLetterPos.emplace_back(
					AABB(
						static_cast<float>(sourceX * glyphW), //source x
						static_cast<float>(sourceY * glyphH), //source y
						static_cast<float>(glyphW),           //source w
						static_cast<float>(glyphH)            //source h
					),
					destinatioon
				);
				//move cursor to next 
				destinatioon.x += destinatioon.w;
			}
		}
	}

	void Font::update()noexcept
	{
		if (mLetterPos.isEmpty()) {
			return;
		}
		// store original base
		const AABB originalBase = mLetterPos.front().second;

		// calculate scale factors RELATIVE to original base
		const float sx = mDest.w / originalBase.w;
		const float sy = mDest.h / originalBase.h;

		// calculate translation based on original base
		const float dx = mDest.x - originalBase.x;
		const float dy = mDest.y - originalBase.y;

		for (auto& pair : mLetterPos) {
			auto& d = pair.second;

			// Apply transformation relative to original base
			d.x = mDest.x + (d.x - originalBase.x) * sx;
			d.y = mDest.y + (d.y - originalBase.y) * sy;

			// Scale width and height
			d.w = originalBase.w * sx;
			d.h = originalBase.h * sy;
		}
	}

	void Font::clear()noexcept
	{
		mLetterPos.clear();
	}
}