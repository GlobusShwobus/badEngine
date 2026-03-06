#pragma once

#include "Float2.h"
#include "Rect.h"

namespace badCore
{
	// NOTE: fuck it and go with always returning structs. Face value it's more expensive than hyper optimized micro funcs 
	// BUT guaranteed info enables big brain optimizations like collision prediction (though i don't want to thing about it atm)
	struct SweepInfo
	{
		class float2 contact_point;
		float time;
		bool is_hit;
	};
	
	struct IntersectionInfo
	{
		class float2 normal;
		float penetration;
		bool is_hit;
	};

	// Rays constructor derives a unit vector and magnitude from the given vector
	class Ray final
	{
	public:
		// vector is expected to be unnormalized
		Ray(const float2& origin, const float2& vector) noexcept;
		//order matters!
		const float length;
		const class float2 origin;
		const class float2 dir;

		SweepInfo sweep_test(const Rect& target)const noexcept;

		constexpr bool is_hit(float time)const noexcept
		{
			return time >= 0.0f && time < length;
		}

		constexpr auto closest_point_on_ray(const float2& point)const noexcept
		{
			float2 vector_between_objects = point - origin;
			float t = dot_product(vector_between_objects, dir);
			// handle cases where point would be on the same infinite line, but not the line segment. clamp it to the line segment
			t = core_clamp(t, 0.0f, length);
			return origin + dir * t;
		}

		IntersectionInfo intersection_test(const float2& point, float radius)const noexcept;
	};

	//NOTE: maybe there will be a gooder place for it, but since it's related to ray currently this is as good as it gets
	//NOTE: consider returning bool for at least if we did write to params
	void reflection_routine_resolved(const Ray& target_surface, float2& point, float2& velocity, float radius) noexcept;
}