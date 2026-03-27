#include "pch.h"
#include "MouseCameraController.h"

bad::MouseCameraController::MouseCameraController() noexcept
	:mTransform{}
{
}

bad::MouseCameraController::MouseCameraController(const bad::Transform& transform) noexcept
	:mTransform(transform)
{
}

bad::MouseCameraController::MouseCameraController(const bad::Point& pos, float scale, float angle_radian) noexcept
	:mTransform(pos, scale, angle_radian)
{
}


void bad::MouseCameraController::read_input(float dt, const SDL_Event& events)noexcept
{
	switch (events.type)
	{
	case SDL_EVENT_KEY_DOWN:

		if (events.key.key == SDLK_Q)
			mTransform.rotate_by(rotational_vel * dt);

		if (events.key.key == SDLK_E)
			mTransform.rotate_by(-rotational_vel * dt);

		break;

	case SDL_EVENT_MOUSE_WHEEL:

	{
		float factor = (events.wheel.y > 0) ? (1.0f / ZOOM_FACTOR) : ZOOM_FACTOR;
		mTransform.scale_by(factor);
	}
		break;


	case SDL_EVENT_MOUSE_BUTTON_DOWN:

		if (events.button.button == SDL_BUTTON_LEFT)
			mDragging = true;

		break;

	case SDL_EVENT_MOUSE_BUTTON_UP:

		if (events.button.button == SDL_BUTTON_LEFT)
			mDragging = false;

		break;

	case SDL_EVENT_MOUSE_MOTION:

		if (mDragging)
		{
			bad::Vector delta{ events.motion.xrel, events.motion.yrel };
			// rotate delta by -camera_angle so pan follows screen axes
			delta = bad::rotate(delta, -mTransform.get_sin(), -mTransform.get_cos());
			mTransform.move_by(delta);
		}

		break;

	default:
		break;
	}
}