#pragma once

#include <concepts>

namespace bad
{
	/**
	* \brief Returns the absolute value.
	* 
	* \param any signed integral type (unsigned is no bueno)
	* \returns by value absolute value of T
	*/
	template <std::signed_integral T>
	constexpr T core_abs(T val)noexcept
	{
		return val < 0 ? val * -1 : val;
	}

	/**
	* \brief Returns the absolute value.
	* 
	* \param any floating point type
	* \returns by value absolute value of T
	*/
	template <std::floating_point T>
	constexpr T core_abs(T val)noexcept
	{
		return val < 0 ? val * -1 : val;
	}

	/**
	* \brief Returns the greater of two values.
	* 
	* \param x as value one
	* \praam y as value two
	* \return Reference to the greater value
	*/
	template <std::totally_ordered T> 
	constexpr const T& core_max(const T& x, const T& y)noexcept 
	{
		return (x < y) ? y : x;
	}

	/**
	* \brief Returns the lesser of two values.
	*
	* \param x as value one
	* \praam y as value two
	* \return Reference to the lesser value
	*/
	template<std::totally_ordered T>
	constexpr const T& core_min(const T& x, const T& y)noexcept 
	{
		return (x < y) ? x : y;
	}

	/**
	* \brief Clamps a value between a lower and upper bound.
	*
	* If val is less than low, low is returned.
	* If val is greater than high, high is returned.
	* Otherwise val is returned.
	*
	* \param val Value to clamp
	* \param low Lower bound
	* \param high Upper bound
	* \return Reference to the clamped value
	*
	* \note Behavior is undefined if low > high.
	*/
	template<std::totally_ordered T>
	constexpr const T& core_clamp(const T& val, const T& low, const T& high) 
	{
		if (val < low) return low;
		if (high < val)return high;
		return val;
	}

	/**
	* \brief Checks if a number is negative.
	*
	* Works for signed integers and floating-point types.
	*
	* \param x Value to test
	* \return true if x is less than zero, false otherwise
	*/
	template<typename T>
	constexpr bool core_is_minus(T x)noexcept
		requires std::signed_integral <T> || std::floating_point <T>
	{
		return x < 0;
	}

	/**
	* \brief Checks if a number is positive.
	*
	* Works for signed integers and floating-point types.
	*
	* \param x Value to test
	* \return true if x is greater than zero, false otherwise
	*/
	template<typename T>
	constexpr bool core_is_plus(T x)noexcept
		requires std::signed_integral <T> || std::floating_point <T>
	{
		return x > 0;
	}

	/**
	* \brief Swaps two values using move semantics.
	*
	* Moves the contents of a into a temporary, then moves b into a,
	* and finally moves the temporary into b.
	*
	* \param a First value
	* \param b Second value
	*
	* \note Requires the type to be move constructible and move assignable.
	*/
	template<typename T> 
	constexpr void core_swap(T& a, T& b)noexcept
		requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T>
	{
		//NOTE:: may require manual rref casting if something breaks
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	/**
	* \brief Checks whether a floating-point value is NaN.
	*
	* NaN (Not a Number) is the only floating-point value
	* that does not compare equal to itself.
	*
	* \param x Floating-point value to test
	* \return true if x is NaN, false otherwise
	*/
	template<std::floating_point T>
	constexpr bool core_is_NAN(T x)noexcept
	{
		return x != x;
	}

	/**
	 * \brief Securely overwrites a block of memory with zeros.
	 *
	 * This function writes zero to every byte in the specified memory region.
	 * The use of a volatile pointer prevents the compiler from optimizing
	 * the memory clearing operation away.
	 *
	 * This is useful for clearing sensitive data such as passwords,
	 * cryptographic keys, or tokens from memory.
	 *
	 * \param ptr Pointer to the beginning of a contiguous memory block
	 * \param size Size of the memory block in bytes
	 *
	 * \note The pointer must refer to writable memory of at least `size` bytes.
	 * \note Passing a null pointer with a non-zero size results in undefined behavior.
	 */
	inline void secure_zero_bytes(void* ptr, std::size_t size) noexcept
	{
		// Force the compiler to actually perform the operation via volatile
		volatile unsigned char* p =
			reinterpret_cast<volatile unsigned char*>(ptr);

		while (size--)
			*p++ = 0;
	}
}