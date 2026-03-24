#include "pch.h"
#include "LineSegment.h"
#include "CoreUtils.h"

bad::LineSegment::LineSegment(const Point& begin, const Point& end) noexcept
	:mOrigin(begin)
{
	bad::Vector vectorized = end - begin;
	mLength = bad::length(vectorized);
	mDir = bad::get_normalized(vectorized, mLength);
}

bad::Point bad::LineSegment::closest_point(const bad::Point& point)const noexcept
{
	bad::Vector vector_between_objects = point - mOrigin;
	float t = dot_product(vector_between_objects, mDir);
	// handle cases where point would be on the same infinite line, but not the line segment. clamp it to the line segment
	t = bad::core_clamp(t, 0.0f, mLength);
	return mOrigin + mDir * t;
}

bad::SweepInfo bad::LineSegment::sweep_test(const Rect& target)const noexcept
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

bad::IntersectionInfo bad::LineSegment::intersection_test(const bad::Point& point, float radius)const noexcept
{
	float2 cp = closest_point(point);

	float2 vec_between_ray_and_point = point - cp;
	float distance = length(vec_between_ray_and_point);

	IntersectionInfo info({}, 0, false);

	if (distance <= radius)
	{
		info.is_hit = true;
		info.normal = (distance > 0.0f) ?
			bad::get_normalized(vec_between_ray_and_point, distance) :
			perpendicular(mDir);

		info.penetration = radius - distance;
	}

	return info;
}

void bad::reflection_routine_resolved(const bad::LineSegment& target_surface, bad::Point& point, bad::Vector& velocity, float radius) noexcept
{
	auto result = target_surface.intersection_test(point, radius);

	if (result.is_hit) {
		float dot = dot_product(velocity, result.normal);

		if (dot > 0.0f) 
			result.normal = -result.normal;

		velocity = reflection(velocity, result.normal);
		point += result.normal * result.penetration;
	}
}