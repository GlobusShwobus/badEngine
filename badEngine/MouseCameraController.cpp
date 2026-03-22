#include "pch.h"
#include "MouseCameraController.h"

bad::MouseCameraController::MouseCameraController() noexcept
{
}

void bad::MouseCameraController::read_input(float dt, const SDL_Event& events)noexcept
{
	switch (events.type)
	{
	case SDL_EVENT_KEY_DOWN:


		break;

	case SDL_EVENT_MOUSE_WHEEL:


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

		break;

	default:
		break;
	}
}