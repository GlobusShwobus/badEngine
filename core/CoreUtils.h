#pragma once

#include <concepts>

namespace badCore
{
	static constexpr float EPSILON_core = 0.0001f;


	// UB if invoked with numeric_limits< MIN >, same as STL
	template <std::signed_integral T>
	constexpr T core_abs(T val)noexcept
	{
		return val < 0 ? val * -1 : val;
	}

	template <std::floating_point T>
	constexpr T core_abs(T val)noexcept
	{
		return val < 0 ? val * -1 : val;
	}

	template <std::totally_ordered T> 
	constexpr const T& core_max(const T& x, const T& y)noexcept 
	{
		return (x < y) ? y : x;
	}

	template<std::totally_ordered T>
	constexpr const T& core_min(const T& x, const T& y)noexcept 
	{
		return (x < y) ? x : y;
	}

	//undefined behavior if low is higher than high
	template<std::totally_ordered T>
	constexpr const T& core_clamp(const T& val, const T& low, const T& high) 
	{
		if (val < low) return low;
		if (high < val)return high;
		return val;
	}

	template<typename T>
	constexpr bool core_is_minus(T x)noexcept
		requires std::signed_integral <T> || std::floating_point <T>
	{
		return x < 0;
	}

	template<typename T>
	constexpr bool core_is_plus(T x)noexcept
		requires std::signed_integral <T> || std::floating_point <T>
	{
		return x > 0;
	}

	template<typename T> 
	constexpr void core_swap(T& a, T& b)noexcept
		requires std::is_move_constructible_v<T>&& std::is_move_assignable_v<T>
	{
		//NOTE:: may require manual rref casting if something breaks
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	template<std::floating_point T>
	constexpr bool core_is_NAN(T x)noexcept
	{
		return x != x;
	}

	inline void secure_zero_bytes(void* ptr, std::size_t size) noexcept
	{
		//force the compiler to actually perform the operation via volitile
		volatile unsigned char* p =
			reinterpret_cast<volatile unsigned char*>(ptr);
		while (size--) {
			*p++ = 0;
		}
	}
}