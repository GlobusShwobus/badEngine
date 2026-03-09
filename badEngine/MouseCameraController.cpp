#include "pch.h"
#include "MouseCameraController.h"
#include "Make_Shape.h"

namespace badEngine
{
	MouseCameraController::MouseCameraController() noexcept
		:mCamera({ 0,0 }, 1.0f, 0), mDragging(false), mRotationSpeed(1.0f)
	{
	}

	MouseCameraController::MouseCameraController(const badCore::float2& pos, float scale, float inital_radian, float rotation_speed) noexcept
		:mCamera(pos, scale, inital_radian), mDragging(false), mRotationSpeed(rotation_speed)
	{
	}

	void MouseCameraController::update(float dt, const SDL_Event& events)noexcept
	{
		switch (events.type)
		{
		case SDL_EVENT_KEY_DOWN:

			if (events.key.key == SDLK_Q)
				rotate_by(dt);//the camera perspective might be messing with me, maybe fliped is correct

			if (events.key.key == SDLK_E)
				rotate_by(-dt);//the camera perspective might be messing with me, maybe fliped is correct

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

			//NOTE: on zoom to keep the dragging relative to the zoom, divide delta by zoom
			if (mDragging) {
				float zoom = mCamera.mScale;

				mCamera.mPos.x -= events.motion.xrel / zoom;
				mCamera.mPos.y -= events.motion.yrel / zoom;
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
		const float zoom = 1.0f / mCamera.mScale;

		const float radius_x = (w * 0.5f) * zoom;
		const float radius_y = (h * 0.5f) * zoom;
		const auto& cam_pos = mCamera.mPos;

		return {
			cam_pos.x - radius_x,
			cam_pos.y - radius_y,
			radius_x * 2,
			radius_y * 2
		};
	}

	void MouseCameraController::rotate_by(float dt) noexcept
	{
		float new_radians = std::fmod(mCamera.get_radians() + mRotationSpeed * dt, badCore::TAU);
		mCamera.set_rotation(new_radians);
	}
}