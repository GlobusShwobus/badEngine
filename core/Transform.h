#pragma once

#include "Float2.h"
#include "Matrix3.h"
//TODO::add .cpp

namespace badCore
{
	class Transform final
	{
	public:
		
		Transform() = default;

		constexpr const float2& get_pos()const noexcept
		{
			return mPos;
		}

		constexpr void move_by(const float2& offset) noexcept
		{
			mPos += offset;
		}

		constexpr void set_pos(const float2& pos)noexcept
		{
			mPos = pos;
		}

		constexpr float get_scale()const noexcept
		{
			return mScale;
		}

		constexpr void scale_by(float scalar) noexcept
		{
			mScale *= scalar;
		}

		constexpr void set_scale(float scale)noexcept
		{
			mScale = scale;
		}

		constexpr float get_angle()const noexcept
		{
			return mAngle;
		}

		constexpr void rotate_by(float degrees)noexcept
		{
			mAngle += degrees;
		}

		constexpr void set_angle(float angle)noexcept
		{
			mAngle = angle;
		}


		Mat3 transform()const noexcept;

		Mat3 transform_inverse()const noexcept;

	private:
		class float2 mPos;
		float mScale = 1.0f;
		float mAngle = 0;
	};
}