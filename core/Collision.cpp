#include "pch.h"
#include "Collision.h"
#include "bad_geometry.h"
#include "bad_utility.h"

namespace badCore
{
	float2 overlap_test(const AABB& a, const AABB& b)noexcept
	{
		const float2 distance_between_centers = absolute(center_point(a) - center_point(b));

		return float2(
			(a.w * 0.5f) + (b.w * 0.5f) - distance_between_centers.x,
			(a.h * 0.5f) + (b.h * 0.5f) - distance_between_centers.y
		);
	}

	Sweep sweep_test(const Ray& ray, const AABB& target)noexcept
	{
		//inv dir
		float2 invdir(
			(ray.dir.x == 0.0f) ? INFINITY : 1.0f / ray.dir.x,
			(ray.dir.y == 0.0f) ? INFINITY : 1.0f / ray.dir.y
		);
		float t_near_x = (target.x - ray.origin.x) * invdir.x;
		float t_far_x = (target.x + target.w - ray.origin.x) * invdir.x;
		float t_near_y = (target.y - ray.origin.y) * invdir.y;
		float t_far_y = (target.y + target.h - ray.origin.y) * invdir.y;

		return {
			core_max(core_min(t_near_x, t_far_x), core_min(t_near_y, t_far_y)),
			core_min(core_max(t_near_x, t_far_x), core_max(t_near_y, t_far_y))
		};
	}

	Sweep sweep_test_dynamic(const AABB& dynamicBox, const Ray& ray, const AABB& staticBox) noexcept
	{
		//expanded rectangle must also be with the consideration of relative velocity
		const AABB expandedTarget = AABB(
			staticBox.x - (dynamicBox.w / 2),
			staticBox.y - (dynamicBox.h / 2),
			staticBox.w + dynamicBox.w,
			staticBox.h + dynamicBox.h
		);

		return sweep_test(ray, expandedTarget);
	}

	SweepHitInfo make_sweep_hit_info(const Sweep& result, const Ray& ray)noexcept
	{
		SweepHitInfo info;
		const float& t_near = result.tNear;
		const float& t_far = result.tFar;

		info.t = (t_near < 0.0f) ? t_far : t_near;
		info.impact_point = ray.origin + info.t * ray.dir;
		info.is_hit = info.t >= 0.0f && (info.t * info.t) < ray.magnitude;

		return info;
	}
}