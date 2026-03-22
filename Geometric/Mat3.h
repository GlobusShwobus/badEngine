#pragma once

#include "Float2.h"

namespace bad
{
	struct Mat3
	{
		// | Ax  Bx  Tx |
		// | Ay  By  Ty |
		// |  0   0   1 |

		float Ax = 1, Bx = 0, Tx = 0;
		float Ay = 0, By = 1, Ty = 0;


		constexpr bad::Point operator*(const bad::Point& v)const noexcept
		{
			return bad::Point{ 
				Ax * v.x + Bx * v.y + Tx,  //Tx * 1 since the third row is implied
				Ay * v.x + By * v.y + Ty   //Ty * 1 since the third row is implied
			};
		}

		constexpr Mat3 operator*(const bad::Mat3& rhs)const noexcept
		{
			Mat3 m;

			m.Ax = Ax * rhs.Ax + Bx * rhs.Ay;
			m.Ay = Ay * rhs.Ax + By * rhs.Ay;

			m.Bx = Ax * rhs.Bx + Bx * rhs.By;
			m.By = Ay * rhs.Bx + By * rhs.By;

			m.Tx = Ax * rhs.Tx + Bx * rhs.Ty + Tx;
			m.Ty = Ay * rhs.Tx + By * rhs.Ty + Ty;

			return m;
		}

		constexpr Mat3& operator*=(const bad::Mat3& rhs) noexcept
		{
			return *this = *this * rhs;
		}

		constexpr static Mat3 identity() noexcept
		{
			return Mat3{};
		}

		constexpr static Mat3 scale(float factor) noexcept
		{
			Mat3 m;
			m.Ax = factor;
			m.By = factor;
			return m;
		}

		//assumes sin cos are pre calculated
		constexpr static Mat3 rotate(float sin, float cos) noexcept
		{
			Mat3 m;
			m.Ax = cos;  m.Ay = sin;
			m.Bx = -sin; m.By = cos;
			return m;
		}

		constexpr static Mat3 translate(float x, float y) noexcept
		{
			Mat3 m;
			m.Tx = x;
			m.Ty = y;
			return m;
		}

		constexpr static Mat3 flip_y() noexcept
		{
			Mat3 m;
			m.By = -1;
			return m;
		}
	};
}