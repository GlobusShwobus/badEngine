#pragma once

#include "Float2.h"
#include "Mat3.h"
#include "MathConstants.h"

namespace bad
{
	class Translation final
	{
	public:
		explicit Translation(float x, float y) :mPos(x, y) {}
		Translation() :Translation(0.f,0.f) {}
		explicit Translation(const bad::Point& p) :Translation(p.x,p.y) {}

		constexpr const bad::Point& get_pos()const noexcept { return mPos; }

		constexpr void set_pos(float x, float y)noexcept { mPos.x = x; mPos.y = y; }

		constexpr void set_pos(const bad::Point& p)noexcept { set_pos(p.x, p.y); }

		constexpr void translate_by(float x, float y)noexcept { mPos.x += x; mPos.y += y; }

		constexpr void translate_by(const bad::Point& p)noexcept { translate_by(p.x, p.y); }

		constexpr Mat3 to_matrix()const noexcept
		{
			return Mat3::translate(mPos);
		}

	private:
		bad::Point mPos;
	};

	class Rotation final
	{
	public:
		Rotation() :mRad(0), mSin(0), mCos(1) {}
		explicit Rotation(float radians) { set_radian(radians); }

		inline void set_radian(float r)noexcept
		{
			mRad = std::fmod(r, bad::TAU);

			if (mRad < 0)
				mRad += bad::TAU;

			mSin = std::sin(mRad);
			mCos = std::cos(mRad);
		}

		void rotate_by(float r)noexcept { set_radian(mRad + r); }

		constexpr float get_radian()const noexcept { return mRad; }
		constexpr float get_sin() const noexcept   { return mSin; }
		constexpr float get_cos() const noexcept   { return mCos; }

		constexpr Mat3 to_matrix()const noexcept
		{
			return Mat3::rotate(mSin, mCos);
		}

	private:
		float mRad;
		float mSin;
		float mCos;
	};


	class Scale final
	{
	public:
		Scale() :mScaleX(1.0f), mScaleY(1.0f) {}
		explicit Scale(float sx, float sy) :mScaleX(sx), mScaleY(sy) {}
		explicit Scale(float scale) :Scale(scale, scale) {}
		explicit Scale(const bad::float2& scale) :Scale(scale.x, scale.y) {}

		constexpr void set_scale(float sx, float sy)noexcept { mScaleX = sx; mScaleY = sy; }
		constexpr void set_scale(float factor)noexcept { set_scale(factor, factor); }
		constexpr void set_scale(const bad::float2& scale)noexcept { set_scale(scale.x, scale.y); }

		constexpr void scale_by(float fx, float fy)noexcept { mScaleX *= fx; mScaleY *= fy; }
		constexpr void scale_by(float fx)noexcept { scale_by(fx, fx); }
		constexpr void scale_by(const bad::float2& scale)noexcept { scale_by(scale.x, scale.y); }

		constexpr float get_sx()const noexcept { return mScaleX; }
		constexpr float get_sy()const noexcept { return mScaleY; }
		constexpr const bad::float2& get_scale()const noexcept { return { mScaleX,mScaleY }; }

		constexpr Mat3 to_matrix()const noexcept
		{
			return Mat3::scale(mScaleX, mScaleY);
		}

	private:
		float mScaleX;
		float mScaleY;
	};
}