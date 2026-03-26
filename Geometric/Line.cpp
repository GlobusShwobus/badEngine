#include "pch.h"
#include "Line.h"
#include "CoreUtils.h"

bad::Point bad::LineSegment::closest_point(const bad::Point& target)const noexcept
{
	bad::Vector line_vec = p1 - p0;
	float line_len = bad::length(line_vec);
	bad::Vector line_normal = bad::get_normalized(line_vec, line_len);

	return bad::closest_point(p0, line_normal, line_len, target);
}

bad::Ray::Ray(const bad::Point& from, const bad::Point& to) noexcept
{
	update_ray(from, to);
}

bad::Ray::Ray(const bad::LineSegment& line) :Ray{ line.p0, line.p1 } {}

void bad::Ray::update_ray(const bad::Point& from, const bad::Point& to)noexcept
{
	mOrigin = from;
	auto vec = to - from;
	mLength = bad::length(vec);
	mDir = bad::get_normalized(vec, mLength);
}

void bad::Ray::update_ray(const bad::LineSegment& line)noexcept
{
	update_ray(line.p0, line.p1);
}


bad::collision::SweepInfo bad::collision::sweep(const bad::Ray& ray, const bad::Rect& target) noexcept
{
	const auto& dir = ray.get_dir();
	const auto& origin = ray.get_origin();

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

bad::collision::HitInfo bad::collision::intersect(const bad::Ray& ray, const bad::Circle& circle) noexcept
{
	const auto& circle_center = circle.get_center();
	const auto circle_radius = circle.get_radius();

	bad::Point closest_point_on_ray = ray.closest_point(circle_center);

	bad::Vector vector_from_point_to_circle = circle_center - closest_point_on_ray;
	float distance = length(vector_from_point_to_circle);

	HitInfo info({}, 0, false);

	if (distance <= circle_radius)
	{
		info.is_hit = true;
		info.normal = (distance > 0.0f) ?
			bad::get_normalized(vector_from_point_to_circle, distance) :
			perpendicular(ray.get_dir());

		info.penetration = circle_radius - distance;
	}

	return info;
}

bad::collision::HitInfo bad::collision::intersect(const bad::LineSegment& line, const bad::Circle& circle) noexcept
{
	bad::Ray ray(line);
	return bad::collision::intersect(ray, circle);
}