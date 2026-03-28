 #pragma once
#include <cmath>
#include "Float2.h"

namespace bad
{
	/**
	* \brief
	* Represents a 2D affine transformation using a compact 3x3 matrix.
	*
	* To make translation operation fit into the matrix, the third row is implied  0 0 1.
	* 
	* To follow matrix multiplication rules a 2D vector is implied to have a third element, always 1.
	*/
	struct Mat3
	{
		// | m00  m01  m02 |	   | m00  m01  m02 |
		// | m10  m11  m12 |	   | m10  m11  m12 |
		// |  0    0    1  |       |  0    0    1  |

		float m00 = 1, m01 = 0, m02 = 0;
		float m10 = 0, m11 = 1, m12 = 0;


		constexpr bad::Point operator*(const bad::Point& v)const noexcept
		{
			// third element if the 2D vector is implied  = 1
			return bad::Point{ 
				m00 * v.x + m01 * v.y + m02,
				m10 * v.x + m11 * v.y + m12 
			};
		}

		constexpr Mat3 operator*(const bad::Mat3& rhs)const noexcept
		{
			Mat3 m;

			m.m00 = m00 * rhs.m00 + m01 * rhs.m10       /* m02 * 0 */; // third row is implied and always 0
			m.m01 = m00 * rhs.m01 + m01 * rhs.m11       /* m02 * 0 */; // third row is implied and always 0
			m.m02 = m00 * rhs.m02 + m01 * rhs.m12 + m02 /* m02 * 1 */; // third row is implied and always 1

			m.m10 = m10 * rhs.m00 + m11 * rhs.m10        /* m12 * 0 */; // third row is implied and always 0
			m.m11 = m10 * rhs.m01 + m11 * rhs.m11        /* m12 * 0 */; // third row is implied and always 0
			m.m12 = m10 * rhs.m02 + m11 * rhs.m12 + m12  /* m12 * 1 */; // third row is implied and always 1

			return m;
		}

		constexpr Mat3& operator*=(const bad::Mat3& rhs) noexcept
		{
			return *this = *this * rhs;
		}

		/**
		*\returns
		* returns the identity matrix:
		*
		* |__1__0__0__|
		* 
		* |__0__1__0__|
		* 
		* |__0__0__1__|
		*/
		constexpr static Mat3 identity() noexcept
		{
			return Mat3{};
		}

		/**
		 * \param sx Scale factor along the X axis
		 * \param sy Scale factor along the Y axis
		 *
		 * \returns 
		 * retruns a matrix with scaling info
		 * 
		 * |__sx__0__0__|
		 * 
		 * |__0__sy__0__|
		 * 
		 * |__0__0__1___|
		 */
		constexpr static Mat3 scale(float sx, float sy) noexcept
		{
			Mat3 m;
			m.m00 = sx;
			m.m11 = sy;
			return m;
		}

		constexpr static Mat3 scale(float factor) noexcept
		{
			return Mat3::scale(factor, factor);
		}

		constexpr static Mat3 scale(const bad::float2& scale) noexcept
		{
			return Mat3::scale(scale.x, scale.y);
		}

		/**
		* \param sin precalculated sin from radians
		* \param cos precalculated cos from radians
		*
		* \returns
		* returns a matrix with rotation info
		* 
		* |__cos__-sin___0__|
		* 
		* |__sin___cos___0__|
		* 
		* |___0_____0____1__|
		*/
		constexpr static Mat3 rotate(float sin, float cos) noexcept
		{
			Mat3 m;
			m.m00 = cos;  m.m01 = -sin;
			m.m10 = sin;  m.m11 = cos;
			return m;
		}

		constexpr static Mat3 rotate(float radian) noexcept
		{
			return rotate(std::sin(radian), std::cos(radian));
		}

		/**
		* \param tx Translation along the X axis
		* \param ty Translation along the Y axis
		*
		* \returns
		* retruns a matrix with translation info
		* 
		* |__1__0__tx__|
		* 
		* |__0__1__ty__|
		* 
		* |__0__0__1___|
		*/
		constexpr static Mat3 translate(float tx, float ty) noexcept
		{
			Mat3 m;
			m.m02 = tx;
			m.m12 = ty;
			return m;
		}

		constexpr static Mat3 translate(const bad::Point& p) noexcept
		{
			return Mat3::translate(p.x, p.y);
		}

		/**
		* \returns
		* returns a matrix that flips the y
		* 
		* |__1___0__0__|
		* 
		* |__0__-1__0__|
		* 
		* |__0___0__1__|
		*/
		constexpr static Mat3 flip_y() noexcept
		{
			Mat3 m;
			m.m11 = -1;
			return m;
		}
	};
}