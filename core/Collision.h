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

	// returns true if there is overlap. modifies given overlap
	float2 overlap_test(const AABB& a, const AABB& b)noexcept;

	constexpr bool is_overlap(const float2& overlap) noexcept
	{
		return overlap.x > 0.0f && overlap.y > 0.0f;
	}

	struct Sweep
	{
		float tNear;
		float tFar;
	};

	Sweep sweep_test(const Ray& ray, const AABB& target)noexcept;

	Sweep sweep_test_dynamic(const AABB& dynamicBox, const Ray& ray, const AABB& staticBox) noexcept;

	constexpr bool is_sweep_collision(const Sweep& result)noexcept
	{
		return result.tNear <= result.tFar && result.tFar >= 0.0f;
	}

	struct SweepHitInfo
	{
		float2 impact_point;
		float t = 1.0f;
		bool is_hit = false;
		//float2 hit_normal;
		//int materialID = -1;// invalid index = -1
	};

	SweepHitInfo make_sweep_hit_info(const Sweep& result, const Ray& ray)noexcept;
};
