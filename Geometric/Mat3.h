#pragma once
#include <cmath>
#include "Float2.h"

namespace bad
{
	/**
	* \brief
	* Represents a 2D affine transformation using a compact 3x3 matrix where the third row is implied (0, 0, 1) but not stored.
	* 
	* The full matrix layout is:
	* 
	* | Ax  Bx  Tx |
	* 
	* | Ay  By  Ty |
	* 
	* |  0   0   1 |
	* 
	* - First column  (Ax, Ay) -> transformed X axis
	* 
	* - Second column (Bx, By) -> transformed Y axis
	* 
	* - Third column  (Tx, Ty) -> translation
	* 
	* NOTE:: This is a mathmatical operation NOT a screen operation. For example rotation clounter clockwise is the same as clockwise on the screen.
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
			// Point is 2X but for correct matrix math it is required for the Vector to be 3 rows long
			// To address this the third row is implied, and is always 1 which means just add +Tx/Ty at the end.
			// IMPORTANT: when upgrading to 3D graphics this is no longer correct implementation. It should become Mat4 and real 3D vector gets a 4 (W) implied value
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
		 * \brief Returns the identity matrix.
		 *
		 * The identity matrix leaves vectors unchanged:
		 * 
		 * x' = x, y' = y
		 */
		constexpr static Mat3 identity() noexcept
		{
			return Mat3{};
		}

		/**
		 * \brief Creates a scaling transformation.
		 *
		 * Scaling modifies the basis vectors X and Y or column 1 and column 2 where values in identity were 1.
		 * 
		 * This results in a matrix:
		 * 
		 * | sx   0   0 |
		 * 
		 * |  0  sy   0 |
		 * 
		 * |  0   0   1 |
		 *
		 * \param sx Scale factor along the X axis
		 * \param sy Scale factor along the Y axis
		 *
		 * \return A matrix that scales points by (sx, sy)
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
		* \brief Creates a rotation transformation.
		*
		* Rotates points around the origin by the given precalculated sin and cos from a radian.
		*
		* This results in a matrix:
		* 
		* |  cos  -sin   0 |
		* 
		* |  sin   cos   0 |
		* 
		* |   0    0     1 |
		*
		* \param sin precalculated sin from radians
		* \param cos precalculated cos from radians
		*
		* \return A matrix that rotates points around the origin
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
		* \brief Creates a translation transformation.
		*
		* Translation offsets all points by (tx, ty) without affecting rotation or scale.
		*
		* This results in a matrix:
		* 
		* | 1   0   tx |
		* 
		* | 0   1   ty |
		* 
		* | 0   0   1  |
		*
		* \param tx Translation along the X axis
		* \param ty Translation along the Y axis
		*
		* \return A matrix that translates points by (tx, ty)
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
		* \brief
		* Flips the Y-axis basis y resulting in literally fliping something upside down.
		* \returns 
		* | 1   0   0 |
		* 
		* | 0  -1   0 |
		* 
		* | 0   0   1 |
		*/
		constexpr static Mat3 flip_y() noexcept
		{
			Mat3 m;
			m.m11 = -1;
			return m;
		}
	};
}