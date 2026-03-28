#pragma once

#include <cmath>

namespace bad
{
	/**
	* \brief
	* bad::float2 is literally just a pair of floats.
	* 
	* Mainly is intended to be a representation of a vector, supporting vector arithmetic.
	* 
	* Though not 100% mathmatically correct, it float2 also represents a point.
	* 
	* 
	* typedefs:
	*
	* bad::Vector = bad::float2
	* 
	* bad::Point = bad::float2
	*/
	class float2 final
	{
	public:

		float x;
		float y;

		/// <summary> Default constructs with 0s  </summary>
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
		constexpr explicit float2(float X, float Y)noexcept
			:x(X), y(Y)
		{
		}

		/**
		* Constructs float2 from a given value for both x and y
		*
		* \param v for value;
		*/
		constexpr explicit float2(float v)noexcept
			:x(v), y(v)
		{
		}

		constexpr float2 operator+    (const float2& rhs) const noexcept { return float2{ x + rhs.x,  y + rhs.y }; }
		constexpr float2 operator-    (const float2& rhs) const noexcept { return float2{ x - rhs.x,  y - rhs.y }; }
		constexpr float2& operator+=  (const float2& rhs) noexcept       { x += rhs.x;  y += rhs.y;  return *this; }
		constexpr float2& operator-=  (const float2& rhs) noexcept       { x -= rhs.x;  y -= rhs.y;  return *this; } 

		constexpr float2 operator+    (float scalar) const noexcept      { return float2{ x + scalar, y + scalar }; }
		constexpr float2 operator-    (float scalar) const noexcept      { return float2{ x - scalar, y - scalar }; }
		constexpr float2 operator*    (float scalar) const noexcept      { return float2{ x * scalar, y * scalar }; }
		constexpr float2 operator/    (float scalar) const noexcept      { return float2{ x / scalar, y / scalar }; }

		constexpr float2& operator+=  (float scalar) noexcept            { x += scalar; y += scalar; return *this; }
		constexpr float2& operator-=  (float scalar) noexcept            { x -= scalar; y -= scalar; return *this; }
		constexpr float2& operator*=  (float scalar) noexcept            { x *= scalar; y *= scalar; return *this; }
		constexpr float2& operator/=  (float scalar) noexcept            { x /= scalar; y /= scalar; return *this; }

		constexpr float2 operator-    ()const noexcept { return float2{ -x, -y }; }
		constexpr bool operator ==    (const float2& rhs)const noexcept { return x == rhs.x && y == rhs.y; }
		constexpr bool operator!=     (const float2& rhs)const noexcept { return !(*this == rhs); }
	};

	constexpr float2 operator+(float scalar, const float2& v) noexcept { return float2{ scalar + v.x, scalar + v.y }; }
	constexpr float2 operator-(float scalar, const float2& v) noexcept { return float2{ scalar - v.x, scalar - v.y }; }
	constexpr float2 operator*(float scalar, const float2& v) noexcept { return float2{ scalar * v.x, scalar * v.y }; }

	using Point = float2;
	using Vector = float2;

	/// <returns> the squared length of the vector </returns>
	constexpr float length_sq(const Vector& vec) noexcept
	{
		return (vec.x * vec.x) + (vec.y * vec.y);
	}

	/// <returns> the length of the vector </returns>
	inline float length(const Vector& vec) noexcept
	{
		return std::sqrt(length_sq(vec));
	}

	/**
	* Normalizes the given vector v with its length.
	* 
	* Assumption is that length is cached by the user so this method avoid doing a square root.
	* 
	* It is also assumed length is of the vector, not some other vector.
	* 
	* \param v for vector to normalize
	* \param pre_calculated_length for denominator
	* 
	* \returns normalized vector
	*/
	constexpr Vector get_normalized(const Vector& v, float pre_calculated_length) noexcept
	{
		return Vector{ v.x / pre_calculated_length ,v.y / pre_calculated_length };
	}

	/**
	* Normalizes the given vector v.
	* 
	* \param v for vector to normalize
	*
	* \returns normalized vector
	*/
	inline float2 get_normalized(const Vector& vec) noexcept
	{
		return get_normalized(vec, length(vec));
	}

	/// <summary> Normalizes the vector in-place (modifies the given vec). </summary>
	inline void normalize(Vector& vec)noexcept
	{
		auto len = length(vec);
		vec.x /= len;
		vec.y /= len;
	}

	/// <returns> Returns a vector rotated 90 degrees counter-clockwise (cartesian). </returns>
	constexpr Vector perpendicular(const Vector& v) noexcept
	{
		return Vector{ v.y, -v.x };
	}

	/// <returns> Returns the dot product of two vectors. </returns>
	constexpr float dot_product(const Vector& a, const Vector& b)noexcept
	{
		return  (a.x * b.x) + (a.y * b.y);
	}

	/**
	* Computes how much vector v travels along a surface (the shadow or projection).
	*
	* \param v Vector to be projected.
	* \param surface_normal Normalized surface direction.
	* 
	* \returns ( v * surface_normal ) * surface_normal
	*/
	constexpr Vector projection(const Vector& v, const Vector& surface_normal)noexcept
	{
		return dot_product(v, surface_normal) * surface_normal;
	}

	/**
	* Computes a reflected vector v against a surface.
	*
	* \param v vector to be reflected.
	* \param surface_normal Normalized surface direction
	* 
	* \returns v - [ 2.0f * ( v * surface_normal ) * surface_normal ]
	*/
	constexpr Vector reflection(const Vector& v, const Vector& surface_normal)noexcept
	{
		return v - (2.0f * projection(v, surface_normal));
	}

	/// <returns> retruns a float2 with -1, 0, 1 for x and y values representing the sign. </returns>
	constexpr float2 sign(const float2& f2) noexcept
	{
		return float2{
			static_cast<float>((f2.x > 0.0f) - (f2.x < 0.0f)),
			static_cast<float>((f2.y > 0.0f) - (f2.y < 0.0f))
		};
	}

	/**
	* \brief
	* Finds the closest point of target against some surface. The closest point is clamped to either end of the surface.
	* 
	* This function is the most cannonical version. It assumes all parameters are related, otherwise it can not produce a correct output.
	* 
	* \param surface_origin the origin point of the surface vector
	* \param surface_normal the direction of the surface vector
	* \param surface length the lenght of the surface vector
	* 
	* \param target the point against which to find the closest point on the surface
	* 
	* \returns the closest point on the surface
	*/
	constexpr Point closest_point(const Point& surface_origin, const Vector& surface_normal, float surface_length, const Point& target) noexcept
	{
		Vector toPoint = target - surface_origin;
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

	/**
	* Using simplified trig identity sin(a+-b) and cos(a+-b) rotates given vector v.
	* 
	* This is functions assumes sinTheta and cosTheta are pre comupted.
	* 
	* \param v the vector to rotate
	* \param sinTheta change on y axis
	* \param cosTheta change on x axis
	* 
	* \retruns rotated vector v
	*/
	constexpr Vector rotate(const Vector& v, float sinTheta, float cosTheta)noexcept
	{
		float rx = v.x * cosTheta - v.y * sinTheta;
		float ry = v.x * sinTheta + v.y * cosTheta;
		return Vector{ rx,ry };
	}

	/// <returns> Returns rotated vector v with a given angle in radian format. </returns>
	inline Vector rotate(const Vector& v, float radian)noexcept
	{
		return rotate(v, std::sin(radian), std::cos(radian));
	}
}