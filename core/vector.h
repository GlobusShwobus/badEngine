#pragma once

#include "bad_concepts.h"

namespace badCore {
	template <typename T> requires VECTOR_TYPE<T>
	class vector final
	{
	public:

		constexpr vector()noexcept = default;

		constexpr vector(T X, T Y)noexcept
			:x(X), y(Y) 
		{}

		template <typename U>
		constexpr vector(const vector<U>& rhs)noexcept
			:x(static_cast<T>(rhs.x)), y(static_cast<T>(rhs.y))
		{}

		template <typename U>
		constexpr vector& operator=(const vector<U>& rhs)noexcept
		{
			x = static_cast<T>(rhs.x);
			y = static_cast<T>(rhs.y);
			return *this;
		}

		template <typename U>
		constexpr auto operator+(const vector<U>& rhs)const noexcept
		{
			return vector<decltype(x + rhs.x)>(x + rhs.x, y + rhs.y);
		}

		template<typename U>
		constexpr vector& operator+=(const vector<U>& rhs)noexcept
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		template <typename S>
		constexpr auto operator-(const vector<S>& rhs)const noexcept
		{
			return vector<decltype(x + rhs.x)>(x - rhs.x, y - rhs.y);
		}

		constexpr vector operator-()const noexcept
		{
			return vector(-x, -y);
		}

		template<typename U>
		constexpr vector& operator-=(const vector<U>& rhs)noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		template<typename U>
		constexpr auto operator*(U scalar)const noexcept
			requires MATHEMATICAL_PRIMITIVE<U>
		{
			return vector<decltype(x*scalar)>(x * scalar, y * scalar);
		}

		template<typename U> 
		constexpr vector& operator*=(U scalar)noexcept
			requires MATHEMATICAL_PRIMITIVE<U>
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		template<typename U> 
		constexpr auto operator/(U scalar)const noexcept
			requires MATHEMATICAL_PRIMITIVE<U>
		{
			return vector<decltype(x * scalar)>(x / scalar, y / scalar);
		}

		template<typename U> 
		constexpr vector& operator/=(U scalar)noexcept
			requires MATHEMATICAL_PRIMITIVE<U>
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		template<typename S>
		constexpr bool operator ==(const vector<S>& rhs)const noexcept
		{
			return x == rhs.x && y == rhs.y;
		}

		template <typename S>
		constexpr bool operator!=(const vector<S>& rhs)const noexcept
		{
			return !(*this == rhs);
		}

		T x = 0;
		T y = 0;
	};
	
	template<typename T, typename U> 
	constexpr auto operator*(T scalar, const vector<U>& v)noexcept
		requires MATHEMATICAL_PRIMITIVE<T>
	{
		return vector<decltype(scalar * v.x)>(v.x * scalar, v.y * scalar);
	}

	template<typename T, typename U> 
	constexpr auto operator/(T scalar, const vector<U>& v)noexcept
		requires MATHEMATICAL_PRIMITIVE<T>
	{
		return vector<decltype(scalar / v.x)>(v.x / scalar, v.y / scalar);
	}

	using int2 = vector<int>;
	using float2 = vector<float>;
	using vec2f = vector<float>;
}