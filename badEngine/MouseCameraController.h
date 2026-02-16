#pragma once

#include "Transform.h"
#include <SDL3/SDL_events.h>
#include "AABB.h"

namespace badEngine
{
	class MouseCameraController
	{
		static constexpr float ZOOM_IN = 1.25f;
		static constexpr float ZOOM_OUT = 0.75f;

	public:
		MouseCameraController() = default;

		void update(float dt, const SDL_Event& events)noexcept;

		badCore::AABB get_viewport(const badCore::int2& window_size)const noexcept;

		void rotate_by(float dt) noexcept;

		const badCore::Transform get_camera()const noexcept
		{
			return mCamera;
		}

	private:

		badCore::Transform mCamera;
		bool mDragging = false;
		float mRotationSpeed = 1;
	};
}