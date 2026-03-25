#pragma once
#include "Float2.h"
#include "Line.h"
#include "Rect.h"

namespace bad
{
	namespace collision 
	{
		void intersection_reflection_resolve(const bad::collision::HitInfo& result, bad::Point& pos, bad::Vector& velocity) noexcept;

		constexpr bool overlap_test(const Rect& a, const Rect& b, float& overlapWidth, float& overlapHeight)noexcept
		{
			overlapWidth = core_min(a.max.x, b.max.x) - core_max(a.min.x, b.min.x);
			overlapHeight = core_min(a.max.y, b.max.y) - core_max(a.min.y, b.min.y);

			return overlapWidth > 0.0f && overlapHeight > 0.0f;
		}
	}
}