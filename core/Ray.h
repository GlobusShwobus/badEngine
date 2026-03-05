#pragma once

#include "Float2.h"
#include "Rect.h"

namespace badCore
{
	// NOTE: fuck it and go with always returning structs. Face value it's more expensive than hyper optimized micro funcs 
	// BUT guaranteed info enables big brain optimizations like collision prediction (though i don't want to thing about it atm)
	struct SweepInfo
	{
		float2 contact_point;
		float time;
		bool is_hit;
	};
	
	struct IntersectionInfo
	{
		float2 normal;
		float penetration;
		bool is_hit;
	};

	// Rays constructor derives a unit vector and magnitude from the given vector
	class Ray
	{
	public:
		// vector is expected to be unnormalized
		Ray(const float2& origin, const float2& vector)
			:rayOrigin(origin), rayLen(vector.length()), rayDir(normal_optimized(vector, rayLen))
		{
		}
		//order matters!
		const float rayLen;
		const struct float2 rayOrigin;
		const struct float2 rayDir;

		SweepInfo sweep_test(const Rect& target)const noexcept
		{
			float2 invdir(
				(rayDir.x == 0.0f) ? INFINITY : 1.0f / rayDir.x,
				(rayDir.y == 0.0f) ? INFINITY : 1.0f / rayDir.y
			);
			float t_near_x = (target.min.x - rayOrigin.x) * invdir.x;
			float t_far_x  = (target.min.x + target.get_width() - rayOrigin.x) * invdir.x;
			float t_near_y = (target.min.y - rayOrigin.y) * invdir.y;
			float t_far_y  = (target.min.y + target.get_height() - rayOrigin.y) * invdir.y;

			float t_entry = core_max(
				core_min(t_near_x, t_far_x), 
				core_min(t_near_y, t_far_y)
			);
			float t_exit  = core_min(
				core_max(t_near_x, t_far_x),
				core_max(t_near_y, t_far_y)
			);

			SweepInfo info({}, INFINITY, false);

			if (t_entry <= t_exit && t_exit >= 0.0f) {
				info.is_hit = true;
				info.time = core_max(t_entry, 0.0f);
				info.contact_point= rayOrigin + rayDir * info.time;
			}
	
			return info;
		}

		constexpr bool is_hit(float time)const noexcept
		{
			return time >= 0.0f && time < rayLen;
		}

		constexpr auto closest_point_on_ray(const float2& point)const noexcept
		{
			float2 vector_between_objects = point - rayOrigin;
			float t = dot_product(vector_between_objects, rayDir);
			// handle cases where point would be on the same infinite line, but not the line segment. clamp it to the line segment
			t = core_clamp(t, 0.0f, rayLen);
			return rayOrigin + rayDir * t;
		}

		IntersectionInfo intersection_test(const float2& point, float radius)const noexcept
		{
			float2 closest_point = closest_point_on_ray(point);

			float2 vec_between_ray_and_point = point - closest_point;
			float distance = vec_between_ray_and_point.length();

			IntersectionInfo info({}, 0, false);

			if (distance <= radius) {
				info.normal= (distance > 0.0f) ?
					normal_optimized(vec_between_ray_and_point, distance) :
					perpendicular_ccw(rayDir);

				info.penetration = radius - distance;
			}

			return info;
		}
	};

	//NOTE: maybe there will be a gooder place for it, but since it's related to ray currently this is as good as it gets
	//NOTE: consider returning bool for at least if we did write to params
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