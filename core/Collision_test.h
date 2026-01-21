#pragma once
#include "bad_utility.h"
#include "bad_geometry.h"
#include "vector.h"
#include "AABB.h"


namespace badCore
{
	using Overlap = float2;
	using Hit_Entry_Exit = float2;

	struct Ray
	{
		Ray(const float2& origin_point, const float2& vector);
		float2 origin;
		float2 dir;
		float max_dist;
	};

	template<typename T>
	constexpr bool contains(const AABB& aabb, const vector<T>& pos) noexcept
	{
		return
			pos.x >= aabb.x &&
			pos.y >= aabb.y &&
			pos.x <= aabb.x + aabb.w &&
			pos.y <= aabb.y + aabb.h;
	}

	constexpr bool contains(const AABB& a, const AABB& b) noexcept
	{
		return
			b.x >= a.x &&
			b.y >= a.y &&
			b.x + b.w <= a.x + a.w &&
			b.y + b.h <= a.y + a.h;
	}

	template<typename T>
	constexpr bool intersects(const AABB& a, const vector<T>& pos) noexcept
	{
		return contains(a, pos);
	}

	constexpr bool intersects(const AABB& a, const AABB& b) noexcept
	{
		return
			a.x < b.x + b.w &&
			a.x + a.w > b.x &&
			a.y < b.y + b.h &&
			a.y + a.h > b.y;
	}

	// returns the distance between the centers of 2 AABB's
	Overlap overlap_test(const AABB& a, const AABB& b)noexcept;

	// returns entry and exit points
	Hit_Entry_Exit sweep_test(const Ray& ray, const AABB& target)noexcept;

	// returns entry and exit points
	Hit_Entry_Exit sweep_dynamic_test(const AABB& dynamicBox, const float2& dynamicVector, const AABB& staticBox) noexcept;
}