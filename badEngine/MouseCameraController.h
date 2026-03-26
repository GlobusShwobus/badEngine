#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "Transform.h"

namespace bad
{
	class MouseCameraController
	{
		static constexpr float ZOOM_FACTOR = 1.25f;
		static constexpr float ROTATION_SPEED = 5.0f;

	public:

		MouseCameraController() noexcept;
		explicit MouseCameraController(const bad::Transform& transform)noexcept;
		MouseCameraController(const bad::Point& pos, const bad::float2& scale, float angle_radian)noexcept;

		void read_input(float dt, const SDL_Event& events)noexcept;

		bad::Mat3 to_matrix()const noexcept
		{
			return mTransform.to_inverse_matrix();
		}

		//if window is nullptr adds no offset
		bad::Mat3 to_matrix_with_screen_offset(SDL_Window* const window)noexcept
		{
			int w = 0, h = 0;
			SDL_GetWindowSize(window, &w, &h);

			float offset_x = w / 2.f;
			float offset_y = h / 2.f;

			return bad::Mat3::translate(offset_x, offset_y) * mTransform.to_inverse_matrix();
		}

	private:
		bad::Transform mTransform;
		bool mDragging = false;
		
		static constexpr float rotational_vel = 0.66f;
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