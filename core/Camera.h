#pragma once

#include "vector.h"
#include "Matrix3.h"
#include "AABB.h"
#include "Make_Shape.h"

namespace badCore
{
	class Camera final
	{
	public:

		Camera() = default;

		float2 get_pos()const noexcept
		{
			return mPos;
		}

		void move_by(const float2& offset) noexcept
		{
			mPos += offset;
		}

		void move_to(const float2& pos)noexcept
		{
			mPos = pos;
		}

		Mat3 get_transform() const {
			return Mat3::scale(mScale, mScale) * Mat3::translation(-mPos);
		}

		float get_zoom()const noexcept
		{
			return mScale;
		}

		void set_zoom(float scale) noexcept
		{
			mScale = scale;
		}

		AABB get_viewport(const int2& window_size)const noexcept
		{
			const float zoom = 1.0f / mScale;
			return make_aabb(mPos, (window_size.x * 0.5f) * zoom, (window_size.y * 0.5f) * zoom);
		}

	private:
		float2 mPos;
		float mScale = 1.0f;
	};
}