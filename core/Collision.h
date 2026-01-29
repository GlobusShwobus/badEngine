#pragma once

#include "vector.h"
#include "AABB.h"
#include "Ray.h"

namespace badCore
{
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


	bool overlap_test(const AABB& a, const AABB& b, float& overlapWidth, float& overlapHeight)noexcept;

	void sweep_test(const AABB& target, const Ray& ray, float& t_entry, float& t_exit)noexcept;

	bool sweep_test(const AABB& target, const Ray& ray, float& t, float2& contact_point)noexcept;

	constexpr bool is_sweep_hit(float entry, float exit)noexcept
	{
		return entry <= exit && exit >= 0.0f;
	}

	bool intersection_test(const Ray& target_surface, const float2& point, float radius, float2& difference, float& distance_squared)noexcept;

};
