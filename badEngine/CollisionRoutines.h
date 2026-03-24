#pragma once
#include "Float2.h"
#include "LineSegment.h"
#include "Transform.h"
#include "Rect.h"

namespace bad
{
	void reflection_routine_resolved(const bad::LineSegment& target_surface, bad::Position& pos, bad::Vector& velocity, float radius) noexcept;

	constexpr bool overlap_test(const Rect& a, const Rect& b, float& overlapWidth, float& overlapHeight)noexcept
	{
		overlapWidth = core_min(a.max.x, b.max.x) - core_max(a.min.x, b.min.x);
		overlapHeight = core_min(a.max.y, b.max.y) - core_max(a.min.y, b.min.y);

		return overlapWidth > 0.0f && overlapHeight > 0.0f;
	}
}