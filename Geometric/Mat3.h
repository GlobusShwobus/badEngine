#pragma once

#include "Float2.h"

namespace bad
{
	struct Mat3
	{
		float a = 1, b = 0;
		float c = 0, d = 1;
		float tx = 0, ty = 0;

		constexpr Mat3 operator*(const Mat3& rhs)const noexcept
		{
			Mat3 r;
			r.a = a * rhs.a + b * rhs.c;
			r.b = a * rhs.b + b * rhs.d;
			r.c = c * rhs.a + d * rhs.c;
			r.d = c * rhs.b + d * rhs.d;

			r.tx = a * rhs.tx + b * rhs.ty + tx;
			r.ty = c * rhs.tx + d * rhs.ty + ty;
			return r;
		}

		constexpr float2 transform(const float2& v)const noexcept
		{
			return {
				a * v.x + b * v.y + tx,
				c * v.x + d * v.y + ty
			};
		}

		constexpr static Mat3 translation(float x, float y) noexcept
		{
			Mat3 m;
			m.tx = x;
			m.ty = y;
			return m;
		}

		constexpr static Mat3 scale(float sx, float sy)noexcept
		{
			Mat3 m;
			m.a = sx;
			m.d = sy;
			return m;
		}

		constexpr static Mat3 rotation(float s, float c)noexcept
		{
			Mat3 m;
			m.a = c;
			m.b = -s;
			m.c = s;
			m.d = c;

			return m;
		}
	};
}