#include "pch.h"
#include "Ray.h"

namespace badCore
{
	Ray::Ray(const float2& origin, const float2& vector) noexcept
		:origin(origin), length(vector.length()), dir(normal_optimized(vector, length))
	{
	}

	SweepInfo Ray::sweep_test(const Rect& target)const noexcept
	{
		float2 invdir(
			(dir.x == 0.0f) ? INFINITY : 1.0f / dir.x,
			(dir.y == 0.0f) ? INFINITY : 1.0f / dir.y
		);
		float t_near_x = (target.min.x - origin.x) * invdir.x;
		float t_far_x = (target.min.x + target.get_width() - origin.x) * invdir.x;
		float t_near_y = (target.min.y - origin.y) * invdir.y;
		float t_far_y = (target.min.y + target.get_height() - origin.y) * invdir.y;

		float t_entry = core_max(
			core_min(t_near_x, t_far_x),
			core_min(t_near_y, t_far_y)
		);
		float t_exit = core_min(
			core_max(t_near_x, t_far_x),
			core_max(t_near_y, t_far_y)
		);

		SweepInfo info({}, INFINITY, false);

		if (t_entry <= t_exit && t_exit >= 0.0f) {
			info.is_hit = true;
			info.time = core_max(t_entry, 0.0f);
			info.contact_point = origin + dir * info.time;
		}

		return info;
	}

	IntersectionInfo Ray::intersection_test(const float2& point, float radius)const noexcept
	{
		float2 closest_point = closest_point_on_ray(point);

		float2 vec_between_ray_and_point = point - closest_point;
		float distance = vec_between_ray_and_point.length();

		IntersectionInfo info({}, 0, false);

		if (distance <= radius) {
			info.normal = (distance > 0.0f) ?
				normal_optimized(vec_between_ray_and_point, distance) :
				perpendicular_ccw(dir);

			info.penetration = radius - distance;
		}

		return info;
	}

	void reflection_routine_resolved(const Ray& target_surface, float2& point, float2& velocity, float radius) noexcept
	{
		auto result = target_surface.intersection_test(point, radius);

		if (result.is_hit) {
			if (dot_product(velocity, result.normal) > 0.0f)
				result.normal = -result.normal;
			if (dot_product(velocity, result.normal) < 0.0f) {
				velocity = reflection(velocity, result.normal);
				point = result.normal * result.penetration;
			}
		}
	}
}