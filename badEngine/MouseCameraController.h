#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "Transform.h"
#include "Rect.h"

namespace bad
{
	class MouseCameraController
	{
		static constexpr float ZOOM_FACTOR = 1.25f;
		static constexpr float ROTATION_SPEED = 5.0f;

	public:

		MouseCameraController(const Transform& t) noexcept;

		void read_input(float dt, const SDL_Event& events)noexcept;

		Mat3 get_view_matrix()const noexcept;

		bad::Transform mCamera;

	private:
		bool mDragging = false;
	};
}

/*
bad::Rect bad::MouseCameraController::get_viewport(SDL_Window* const window)const
{
	assert(window != nullptr);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	const float zoom = 1.0f / mCamera.mScale;

	const float viewport_w = (w * 0.5f) * zoom;
	const float viewport_h = (h * 0.5f) * zoom;

	return bad::make_rect_from_center(mCamera.mPos, viewport_w, viewport_h);
}
*/