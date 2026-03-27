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
			delta = bad::rotate(delta, -mTransform.get_sin(), -mTransform.get_cos());
			mTransform.move_by(delta);
		}

		break;

	default:
		break;
	}
}

bad::Mat3 bad::MouseCameraController::to_matrix()const noexcept
{
	return mTransform.to_inverse_matrix();
}

bad::Mat3 bad::MouseCameraController::to_matrix_with_screen_offset(SDL_Window* const window)noexcept
{
	int w = 0, h = 0;
	SDL_GetWindowSize(window, &w, &h);

	float offset_x = w / 2.f;
	float offset_y = h / 2.f;

	return bad::Mat3::translate(offset_x, offset_y) * mTransform.to_inverse_matrix();
}

bad::Rect bad::MouseCameraController::get_viewport(SDL_Window* const window)const
{
	assert(window != nullptr);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	//do large radius to ignore rotation (not 100% correct but fine for braod phase intersection)
	int viewport_large_radius = w < h ? h : w;

	viewport_large_radius *= mTransform.get_scale();

	return bad::make_rect_from_center(mTransform.get_pos(), viewport_large_radius, viewport_large_radius);
}