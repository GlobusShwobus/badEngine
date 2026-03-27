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
			, mScale(1.f)
			, mRadians(0.f)
			, mSin(0.f)
			, mCos(1.f)
		{
		}

		Transform(const bad::Point& pos, float scale, float radian)
			:mPos(pos)
			, mScale(scale)
		{
			set_radian(radian);
		}

		inline Mat3 to_matrix()const noexcept
		{
			return Mat3::translate(mPos) * Mat3::rotate(mSin, mCos) * Mat3::scale(mScale);
		}

		inline Mat3 to_inverse_matrix()const noexcept
		{
			return Mat3::scale(1 / mScale, 1 / mScale) * Mat3::rotate(-mSin, mCos) * Mat3::translate(-mPos);
		}


		void set_position(const bad::Vector& pos)noexcept
		{
			mPos = pos;
		}

		//does += pos internally
		void move_by(const bad::Vector& vel)noexcept
		{
			mPos += vel;
		}

		void set_radian(float radian)noexcept
		{
			mRadians = std::fmod(radian, bad::TAU);

			if (mRadians < 0)
				mRadians += bad::TAU;

			mSin = std::sin(mRadians);
			mCos = std::cos(mRadians);
		}

		//does += radians internally
		void rotate_by(float radian)noexcept
		{
			set_radian(mRadians + radian);
		}

		void set_scale(float scale)noexcept
		{
			mScale = scale;
		}

		//unlike others, this function does *= internally
		void scale_by(float scale)noexcept
		{
			mScale *= scale;
		}

		const bad::Point& get_pos()const noexcept { return mPos; }
		float get_scale()const noexcept { return mScale; }
		float get_radian()const noexcept { return mRadians; }
		float get_sin()const noexcept { return mSin; }
		float get_cos()const noexcept { return mCos; }


	private:
		bad::Point mPos; 		/// <summary> Position (translation) </summary>

		float mScale;			/// <summary> Scaling factors </summary>

		float mRadians; 		/// <summary> Rotation angle in radians </summary>

		float mSin; 			/// <summary> Cached sine value of rotation </summary>

		float mCos; 			/// <summary> Cached cosine value of rotation </summary>
	};
}