#pragma once
#include <cmath>
#include "bad_utility.h"
#include "Ray.h"
#include "vector.h"
#include "AABB.h"
#include "Matrix3.h"

namespace badCore
{
	template<typename T>
	constexpr vector<T> absolute(const vector<T>& vec)noexcept
	{
		return { core_abs(vec.x), core_abs(vec.y) };
	}

	template<typename T>
	constexpr vector<T> perpendicular(const vector<T>& vec)noexcept
	{
		return { -vec.y, vec.x };
	}

	inline float2 angle_to_dir(float angle) noexcept
	{
		return float2(std::cos(angle), std::sin(angle));
	}

	template <typename T>
	constexpr vector<T> rotate(const vector<T>& v, const float2& rot)noexcept
	{
		return {
			v.x * rot.x - v.y * rot.y,
			v.x * rot.y + v.y * rot.x
		};
	}

	template<typename T>
	constexpr auto length_squared(const vector<T>& vec)noexcept
	{
		return (vec.x * vec.x) + (vec.y * vec.y);
	}

	template <typename T>
	auto length(const vector<T>& v)noexcept
	{
		return std::sqrt((v.x * v.x) + (v.y * v.y));
	}

	template <typename T>
	constexpr auto dot(const vector<T>& a, const vector<T>& b)noexcept
	{
		return (a.x * b.x) + (a.y * b.y);
	}

	template <typename T>
	inline float2 normalize(const vector<T>& v) noexcept
	{
		return v / length(v);
	}

	template <typename T>
	constexpr float2 normalize(const vector<T>& v, float magnitude) noexcept
	{
		return v / magnitude;
	}

	template <typename T>
	constexpr auto closest_point(const Ray& ray, const vector<T>& point)noexcept
	{
		auto vector_between_objects = point - ray.origin;
		float t = dot(vector_between_objects, ray.dir);
		// handle cases where point would be on the same infinite line, but not the line segment. clamp it to the line segment
		t = core_clamp(t, 0.0f, ray.magnitude);
		return ray.origin + ray.dir * t;
	}

	template <typename T>
	inline auto projection(const vector<T>& i, const vector<T>& unit_vector)noexcept
	{
		return dot(i, unit_vector) * unit_vector;
	}

	template <typename T>
	inline auto reflection(const vector<T>& i, const vector<T>& unit_vector)noexcept
	{
		return i - (2.0f * projection(i, unit_vector));
	}

	template <typename T>
	constexpr vector<T> sign(const vector<T>& v)noexcept
	{
		return vector<T>(
			(v.x > 0) - (v.x < 0),
			(v.y > 0) - (v.y < 0)
		);
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