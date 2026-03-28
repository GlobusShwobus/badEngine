#include "pch.h"
#include "Animatable.h"
#include <algorithm>

bad::Clip bad::make_clip(float texture_w, float texture_h, const SDL_FRect& first_frame, uint16_t frame_count, float frame_duration, bool loops)
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
	clip.frame_duration = frame_duration;
	clip.loop = loops;
	clip.frames.reserve(frame_count);

	for (uint16_t i = 0; i < frame_count; ++i)
		clip.frames.emplace_back(SDL_FRect{ first_frame.x + frame_w * i, first_frame.y, frame_w, frame_h });

	return clip;
}

void bad::AnimationPlayer::update(float dt)
{
	if (!mCurrentClip)
		return;

	mTimer += dt;

	if (mTimer >= mCurrentClip->frame_duration)
	{
		const uint32_t numFrames = static_cast<uint32_t>(mCurrentClip->frames.size());
		const float frameDur = mCurrentClip->frame_duration;

		//calculate total frames to advance
		const uint32_t framesToAdvance = static_cast<uint32_t>(mTimer / frameDur);
		mTimer = std::fmod(mTimer, frameDur);

		// Calculate new frame position
		uint32_t newFrame = mCurrentFrame + framesToAdvance;

		//if loops the correct frame is the remainder of how many times numFrames fits into newFrame
		if (mCurrentClip->loop)
			mCurrentFrame = newFrame % numFrames;
		else//if it doesn't look then must check if newFrames is larger than numframes
			mCurrentFrame = (newFrame < numFrames) ? newFrame : (numFrames - 1);

		mSprite.set_source(mCurrentClip->frames[mCurrentFrame]);
	}
}

void bad::AnimationPlayer::add_clip(AnimID id, Clip&& clip)
{
	if (clip.frames.empty())
		throw std::logic_error("Animation clip has no frames");

	float total_width = 0.0f;
	float max_height = 0.0f;

	std::for_each(clip.frames.begin(), clip.frames.end(), [&](const SDL_FRect& frame) {
		total_width += frame.w;
		max_height = std::max(max_height, frame.h);
		}
	);

	if (total_width > mSprite.get_width() || max_height > mSprite.get_height())
		throw std::logic_error("AnimationPlayer::add_clip: clip frames exceed sprite bounds");

	auto [it, inserted] = mClips.emplace(id, std::move(clip));

	if (!inserted)
		throw std::logic_error("AnimationPlayer::add_clip: duplicate animation id");
}

void bad::AnimationPlayer::play(AnimID id)
{
	auto it = mClips.find(id);

	if (it == mClips.end())
		throw std::invalid_argument("AnimationPlayer::play: was called with an id that doesn't exist");

	//prevent replaying same clip
	if (mCurrentClip == &it->second)
		return;

	mCurrentClip = &it->second;

	mCurrentFrame = 0;
	mTimer = 0.f;

	mSprite.set_source(mCurrentClip->frames[0]);
}

bool bad::AnimationPlayer::draw(SDL_Renderer* const renderer, const SDL_FRect& dest)const noexcept
{
	return SDL_RenderTexture(renderer, &mSprite.get_texture(), &mSprite.get_source(), &dest);
}