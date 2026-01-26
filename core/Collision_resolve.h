#pragma once
#include "Collision_test.h"
#include "vector.h"
#include "Ray.h"

namespace badCore
{
	struct Displacement
	{
		float2 A;
		float2 B;
	};

	struct Hit
	{
		float2 impact_point;
		float t = 1.0f;
		bool is_hit = false;
		//int materialID = -1;// invalid index = -1
	};

	// produces info from a sweep test saying if there was a hit, when exactly and impact point
	Hit sweep_resolve(const Hit_Entry_Exit& test_result, const Ray& ray)noexcept;

	// checks if overlap exists
	constexpr bool has_overlap(const Overlap& result)noexcept
	{
		return result.x > 0.0f && result.y > 0.0f;
	}

	// produces info from an overlap test saying how much should object A and object B be displaced to resolve overlap
	Displacement SAT_resolve(const float2& aVec, const float2& bVec, const Overlap& overlap) noexcept;
}