#pragma once

#include "Sprite.h"
#include <SDL3/SDL_rect.h>
#include <vector>
#include <unordered_map>

//TODO:: pause function
//TODO:: draw rotated

namespace badWindow
{
	/// <summary> An Enumeration type for naming different animations in the AnimationPlayer manager. </summary>
	enum class AnimID
	{
		Idle,
		Left,
		Right
	};

	/// <summary> A handle to describe what a clip means (rectangles as frames; frame duration; if the clip loops) </summary>
	struct Clip {
		std::vector<SDL_FRect> frames;
		float frame_duration = 0.08f;
		bool loop = false;
	};

	/**
	* Creates a clip.
	* \param width of the texture.
	* \param height of the texture.
	* \param first_frame as the position of the first frame.
	* \param frame_count as the count of how many frames the clip consists of.
	* \param optional frame_duration, set as 0.08f default
	* \param optional loops sets the clip to be a looping type or not.
	* \returns a clip with given count of frames
	* \throws std::runtime_error if the frame_count is 0.
	* \throws std::runtime_error if given width and/or height of the first_frame is 0 or negative.
	* \throws std::runtime_error if the first frame clips outside of the given texture dimensions.
	* \throws std::runtime_error if first_frame.width * frame_count is larger than texture width.
	*/
	Clip make_clip(float texture_w, float texture_h, const SDL_FRect& first_frame, uint16_t frame_count, float frame_duration = 0.08f, bool loops = false);

	/// <summary> 
	/// A manager class that stores clips for a given Texture. 
	/// Stores a texture ptr internally but is not the owner and does not manage the lifetime. Copy semantics disabled; moves are custom.
	/// </summary>
	class AnimationPlayer final
	{
	public:
		/**
		* Creates AnimationPlayer and internally a Sprite with the given texture.
		* 
		* Neither Sprite nor AnimationPlayer are owners of the texture and do not manage its lifetime beyond disabling local copy semantics.
		* \param texture pointer.
		* \throws AnimationPlayer does no checks but Sprite can internally throw std::runtime_error if given texture is nullptr.
		*/
		explicit AnimationPlayer(SDL_Texture* const texture)
			:mSprite(texture), mTimer(0), mCurrentFrame(0)
		{
		}

		/**
		* Move constructs *this from rhs.
		* 
		* Moves over all data except mTimer and mCurrent frame which are initalized to 0.
		* 
		* Sets rhs.mCurrentClip to nullptr.
		* 
		* Leaves rhs in valid but unspecified state. Any use of it is for all intents and purposes UB.
		* 
		* \throws noexcept
		*/
		AnimationPlayer(AnimationPlayer&& rhs)noexcept
			:mClips(std::move(rhs.mClips)), mCurrentClip(rhs.mCurrentClip), mSprite(std::move(rhs.mSprite)), mTimer(0.0f), mCurrentFrame(0)
		{
			rhs.mCurrentClip = nullptr;
		}

		/**
		* Uses move operator to move values from rhs to *this or does nothing if this = &rhs.
		* 
		* Moves over all data except mTimer and mCurrentFrame which are set to 0.
		* 
		* Sets rhs.mCurrentClip to nullptr.
		* 
		* Leaves rhs in valid but unspecified state. Any use of it is for all intents and purposes UB.
		* 
		* \throws noexcept
		*/
		AnimationPlayer& operator=(AnimationPlayer&& rhs)noexcept
		{
			if (this != &rhs) {
				mClips = std::move(rhs.mClips);
				mCurrentClip = rhs.mCurrentClip;
				rhs.mCurrentClip = nullptr;
				mSprite = std::move(rhs.mSprite);
				mTimer = 0.0f;
				mCurrentFrame = 0;
			}
			return *this;
		}

		AnimationPlayer() = delete;
		AnimationPlayer(const AnimationPlayer&) = delete;
		AnimationPlayer& operator=(const AnimationPlayer&) = delete;

		/// <summary> The destructor does nothing special. </summary>
		~AnimationPlayer()noexcept = default;

		/**
		* Updates the current clips frame index to the next frame.
		* 
		* Does nothing if there is no play() was not called or stop() was called.
		* 
		* Does nothing if given delta time is less than frame_duration.
		* 
		* On update calls Sprites set_source() function which in DEBUG may assert if configuration was faulty.
		* \param dt for delta time
		* \throws if clip is malformed it will throw out of bounds access errors.
		*/
		void update(float dt);

		/**
		* Adds a given clip to the list.
		* 
		* Performs a validation check making sure the passed clips dimensions fit the stored mSprite.
		* 
		* \param id the id of the clip
		* \param clip by rvalue 
		* \throws std::logic_error if given clips frame count is 0.
		* \throws std::logic_error if the total width of of the frames exceeds Sprites width
		* \throws std::logic_error if the maximum height of a frame is more than the Sprites height
		* \throws if duplicate id
		* \throws if std::unordered_map throws
		*/
		void add_clip(AnimID id, Clip&& clip);

		/**
		* Plays the requested clip.
		* 
		* Prevents calling play on the same ID twice.
		* 
		* Since clips are exposed for 
		* \param id as animation id
		* \throws std::invalid_argument if a clip with a given id is not stored intnernally.
		* \throws if clip is malformed it will throw out of bounds access errors.
		*/
		void play(AnimID id);

		/**
		* Sets the current clip to nullptr and sets counters to 0.
		* 
		* When update is called, nothing happens.
		* 
		* However the last drawn state will still be drawn.
		* \throws noexcept
		*/
		inline void stop()noexcept
		{
			mCurrentClip = nullptr;
			mTimer = 0.0f;
			mCurrentFrame = 0;
		}
		/**
		* Sets the counters to 0 but retains the current clip.
		* 
		* When update is called the animation starts from 0.
		*
		* \throws noexcept
		*/
		inline void restart()noexcept
		{
			mTimer = 0.0f;
			mCurrentFrame = 0;
		}

		/**
		* Draws the current frame onto the given rendering context.
		* 
		* \param renderer for rendering context.
		* \param dest for location where to draw on the window.
		* \returns bool true on success false on failure; call SDL_GetError() for more info.
		* \throws noexcept
		*/
		bool draw(SDL_Renderer* const renderer, const SDL_FRect& dest)const noexcept;

		/**
		* Exposes the sprite for read only access.
		* 
		* Intended for creating clips.
		* \returns cached Sprite const reference
		*/
		inline const Sprite& get_sprite()const noexcept { return mSprite; }

	private:

		/// <summary> Hash map for storing clips. </summary>
		std::unordered_map<AnimID, Clip> mClips;

		/// <summary> Tracks the currently clip set by .play() function. </summary>
		const Clip* mCurrentClip = nullptr;

		/// <summary> Sprite. </summary>
		Sprite mSprite;

		/// <summary> Tracks the time of the current clip. </summary>
		float mTimer;

		/// <summary> Tracks the exact frame of the current clip. </summary>
		uint32_t mCurrentFrame;
	};
}