#pragma once

#include <cmath>

namespace bad
{
	/**
	* \brief float2 is literally just a pair of floats.
	*
	* In most if not all cases float2 is used as either a vector or a point on the coordinate plane.
	*
	* Since a 'point' != 'vector' but separating them would produce a lot of headache,
	* the decision is to only define arithmetic operators in class.
	*
	* Outside the class additional typedefs for 'Point' and 'Vector' are provided.
	*
	* Arithmetic operators perform component-wise operations.
	*/
	class float2 final
	{
	public:

		float x;
		float y;

		/// <summary> Default constructs float2 {0.0f, 0.0f}  </summary>
		constexpr float2()noexcept
			:x(0.0f), y(0.0f)
		{
		}

		/**
		* Constructs float2 from given floats.
		*
		* \param X for x
		* \param Y for y
		*/
		constexpr float2(float X, float Y)noexcept
			:x(X), y(Y)
		{
		}

		/**
		* Constructs float2 from a given float
		*
		* \param x = v;
		* \param y = v;
		*/
		constexpr explicit float2(float v)noexcept
			:x(v), y(v)
		{
		}

		constexpr float2 operator+    (const float2& rhs) const noexcept { return { x + rhs.x,  y + rhs.y }; }
		constexpr float2 operator-    (const float2& rhs) const noexcept { return { x - rhs.x,  y - rhs.y }; }
		constexpr float2& operator+=  (const float2& rhs) noexcept       { x += rhs.x;  y += rhs.y;  return *this; }
		constexpr float2& operator-=  (const float2& rhs) noexcept       { x -= rhs.x;  y -= rhs.y;  return *this; } 

		constexpr float2 operator+    (float scalar) const noexcept      { return { x + scalar, y + scalar }; }
		constexpr float2 operator-    (float scalar) const noexcept      { return { x - scalar, y - scalar }; }
		constexpr float2 operator*    (float scalar) const noexcept      { return { x * scalar, y * scalar }; }
		constexpr float2 operator/    (float scalar) const noexcept      { return { x / scalar, y / scalar }; }

		constexpr float2& operator+=  (float scalar) noexcept            { x += scalar; y += scalar; return *this; }
		constexpr float2& operator-=  (float scalar) noexcept            { x -= scalar; y -= scalar; return *this; }
		constexpr float2& operator*=  (float scalar) noexcept            { x *= scalar; y *= scalar; return *this; }
		constexpr float2& operator/=  (float scalar) noexcept            { x /= scalar; y /= scalar; return *this; }

		constexpr float2 operator-    ()const noexcept { return { -x, -y }; }
		constexpr bool operator ==    (const float2& rhs)const noexcept { return x == rhs.x && y == rhs.y; }
		constexpr bool operator!=     (const float2& rhs)const noexcept { return !(*this == rhs); }
	};

	constexpr float2 operator+(float scalar, const float2& v) noexcept { return { scalar + v.x, scalar + v.y }; }
	constexpr float2 operator-(float scalar, const float2& v) noexcept { return { scalar - v.x, scalar - v.y }; }
	constexpr float2 operator*(float scalar, const float2& v) noexcept { return { scalar * v.x, scalar * v.y }; }

	using Point = float2;
	using Vector = float2;

	/**
	* \retruns the squared lenght (magnitude^2) of the vector
	*/
	constexpr float length_sq(const Vector& vec) noexcept
	{
		return (vec.x * vec.x) + (vec.y * vec.y);
	}

	/**
	* \returns the length of the vector.
	*/
	inline float length(const Vector& vec) noexcept
	{
		return std::sqrt(length_sq(vec));
	}

	/**
	* Returns a normalized copy of the vector using a precomputed magnitude.
	*
	* This avoids recomputing the length of the vector when it is already known,
	* which can improve performance in tight math or physics loops.
	*
	* \param v Vector to normalize.
	* \param pre_calculated_magnitude Length of the vector.
	* \return Normalized vector.
	*
	* \note Behavior is undefined if the magnitude is zero.
	*/
	constexpr Vector get_normalized(const Vector& v, float pre_calculated_magnitude) noexcept
	{
		return { v.x / pre_calculated_magnitude ,v.y / pre_calculated_magnitude };
	}

	/**
	* Returns a normalized copy of the vector.
	*
	* The returned vector has a length of 1 while preserving direction.
	* The original vector is not modified.
	*
	* \returns copy of this vector normalized
	*
	* \note Behavior is undefined if the vector length is zero.
	*/
	inline float2 get_normalized(const Vector& vec) noexcept
	{
		return get_normalized(vec, length(vec));
	}

	/**
	* Normalizes the vector in-place.
	*
	* After the operation the vector will have length 1.
	*
	* \returns this vector by referenec
	* \note Behavior is undefined if the vector length is zero.
	*/
	inline void normalize(Vector& vec)noexcept
	{
		auto len = length(vec);
		vec.x /= len;
		vec.y /= len;
	}

	/**
	* Returns a vector perpendicular to the input vector rotated
	* 90 degrees counter-clockwise.
	*
	* If v = (x, y), the result is (-y, x).
	*
	* \param v Input vector.
	* \return Counter-clockwise perpendicular vector.
	*/
	constexpr Vector perpendicular(const Vector& v) noexcept
	{
		return { -v.y, v.x };
	}

	/**
	* Computes the dot product of two vectors.
	*
	* The dot product measures how aligned two vectors are and is commonly
	* used for projections, reflections, and angle tests.
	*
	* \param a First vector.
	* \param b Second vector.
	* \return Scalar dot product.
	*/
	constexpr float dot_product(const Vector& a, const Vector& b)noexcept
	{
		return  (a.x * b.x) + (a.y * b.y);
	}

	/**
	* Computes the projection (shadow) of a vector onto a surface normal.
	*
	* Other way of thinking about it is: get the length (dot) of v1*v2, and revectorize it by lenght * vector which produces a vector.
	* 
	* The surface normal is expected to be normalized.
	*
	* \param v Vector to be projected.
	* \param surface_normal Normalized surface normal.
	* \return Projected vector.
	*/
	constexpr Vector projection(const Vector& v, const Vector& surface_normal)noexcept
	{
		return dot_product(v, surface_normal) * surface_normal;
	}

	/**
	* Computes the reflection vector of v against some normal (surface).
	*
	* The surface normal is expected to be normalized because by this point the user ought to already have a normal cached anyway.
	*
	* \param v vector to be reflected.
	* \param surface_normal some surface/line segment/other.
	* \return Reflected vector by value.
	*/
	constexpr Vector reflection(const Vector& v, const Vector& surface_normal)noexcept
	{
		return v - (2.0f * projection(v, surface_normal));
	}

	/**
	* \brief Returns a vector containing the sign of each component.
	*
	*
	* \returns returns flaot2 with following values for x and y:
	*
	*
	*  1 if positive
	*
	* -1 if negative
	*
	*  0 if zero
	*/
	constexpr float2 sign(const float2& f2) noexcept
	{
		return {
			static_cast<float>((f2.x > 0.0f) - (f2.x < 0.0f)),
			static_cast<float>((f2.y > 0.0f) - (f2.y < 0.0f))
		};
	}

	constexpr bad::Point closest_point(const bad::Point& surface_origin, const bad::Vector& surface_normal, float surface_length, const bad::Point& target) noexcept
	{
		bad::Vector toPoint = target - surface_origin;
		float projection = dot_product(toPoint, surface_normal);

		//clamp
		if (projection < 0.0f) {
			projection = 0.0f;
		}
		else if (projection > surface_length) {
			projection = surface_length;
		}

		return surface_origin + surface_normal * projection;
	}
}