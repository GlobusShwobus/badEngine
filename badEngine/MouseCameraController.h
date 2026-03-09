#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "Transform.h"
#include "Rect.h"

namespace badEngine
{
	class MouseCameraController
	{
		static constexpr float ZOOM_IN = 1.25f;
		static constexpr float ZOOM_OUT = 0.75f;

	public:
		explicit MouseCameraController() noexcept;
		MouseCameraController(const badCore::float2& pos, float scale, float inital_radian, float rotation_speed) noexcept;

		void update(float dt, const SDL_Event& events)noexcept;

		badCore::Rect get_viewport(SDL_Window* const window)const;

		badCore::Transform mCamera;
		float mRotationSpeed;

	private:

		bool mDragging;
	};
}