#pragma once

#include "vector.h"
#include "AABB.h"
#include "Sequence.h"

namespace badCore
{
	constexpr AABB make_aabb(const float2& center, float radius_x, float radius_y)noexcept
	{
		return{ center.x - radius_x, center.y - radius_y, radius_x + radius_x, radius_y + radius_y };
	}

	constexpr AABB expand_aabb(const AABB& expandable, const AABB& other) noexcept  
	{
		return AABB(
			expandable.x - (other.w / 2),
			expandable.y - (other.h / 2),
			expandable.w + other.w,
			expandable.h + other.h
		);
	}

	AABB make_union(const AABB& a, const AABB& b)noexcept;

	Sequence<float2> make_poly(float outerRadius, float innerRadius, int nFlares);

	Sequence<float2> make_circle(const float2& center, float radius);
}