#include "pch.h"
#include "Collision_test.h"

namespace badCore
{
	Ray::Ray(const float2& origin_point, const float2& vector)
		:origin(origin_point), dir(normalized(vector)), max_dist(length_squared(vector))
	{
	}

	// returns the distance between the centers of 2 AABB's
	Overlap overlap_test(const AABB& a, const AABB& b)noexcept
	{
		const float2 distance_between_centers = absolute(center_point(a) - center_point(b));

		return Overlap{
			(a.w * 0.5f) + (b.w * 0.5f) - distance_between_centers.x,
			(a.h * 0.5f) + (b.h * 0.5f) - distance_between_centers.y
		};
	}
	// returns entry and exit points
	Hit_Entry_Exit sweep_test(const Ray& ray, const AABB& target)noexcept
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

		return Hit_Entry_Exit(
			core_max(core_min(t_near_x, t_far_x), core_min(t_near_y, t_far_y)),
			core_min(core_max(t_near_x, t_far_x), core_max(t_near_y, t_far_y))
		);
	}

	// returns entry and exit points
	Hit_Entry_Exit sweep_dynamic_test(const AABB& dynamicBox, const float2& dynamicVector, const AABB& staticBox) noexcept
	{
		//expanded rectangle must also be with the consideration of relative velocity
		const AABB expandedTarget = AABB(
			staticBox.x - (dynamicBox.w / 2),
			staticBox.y - (dynamicBox.h / 2),
			staticBox.w + dynamicBox.w,
			staticBox.h + dynamicBox.h
		);
		Ray ray = Ray(
			center_point(dynamicBox),
			dynamicVector
		);
		return sweep_test(ray, expandedTarget);
	}
}