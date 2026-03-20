#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "Transform.h"
#include "Rect.h"

namespace bad
{
	class MouseCameraController
	{
		static constexpr float ZOOM_IN = 1.25f;
		static constexpr float ZOOM_OUT = 0.75f;

	public:
		explicit MouseCameraController() noexcept;
		MouseCameraController(const bad::Point& pos, float scale, float inital_radian, float rotation_speed) noexcept;

		void update(float dt, const SDL_Event& events)noexcept;

		//NOTE:: if the camera rotates then technically the viewport is busted... rotation is not taken into account
		//			not sure how to fix it atm. either view port must also consider rotation OR the entities must consider rotation OR realistically eveyrhing has to
		bad::Rect get_viewport(SDL_Window* const window)const;

		bad::Transform mCamera;

	private:
		float mRotationSpeed;
		bool mDragging;
	};
}