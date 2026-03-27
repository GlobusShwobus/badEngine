#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "Transform.h"
#include "Rect.h"

namespace bad
{
	class MouseCameraController
	{
		static constexpr float ZOOM_FACTOR = 1.15f;
		static constexpr float ROTATION_SPEED = 5.0f;

	public:

		MouseCameraController() noexcept;
		explicit MouseCameraController(const bad::Transform& transform)noexcept;
		MouseCameraController(const bad::Point& pos, float scale, float angle_radian)noexcept;

		void read_input(float dt, const SDL_Event& events)noexcept;

		bad::Mat3 to_matrix()const noexcept;

		//if window is nullptr adds no offset
		bad::Mat3 to_matrix_with_screen_offset(SDL_Window* const window)noexcept;

		bad::Rect get_viewport(SDL_Window* const window)const;

	private:
		bad::Transform mTransform;
		bool mDragging = false;
		
		static constexpr float rotational_vel = 2.f;
	};
}