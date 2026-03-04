#pragma once

#include <cmath>

namespace badCore 
{
	class float2 final
	{
	public:

		float x;
		float y;

		constexpr float2()noexcept 
			:x(0.f), y(0.f)
		{
		}
		constexpr float2(float X, float Y)noexcept
			:x(X), y(Y) 
		{
		}
		constexpr explicit float2(float v)noexcept
			:x(v), y(v)
		{
		}

		constexpr float2 operator+    (const float2& rhs)const noexcept    { return { x + rhs.x,  y + rhs.y  }; }
		constexpr float2 operator-    (const float2& rhs)const noexcept    { return { x - rhs.x,  y - rhs.y  }; }
		constexpr float2 operator*    (float scalar)const noexcept         { return { x * scalar, y * scalar }; }
		constexpr float2 operator/    (float scalar)const noexcept         { return { x / scalar, y / scalar }; }
		constexpr float2 operator-    ()const noexcept                     { return { -x, -y };                 }

		constexpr float2& operator+=  (const float2& rhs)noexcept          { x += rhs.x;  y += rhs.y;  return *this; }
		constexpr float2& operator-=  (const float2& rhs)noexcept          { x -= rhs.x;  y -= rhs.y;  return *this; }
		constexpr float2& operator*=  (float scalar)noexcept               { x *= scalar; y *= scalar; return *this; }
		constexpr float2& operator/=  (float scalar)noexcept               { x /= scalar; y /= scalar; return *this; }

		constexpr bool operator ==    (const float2& rhs)const noexcept    { return x == rhs.x && y == rhs.y; }
		constexpr bool operator!=     (const float2& rhs)const noexcept    { return !(*this == rhs);          }


		inline float magnitude()const noexcept 
		{
			return std::sqrt((x * x) + (y * y));
		}

		inline float2 normal()const noexcept
		{
			auto len = magnitude();
			return { x / len, y / len };
		}

		constexpr float2 perpendicular_ccw() noexcept
		{
			return { -y, x };
		}

		constexpr float2 perpendicular_cw() noexcept
		{
			return { y, -x };
		}

		constexpr float2 sign()noexcept
		{
			return { (x > 0) - (x < 0), (y > 0) - (y < 0) };
		}
	};
	
	constexpr float2 operator*(float scalar, const float2& v)noexcept
	{
		return float2{ v.x * scalar, v.y * scalar };
	}

	constexpr float magnitude_sq(const float2& v) noexcept
	{
		return (v.x * v.x) + (v.y * v.y);
	}

	constexpr float2 normal_optimized(const float2& v, float pre_calculated_magnitude) noexcept
	{
		return { v.x / pre_calculated_magnitude ,v.y / pre_calculated_magnitude };
	}

	constexpr float dot_product(const float2& a, const float2& b)noexcept
	{
		return  (a.x * b.x) + (a.y * b.y);
	}

	constexpr float2 projection(const float2& v, const float2& unit_vector)noexcept
	{
		return dot_product(v, unit_vector) * unit_vector;
	}

	constexpr float2 reflection(const float2& i, const float2& unit_vector)noexcept
	{
		return i - (2.0f * projection(i, unit_vector));
	}


	//maybe poopoo caakaa and needs depricating because mat3 does rotation anyway.
	//even if angle to normal is needed, it has a place elsewhere
	inline float2 angle_to_normalized(float angle) noexcept
	{
		return float2(std::cos(angle), std::sin(angle));
	}

	constexpr float2 rotated(const float2& v, const float2& rot)noexcept
	{
		return {
			v.x * rot.x - v.y * rot.y,
			v.x * rot.y + v.y * rot.x
		};
	}
}