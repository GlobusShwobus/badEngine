#include "pch.h"
#include "MouseCameraController.h"
#include "Make_Shape.h"

namespace badEngine
{
	void MouseCameraController::update(float dt, const SDL_Event& events)noexcept
	{
		switch (events.type)
		{
		case SDL_EVENT_KEY_DOWN:

			if (events.key.key == SDLK_Q)
				rotate_by(dt);

			if (events.key.key == SDLK_E)
				rotate_by(-dt);

			break;

		case SDL_EVENT_MOUSE_WHEEL:

			if (events.wheel.y > 0) {
				mCamera.scale_by(ZOOM_OUT);
			}
			else if (events.wheel.y < 0) {
				mCamera.scale_by(ZOOM_IN);
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

			//NOTE: on zoom to keep the dragging relative to the zoom, divide delta by zoom
			if (mDragging) {
				// Convert screen movement to world movement
				float zoom = mCamera.get_scale();

				mCamera.move_by(badCore::float2(
					-events.motion.xrel / zoom,
					-events.motion.yrel / zoom
				));
			}
			break;

		default:
			break;
		}
	}
	badCore::Rect MouseCameraController::get_viewport(SDL_Window* const window)const
	{
		assert(window != nullptr);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		const float zoom = 1.0f / mCamera.get_scale();

		const float radius_x = (w * 0.5f) * zoom;
		const float radius_y = (h * 0.5f) * zoom;
		const auto& cam_pos = mCamera.get_pos();

		return {
			cam_pos.x - radius_x,
			cam_pos.y - radius_y,
			radius_x * 2,
			radius_y * 2
		};
	}

	void MouseCameraController::rotate_by(float dt) noexcept {
		const float angle = mCamera.get_angle();
		float new_angle = angle + mRotationSpeed * dt;

		if (new_angle > 360.0f) new_angle -= 360.0f;
		if (new_angle < 0.0f)   new_angle += 360.0f;

		mCamera.set_angle(new_angle);
	}

	const badCore::Transform MouseCameraController::get_camera()const noexcept
	{
		return mCamera;
	}
}