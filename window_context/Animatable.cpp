#include "pch.h"
#include "Animatable.h"

namespace badWindow
{
	Clip make_clip(float texture_w, float texture_h, const SDL_FRect& first_frame, uint16_t frame_count)
	{
		if (frame_count == 0)
			throw std::runtime_error("make_clip: frame_count cannot be zero");

		const float frame_w = first_frame.w;
		const float frame_h = first_frame.h;

		if (frame_w <= 0.f || frame_h <= 0.f)
			throw std::runtime_error("make_clip: frame size must be positive");

		// check first frame inside texture
		if (first_frame.x < 0 || first_frame.y < 0 ||
			first_frame.x + frame_w > texture_w ||
			first_frame.y + frame_h > texture_h)
		{
			throw std::runtime_error("make_clip: first frame outside texture bounds");
		}

		// check entire animation strip fits
		const float last_frame_right = first_frame.x + frame_w * frame_count;

		if (last_frame_right > texture_w)
			throw std::runtime_error("make_clip: animation strip exceeds texture width");


		Clip clip;
		clip.frames.reserve(frame_count);

		for (uint16_t i = 0; i < frame_count; ++i)
		{
			clip.frames.emplace_back(SDL_FRect{ first_frame.x + frame_w * i, first_frame.y, frame_w, frame_h });
		}

		return clip;
	}

	explicit AnimationPlayer::AnimationPlayer(SDL_Texture* const texture)
		:mSprite(texture), mTimer(0), mCurrentFrame(0)
	{
	}

	void AnimationPlayer::update(float dt) noexcept
	{
		if (!mCurrentClip)
			return;

		mTimer += dt;

		if (mTimer >= mCurrentClip->frame_duration)
		{
			while (mTimer >= mCurrentClip->frame_duration)
			{
				mTimer -= mCurrentClip->frame_duration;

				++mCurrentFrame;

				if (mCurrentFrame >= mCurrentClip->frames.size()) {
					if (mCurrentClip->loop)
						mCurrentFrame = 0;
					else
						mCurrentFrame = static_cast<uint32_t>(mCurrentClip->frames.size() - 1);
				}
			}
			mSprite.set_source(mCurrentClip->frames[mCurrentFrame]);
		}
	}

	void AnimationPlayer::add_clip(AnimID id, Clip clip)
	{
		mClips.emplace(id, std::move(clip));
	}

	void AnimationPlayer::play(AnimID id) noexcept
	{
		auto it = mClips.find(id);
		assert(it != mClips.end());

		mCurrentClip = &it->second;

		mCurrentFrame = 0;
		mTimer = 0.f;

		mSprite.set_source(mCurrentClip->frames[0]);
	}

	bool AnimationPlayer::draw(SDL_Renderer* const renderer, const SDL_FRect& dest)const noexcept
	{
		return mSprite.draw(renderer, dest);
	}

	const Sprite& AnimationPlayer::get_sprite()const noexcept
	{
		return mSprite;
	}
}