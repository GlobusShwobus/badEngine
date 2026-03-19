#include "pch.h"
#include "Ray.h"

bad::Ray::Ray(const Point& origin, const Vector& vector) noexcept
		:mOrigin(origin), mLength(length(vector)), mDir(normal_optimized(vector, mLength))
	{
	}

bad::SweepInfo bad::Ray::sweep_test(const Rect& target)const noexcept
{
	float2 invdir(
		(mDir.x == 0.0f) ? INFINITY : 1.0f / mDir.x,
		(mDir.y == 0.0f) ? INFINITY : 1.0f / mDir.y
	);
	float t_near_x = (target.min.x - mOrigin.x) * invdir.x;
	float t_far_x = (target.min.x + target.get_width() - mOrigin.x) * invdir.x;
	float t_near_y = (target.min.y - mOrigin.y) * invdir.y;
	float t_far_y = (target.min.y + target.get_height() - mOrigin.y) * invdir.y;

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
		info.contact_point = mOrigin + mDir * info.time;
	}

	return info;
}

bad::IntersectionInfo bad::Ray::intersection_test(const Point& point, float radius)const noexcept
{
	float2 closest_point = closest_point_on_ray(point);

	float2 vec_between_ray_and_point = point - closest_point;
	float distance = length(vec_between_ray_and_point);

	IntersectionInfo info({}, 0, false);

	if (distance <= radius) {
		info.normal = (distance > 0.0f) ?
			normal_optimized(vec_between_ray_and_point, distance) :
			perpendicular_ccw(mDir);

		info.penetration = radius - distance;
	}

	return info;
}

void bad::reflection_routine_resolved(const Ray& target_surface, float2& point, float2& velocity, float radius) noexcept
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