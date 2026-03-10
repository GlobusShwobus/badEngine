#pragma once

#include "Sprite.h"
#include <SDL3/SDL_rect.h>
#include <vector>
#include <unordered_map>

namespace badWindow
{
	enum class AnimID
	{
		Idle,
		Left,
		Right
	};

	struct Clip {
		std::vector<SDL_FRect> frames;
		float frame_duration = 0.08f;
		bool loop = false;
	};

	Clip make_clip(float texture_w, float texture_h, const SDL_FRect& first_frame, uint16_t frame_count);

	class AnimationPlayer final
	{
	public:
		explicit AnimationPlayer(SDL_Texture* const texture);

		void update(float dt) noexcept;

		void add_clip(AnimID id, Clip clip);

		void play(AnimID id) noexcept;

		bool draw(SDL_Renderer* const renderer, const SDL_FRect& dest)const noexcept;

		const Sprite& get_sprite()const noexcept;

	private:
		std::unordered_map<AnimID, Clip> mClips;
		const Clip* mCurrentClip = nullptr;
		Sprite mSprite;

		float mTimer;
		uint32_t mCurrentFrame;
	};
}