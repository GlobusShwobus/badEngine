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
		MouseCameraController() = default;

		void update(float dt, const SDL_Event& events)noexcept;

		badCore::Rect get_viewport(SDL_Window* const window)const;

		void rotate_by(float dt) noexcept;

		const badCore::Transform get_camera()const noexcept;

	private:

		badCore::Transform mCamera;
		bool mDragging = false;
		float mRotationSpeed = 1;
	};
}