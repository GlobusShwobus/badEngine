#pragma once
#include <cmath>
#include <limits>
#include "vector.h"
#include "AABB.h"

namespace badCore
{
	template<typename T>
	inline vector<T> absolute(const vector<T>& vec)noexcept
	{
		return { std::abs(vec.x), std::abs(vec.y) };
	}

	template <typename T>
	constexpr auto dot(const vector<T>& v1, const vector<T>& v2)noexcept
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	template<typename T>
	constexpr auto length_squared(const vector<T>& vec)noexcept
	{
		return (vec.x * vec.x) + (vec.y * vec.y);
	}

	template <typename T>
	inline auto length(const vector<T>& v)noexcept
	{
		return std::sqrt((v.x * v.x) + (v.y * v.y));
	}

	template <typename T>
	inline float2 normalized(const vector<T>& v) noexcept
	{
		const float len2 = static_cast<float>(length_squared(v));

		if (len2 <= std::numeric_limits<float>::epsilon())
			return { 0.0f, 0.0f };

		const float invlen = 1.0f / std::sqrt(len2);
		return { v.x * invlen, v.y * invlen };
	}

	template <typename T>
	constexpr vector<T> sign(const vector<T>& v)noexcept
	{
		return vector<T>(
			(v.x > 0) - (v.x < 0),
			(v.y > 0) - (v.y < 0)
		);
	}

	template <typename T>
	inline float angle(const vector<T>& v1, const vector<T>& v2)noexcept
	{
		const float len1 = length(v1);
		const float len2 = length(v2);

		if (len1 <= std::numeric_limits<float>::epsilon() ||
			len2 <= std::numeric_limits<float>::epsilon())
			return std::numeric_limits<float>::quiet_NaN();


		float cosAlpha = dot(v1, v2) / (len1 * len2);

		cosAlpha = core_clamp(cosAlpha, -1.0f, 1.0f);

		return std::acos(cosAlpha);
	}

	constexpr float perimeter(const AABB& aabb) noexcept
	{
		return 2.0f * (aabb.w + aabb.h);
	}

	constexpr float2 center_point(const AABB& a) noexcept
	{
		return { a.x + (a.w * 0.5f), a.y + (a.h * 0.5f) };
	}
}