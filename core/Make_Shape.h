#pragma once

#include "Float2.h"
#include "Rect.h"
#include "Sequence.h"

namespace badCore
{
	constexpr Rect make_union_rect(const Rect& a, const Rect& b)noexcept
	{
		const float minx = core_min(a.min.x, b.min.x);
		const float miny = core_min(a.min.y, b.min.y);
		const float maxx = core_max(a.max.x, b.max.x);
		const float maxy = core_max(a.max.y, b.max.y);

		return { {minx, miny},{maxx, maxy} };
	}

	Sequence<float2> make_poly(float outerRadius, float innerRadius, int nFlares);

	Sequence<float2> make_circle(const float2& center, float radius);
}