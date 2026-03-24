#pragma once

#include "Float2.h"
#include "Mat3.h"
#include "MathConstants.h"

namespace bad
{
	class Transform
	{
	public:
		Transform(const bad::Point& pos, const bad::float2& scale, float radians)
			:mPos(pos), mScale(scale), mRadians(radians), mSin(std::sin(radians)), mCos(std::cos(radians))
		{
		}

		inline Mat3 TRS_matrix()const noexcept
		{
			return Mat3::translate(mPos) * Mat3::rotate(mSin, mCos) * Mat3::scale(mScale);
		}

		void translate(const bad::Vector& vector) noexcept
		{
			mPos += vector;
		}

		void rotate(float radians)noexcept
		{

			mRadians = std::fmod(mRadians + radians, bad::TAU);

			if (mRadians < 0)
				mRadians += bad::TAU;

			mSin = std::sin(mRadians);
			mCos = std::cos(mRadians);
		}

		void scale(const bad::float2& scale)noexcept
		{
			mScale.x *= scale.x;
			mScale.y *= scale.y;
		}

		float get_radians()const noexcept { return mRadians; }
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