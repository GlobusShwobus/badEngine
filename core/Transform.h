#pragma once

#include "Float2.h"
#include "Matrix3.h"


namespace badCore
{
	class Transform 
	{
	public:
		Transform(const float2& pos, float scale, float radians);

		Mat3 transform()const noexcept;

		Mat3 transform_inverse()const noexcept;

		void set_rotation(float radians)noexcept;

		void update_sincos()noexcept;

		constexpr float get_radians()const noexcept
		{
			return mRadians;
		}

		class float2 mPos;
		float mScale;

	private:
		float mRadians;
		float mSin;
		float mCos;
	};
}