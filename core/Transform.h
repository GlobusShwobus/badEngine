#pragma once

#include "vector.h"
#include "Matrix3.h"
//TODO::add .cpp

namespace badCore
{
	class Transform final
	{
	public:
		
		Transform() = default;

		float2 get_pos()const noexcept
		{
			return mPos;
		}

		void move_by(const float2& offset) noexcept
		{
			mPos += offset;
		}

		void set_pos(const float2& pos)noexcept
		{
			mPos = pos;
		}

		float get_scale()const noexcept
		{
			return mScale;
		}

		void scale_by(float scalar) noexcept
		{
			mScale *= scalar;
		}

		void set_scale(float scale)noexcept
		{
			mScale = scale;
		}

		float get_angle()const noexcept
		{
			return mAngle;
		}

		float rotate_by(float degrees)noexcept
		{
			mAngle += degrees;
		}

		void set_angle(float angle)noexcept
		{
			mAngle = angle;
		}


		Mat3 transform()const noexcept
		{
			//ORDER MATTERS
			return badCore::Mat3::translation(mPos) * badCore::Mat3::rotation(mAngle) * badCore::Mat3::scale(mScale, mScale);
		}

		Mat3 transform_inverse()const noexcept
		{
			const float invScale = 1.0f / mScale;
			//camera transform are inverse of object transforms (perspective)
			return Mat3::scale(invScale, invScale) * Mat3::rotation(-mAngle) * Mat3::translation(-mPos);
		}

	private:
		float2 mPos;
		float mScale = 1.0f;
		float mAngle = 0;
	};
}