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
	badCore::AABB MouseCameraController::get_viewport(const badCore::int2& window_size)const noexcept
	{
		const float zoom = 1.0f / mCamera.get_scale();
		return badCore::make_aabb(mCamera.get_pos(), (window_size.x * 0.5f) * zoom, (window_size.y * 0.5f) * zoom);
	}

	void MouseCameraController::rotate_by(float dt) noexcept {
		const float angle = mCamera.get_angle();
		float new_angle = angle + mRotationSpeed * dt;

		if (new_angle > 360.0f) new_angle -= 360.0f;
		if (new_angle < 0.0f)   new_angle += 360.0f;

		mCamera.set_angle(new_angle);
	}
}