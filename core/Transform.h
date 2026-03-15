#pragma once

#include "Float2.h"
#include "Matrix3.h"

namespace badCore
{
	/**
	* Transform represents a 2D geometric transformation.
	*
	* A Transform combines the three basic operations used to position objects
	* in the world:
	*
	*  - Translation (position)
	*  - Rotation (in radians)
	*  - Uniform scaling
	*
	* Translation is represented by a float2 coordinate.
	* Scaling is represented by a single float and is applied uniformly on both axes.
	*
	* All rotations in the engine are measured in radians.
	*
	*
	* --- IMPORTANT PERFORMANCE NOTE ---
	*
	* This class intentionally does NOT compute sin() and cos() automatically.
	*
	* Trigonometric functions are relatively expensive, and automatically
	* recalculating them every time the transform is used would introduce
	* unnecessary overhead in many cases.
	*
	* Instead, the engine requires the user to explicitly call:
	*
	*     Transform::update_sincos()
	*
	* whenever the rotation changes.
	*
	* This design provides two performance advantages:
	*
	* 1. Objects that never rotate will never recompute sin/cos (but do require at least one call to update).
	* 2. Objects that rotate multiple times per frame only need to compute
	*    sin/cos once before the transform matrix is used.
	*
	*
	* --- WARNING ---
	*
	* The constructor does NOT initialize the sine and cosine values.
	*
	* Using transform() or transform_inverse() before calling update_sincos()
	* after construction or rotation change results in undefined behavior.
	*
	*
	* --- Transformation Order ---
	*
	* The transform matrix is constructed in the following order:
	*
	*     Translation * Rotation * Scale
	*
	* meaning objects are:
	*
	*     1. Scaled
	*     2. Rotated
	*     3. Translated
	*
	* This ordering matches common object-to-world transformation conventions.
	*/
	class Transform 
	{
	public:

		/**
		* Creates a Transform with the specified position, scale and rotation.
		*
		* \param pos world position
		* \param scale uniform scaling factor
		* \param radians rotation angle in radians
		*
		* \note This constructor does NOT compute sine/cosine values.
		*       update_sincos() must be called before the transform is used.
		*/
		Transform(const Point& pos, float scale, float radians);

		/**
		* \returns Returns the transformation matrix representing this transform.
		*/
		Mat3 transform()const noexcept;

		/**
		* \returns Returns the inverse transformation matrix.
		*/
		Mat3 transform_inverse()const noexcept;

		/**
		* Sets the rotation angle in radians.
		*
		* \note update_sincos() must be called after modifying the rotation.
		*/
		void set_rotation(float radians)noexcept;

		/**
		* Sets the rotation angle in radians and calls update_sincos()
		*/
		void set_rotation_and_update(float radians) noexcept;

		/**
		* Updates cached sine and cosine values from the current rotation.
		*
		* This function must be called whenever the rotation changes before
		* the transform matrix is used.
		*/
		void update_sincos()noexcept;

		/**
		* Returns the current rotation in radians.
		*/
		constexpr float get_radians()const noexcept
		{
			return mRadians;
		}

		/// <summary> Position (translation) </summary>
		Point mPos;

		/// <summary> Uniform scaling factor </summary>
		float mScale;

	private:
		/// <summary> Rotation angle in radians </summary>
		float mRadians;

		/// <summary> Cached sine value of rotation </summary>
		float mSin;

		/// <summary> Cached cosine value of rotation </summary>
		float mCos;
	};
}