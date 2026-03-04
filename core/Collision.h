#pragma once

#include "vector.h"
#include "AABB.h"
#include "Ray.h"

namespace badCore
{

	void sweep_test(const AABB& target, const Ray& ray, float& t_entry, float& t_exit)noexcept;

	bool sweep_test(const AABB& target, const Ray& ray, float& t, float2& contact_point)noexcept;

	constexpr bool is_sweep_hit(float entry, float exit)noexcept
	{
		return entry <= exit && exit >= 0.0f;
	}

	bool intersection_test(const Ray& target_surface, const float2& point, float radius)noexcept;

	bool intersection_test(const Ray& target_surface, const float2& point, float radius, float2& difference, float& distance_squared)noexcept;

	//TODO:: this is temporary signature until i get a canon entity base class going then just use that one instead of args
	struct Resolution
	{
		float2 new_velocity;
		float2 position_offset;
		bool is_hit = false;
	};
	
	Resolution reflection_routine_resolved(const Ray& target_surface, const float2& point, const float2& velocity, float radius) noexcept;
};
