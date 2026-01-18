#pragma once
#include <SDL3/SDL_render.h>
#include "Sequence.h"
#include "Rectangle.h"
#include "Texture.h"

namespace badEngine {
	class WindowContext;//forward declare my thing

	// base class for all drawable types that contain SDL_Texture*
	// is meant to be used as an interface class
	class Sprite
	{
		friend class WindowContext;

	public:
		
		virtual ~Sprite() = default;

		float get_scale()const noexcept;

		// sets the width and height of destination using original source as a base
		void set_scale(float scale) noexcept;

		// sets the x and y of destination
		void set_pos(const float2& pos)noexcept;

	protected:
		// a texture can be either static or targetable, thus polymorphism is used to save on code duplication
		// a derived class of sprite however should always demand either a static or targetable texture becasue TextureBase is also an interface
		explicit Sprite(const Texture& texture) noexcept;

		AABB mSource;
		AABB mDest;
		SDL_Texture* const mTexture;
		float mScale = 1.0f;
	};

	// basic drawable sprite
	class BasicSprite: public Sprite
	{
	public:
		BasicSprite(const Texture& texture);
	};

	// animatable object, creates frames from a texture and updates based on a time step
	class Animation :public Sprite
	{
	public:
		Animation(const Texture& texture, uint16_t frameWidth, uint16_t frameHeight, uint16_t* nColumns = nullptr, uint16_t* nRows = nullptr);

		// update step
		void update(float step)noexcept;
		
		// set interval in which a frame should be changed
		void set_hold_time(float time)noexcept;

		// set the line of animation to run
		uint16_t get_lines_count()const noexcept;

		//get the count for lines total
		void set_line(uint16_t line)noexcept;

	private:
		Sequence<float2> mFrames;

		float mHoldTime = 0.08f;
		float mCurrentFrameTime = 0.0f;

		uint16_t mColumnsN = 0;
		uint16_t mRowsN = 0;

		uint16_t mCurrentColumn = 0;
		uint16_t mCurrentRow = 0;
	};

	// font object, requires the texture to be build in a specific way
	class Font : public Sprite
	{
		static constexpr char first_ASCII_character = ' ';
		static constexpr char last_ASCII_character = '~';

	public:

		Font(const Texture& texture, uint32_t columnsCount, uint32_t rowsCount);

		// collects pieces of the texture to draw as a text, if pos or scale is changed after set_text, text is not updated
		void set_text(std::string_view string)noexcept;

		// updates the current text (should be used only after pos/scale have been changed, otherwise wasted computation)
		void update()noexcept;

		//clears all text
		void clear()noexcept;

		const auto begin()const noexcept
		{
			return mLetterPos.begin();
		}
		const auto end()const noexcept
		{
			return mLetterPos.end();
		}
	private:
		Sequence<std::pair<AABB, AABB>> mLetterPos;
		uint32_t mColumnsCount = 0;
	};
}
