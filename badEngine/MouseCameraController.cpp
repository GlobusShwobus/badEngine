#include "pch.h"
#include "MouseCameraController.h"

bad::MouseCameraController::MouseCameraController(const bad::Transform& transform) noexcept
	:mCamera(transform)
{
}

void bad::MouseCameraController::update(float dt, const SDL_Event& events)noexcept
{
	switch (events.type)
	{
	case SDL_EVENT_KEY_DOWN:

		if (events.key.key == SDLK_Q)
			mCamera.set_rotation(std::fmod(mCamera.get_radians() + mRotationSpeed * dt, bad::TAU));

		if (events.key.key == SDLK_E)
			mCamera.set_rotation(std::fmod(mCamera.get_radians() + mRotationSpeed * -dt, bad::TAU));

		break;

	case SDL_EVENT_MOUSE_WHEEL:

		if (events.wheel.y > 0) {
			mCamera.mScale *= ZOOM_OUT;
		}
		else if (events.wheel.y < 0) {
			mCamera.mScale *= ZOOM_IN;
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
			mCamera.mPos.x -= events.motion.xrel;
			mCamera.mPos.y += events.motion.yrel;
		}
		break;

	default:
		break;
	}
}

bad::Rect bad::MouseCameraController::get_viewport(SDL_Window* const window)const
{
	assert(window != nullptr);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	const float radius_x = (w * 0.5f) * mCamera.mScale;
	const float radius_y = (h * 0.5f) * mCamera.mScale;
	const auto& cam_pos = mCamera.mPos;

	return {
		cam_pos.x - radius_x,
		cam_pos.y - radius_y,
		radius_x * 2,
		radius_y * 2
	};
}