#include "pch.h"
#include "MouseCameraController.h"

bad::MouseCameraController::MouseCameraController(const bad::Transform& transform) noexcept
	:mCamera(transform)
{
}

void bad::MouseCameraController::read_input(float dt, const SDL_Event& events)noexcept
{
	switch (events.type)
	{
	case SDL_EVENT_KEY_DOWN:
		//not sure yet. depends on when i test of with textures, but on mathmatical cartesian plane this is correct
		if (events.key.key == SDLK_Q)
			mCamera.set_rotation(std::fmod(mCamera.get_radians() + ROTATION_SPEED * dt, bad::TAU));

		if (events.key.key == SDLK_E)
			mCamera.set_rotation(std::fmod(mCamera.get_radians() + ROTATION_SPEED * -dt, bad::TAU));

		break;

	case SDL_EVENT_MOUSE_WHEEL:

		if (events.wheel.y > 0) {
			mCamera.mScale *= ZOOM_FACTOR;
		}
		else if (events.wheel.y < 0) {
			mCamera.mScale /= ZOOM_FACTOR;
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
			mCamera.mPos.x += events.motion.xrel;
			mCamera.mPos.y -= events.motion.yrel;
		}
		break;

	default:
		break;
	}
}

bad::Mat3 bad::MouseCameraController::get_view_matrix()const noexcept
{
	return mCamera.TRS_matrix();
}