#pragma once

#include "Float2.h"
#include "Mat3.h"
#include "MathConstants.h"

namespace bad
{
	class Transform
	{
	public:
		Transform()
			: mPos(0.f, 0.f)
			, mScale(1.f, 1.f)
			, mRadians(0.f)
			, mSin(0.f)
			, mCos(1.f)
		{
		}

		Transform(const bad::Point& pos, const bad::float2& scale, float radian)
			:mPos(pos)
			, mScale(scale)
		{
			set_radian(radian);
		}

		inline Mat3 to_matrix()const noexcept
		{
			return Mat3::translate(mPos) * Mat3::rotate(mSin, mCos) * Mat3::scale(mScale);
		}

		void set_position(const bad::Vector& v)noexcept
		{
			mPos = v;
		}

		void set_radian(float radian)noexcept
		{
			mRadians = std::fmod(radian, bad::TAU);

			if (mRadians < 0)
				mRadians += bad::TAU;

			mSin = std::sin(mRadians);
			mCos = std::cos(mRadians);
		}

		void set_scale(const bad::float2& scale)noexcept
		{
			mScale = scale;
		}

		float get_radian()const noexcept { return mRadians; }
		const bad::Point& get_pos()const noexcept { return mPos; }
		const bad::float2& get_scale()const noexcept { return mScale; }

	private:
		bad::Point mPos; 		/// <summary> Position (translation) </summary>

		bad::float2 mScale; 	/// <summary> Scaling factors </summary>

		float mRadians; 		/// <summary> Rotation angle in radians </summary>

		float mSin; 			/// <summary> Cached sine value of rotation </summary>

		float mCos; 			/// <summary> Cached cosine value of rotation </summary>
	};
}