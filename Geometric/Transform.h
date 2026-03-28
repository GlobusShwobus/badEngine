#pragma once

#include "Float2.h"
#include "Mat3.h"
#include "MathConstants.h"

namespace bad
{
	/**
	* \brief
	* Transform is basically just a more human readable version of bad::Mat3.
	* 
	* Transform manages the position, rotation and scale of the object providing concrete getters.
	* 
	* A Transform should be used for managing screen position, but it should not be used as a parameter for modification of points.
	* That is Mat3's job.
	*/
	class Transform
	{
	public:
		/// <summary> Default initalizes transform. </summary>
		Transform()
			: mPos(0.f, 0.f)
			, mScale(1.f)
			, mRadians(0.f)
			, mSin(0.f)
			, mCos(1.f)
		{
		}
		/// <summary>
		/// Initializes Transform with the given pos, scale and radian.
		/// </summary>
		Transform(const bad::Point& pos, float scale, float radian)
			:mPos(pos)
			, mScale(scale)
		{
			set_radian(radian);
		}

		/// <returns> Returns transformation to Mat3 with T * R * S </returns>
		inline Mat3 to_matrix()const noexcept
		{
			return Mat3::translate(mPos) * Mat3::rotate(mSin, mCos) * Mat3::scale(mScale);
		}

		/// <returns> Returns inverse transformation to Mat3 with S * R * T</returns>
		inline Mat3 to_inverse_matrix()const noexcept
		{
			return Mat3::scale(1 / mScale, 1 / mScale) * Mat3::rotate(-mSin, mCos) * Mat3::translate(-mPos);
		}

		/// <summary>
		/// Pos = new pos;
		/// </summary>
		/// <param name="pos"> new pos </param>
		void set_position(const bad::Vector& new_pos)noexcept
		{
			mPos = new_pos;
		}

		/// <summary>
		/// Pos += offset;
		/// </summary>
		/// <param name="offset"> offset </param>
		void move_by(const bad::Vector& offset)noexcept
		{
			mPos += offset;
		}

		/// <summary>
		/// Radian = new radian;
		/// </summary>
		/// <param name="ranew_radiandian"> new_radian </param>
		void set_radian(float new_radian)noexcept
		{
			mRadians = std::fmod(new_radian, bad::TAU);

			if (mRadians < 0)
				mRadians += bad::TAU;

			mSin = std::sin(mRadians);
			mCos = std::cos(mRadians);
		}

		/// <summary>
		/// Radian += offset;
		/// </summary>
		/// <param name="offset"> offset </param>
		void rotate_by(float offset)noexcept
		{
			set_radian(mRadians + offset);
		}

		/// <summary>
		/// Scale = new_scale;
		/// </summary>
		/// <param name="new_scale"> new_scale </param>
		void set_scale(float new_scale)noexcept
		{
			mScale = new_scale;
		}

		/// <summary>
		/// Scale *= factor;
		/// </summary>
		/// <param name="factor"> factor </param>
		void scale_by(float factor)noexcept
		{
			mScale *= factor;
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