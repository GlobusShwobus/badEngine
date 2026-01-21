#include "pch.h"
#include "Collision_resolve.h"

namespace badCore
{
	Hit sweep_resolve(const Hit_Entry_Exit& test_result, const Ray& ray)noexcept
	{
		Hit hit;
		const float& t_near = test_result.x;
		const float& t_far = test_result.y;

		// check if direction, if false then no collision
		if (t_near <= t_far && t_far >= 0.0f) {
			hit.t = (t_near < 0.0f) ? t_far : t_near;
			hit.impact_point = ray.origin + hit.t * ray.dir;
			hit.is_hit = hit.t >= 0.0f && (hit.t * hit.t) < ray.max_dist;
		}

		return hit;
	}

	Displacement SAT_resolve(const float2& aVec, const float2& bVec, const Overlap& overlap) noexcept
	{
		const float2 rVec = aVec - bVec;
		float2 A;
		float2 B;

		if (overlap.x < overlap.y) {//resolve on X axis
			A.x = (rVec.x > 0) ? overlap.x : -overlap.x;
			A.y = 0;
			B.x = -A.x;
			B.y = 0;
		}
		else {//resolve on Y axis
			A.x = 0;
			A.y = (rVec.y > 0) ? overlap.y : -overlap.y;
			B.x = 0;
			B.y = -A.y;
		}
		return { A,B };
	}
}