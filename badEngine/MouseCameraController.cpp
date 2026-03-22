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

		if (events.key.key == SDLK_Q)
			mCamera.set_rotation(std::fmod(mCamera.get_radians() - ROTATION_SPEED * dt, bad::TAU));

		if (events.key.key == SDLK_E)
			mCamera.set_rotation(std::fmod(mCamera.get_radians() + ROTATION_SPEED * dt, bad::TAU));

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
		//BECAUSE INVERSE TRS VIEW
		if (mDragging) 
		{
			mCamera.mPos.x -= events.motion.xrel;
			mCamera.mPos.y -= events.motion.yrel;
		}
		break;

	default:
		break;
	}
}

bad::Mat3 bad::MouseCameraController::get_view_matrix()const noexcept
{
	//VERY FUGGIN IMPORTANT. INVERSE. ALL LOGIC IS IN LEGIT MATH. VISUAL TRANSLATION IS IN MEME (WHOEVER THE ASSHOLE WAS WHO FLIPED CARTESIAN Y) FORMAT.
	return mCamera.TRS_inverse_matrix();
}