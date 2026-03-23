#pragma once

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
		// | Ax  Bx  Tx |
		// | Ay  By  Ty |
		// |  0   0   1 |

		float Ax = 1, Bx = 0, Tx = 0;
		float Ay = 0, By = 1, Ty = 0;


		constexpr bad::Point operator*(const bad::Point& v)const noexcept
		{
			// Point is 2X but for correct matrix math it is required for the Vector to be 3 rows long
			// To address this the third row is implied, and is always 1 which means just add +Tx/Ty at the end.
			// IMPORTANT: when upgrading to 3D graphics this is no longer correct implementation. It should become Mat4 and real 3D vector gets a 4 (W) implied value
			return bad::Point{ 
				Ax * v.x + Bx * v.y + Tx,  //Tx * 1 since the third row is implied
				Ay * v.x + By * v.y + Ty   //Ty * 1 since the third row is implied
			};
		}

		constexpr Mat3 operator*(const bad::Mat3& rhs)const noexcept
		{
			Mat3 m;

			m.Ax = Ax * rhs.Ax + Bx * rhs.Ay      /* Tx * m31 */; // third row is implied and always 0
			m.Bx = Ax * rhs.Bx + Bx * rhs.By      /* Tx * m32 */; // third row is implied and always 0
			m.Tx = Ax * rhs.Tx + Bx * rhs.Ty + Tx /* Tx * m33 */; // third row is implied and always 1

			m.Ay = Ay * rhs.Ax + By * rhs.Ay      /* Ty * m31 */; // third row is implied and always 0
			m.By = Ay * rhs.Bx + By * rhs.By      /* Ty * m32 */; // third row is implied and always 0
			m.Ty = Ay * rhs.Tx + By * rhs.Ty + Ty /* Ty * m33 */; // third row is implied and always 1

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
			m.Ax = sx;
			m.By = sy;
			return m;
		}

		constexpr static Mat3 scale(float factor) noexcept
		{
			return Mat3::scale(factor, factor);
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
			m.Ax = cos;  m.Ay = sin;
			m.Bx = -sin; m.By = cos;
			return m;
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
		constexpr static Mat3 translate(float x, float y) noexcept
		{
			Mat3 m;
			m.Tx = x;
			m.Ty = y;
			return m;
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
			m.By = -1;
			return m;
		}
	};
}