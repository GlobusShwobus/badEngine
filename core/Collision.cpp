#include "pch.h"
#include "Collision.h"
#include "bad_geometry.h"
#include "bad_utility.h"

namespace badCore
{
	bool overlap_test(const AABB& a, const AABB& b, float& overlapWidth, float& overlapHeight)noexcept
	{
		const float2 distance_between_centers = absolute(center_point(a) - center_point(b));

		overlapWidth = (a.w * 0.5f) + (b.w * 0.5f) - distance_between_centers.x;
		overlapHeight = (a.h * 0.5f) + (b.h * 0.5f) - distance_between_centers.y;

		return overlapWidth > 0.0f && overlapHeight > 0.0f;
	}

	void sweep_test(const AABB& target, const Ray& ray, float& t_entry, float& t_exit)noexcept
	{
		float2 invdir(
			(ray.dir.x == 0.0f) ? INFINITY : 1.0f / ray.dir.x,
			(ray.dir.y == 0.0f) ? INFINITY : 1.0f / ray.dir.y
		);
		float t_near_x = (target.x - ray.origin.x) * invdir.x;
		float t_far_x = (target.x + target.w - ray.origin.x) * invdir.x;
		float t_near_y = (target.y - ray.origin.y) * invdir.y;
		float t_far_y = (target.y + target.h - ray.origin.y) * invdir.y;

		t_entry = core_max(core_min(t_near_x, t_far_x), core_min(t_near_y, t_far_y));
		t_exit = core_min(core_max(t_near_x, t_far_x), core_max(t_near_y, t_far_y));
	}

	bool sweep_test(const AABB& target, const Ray& ray, float& time, float2& contact_point)noexcept
	{
		float tNear, tFar;
		sweep_test(target, ray, tNear, tFar);

		bool is_hit = false;

		if (is_sweep_hit(tNear, tFar)) {
			time = (tNear < 0.0f) ? tFar : tNear;
			contact_point = ray.origin + time * ray.dir;
			is_hit = time >= 0.0f && (time * time) < ray.magnitude;
		}

		return is_hit;
	}

	bool intersection_test(const Ray& target_surface, const float2& point, float radius, float2& out_normal, float& out_penetration)noexcept
	{
		float2 closest = closest_point(target_surface, point);
	
		float2 difference = point - closest;
		float dist = length(difference);

		if (dist > radius) {
			return false;
		}

		out_normal = normalize(difference, dist);
		out_penetration = radius - dist;

		return true;
	}
}