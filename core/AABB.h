#pragma once

#include "Float2.h"

namespace badCore
{
	class AABB
	{
		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;
	};

	constexpr float perimeter(const AABB& aabb) noexcept
	{
		return 2.0f * (aabb.w + aabb.h);
	}

	constexpr float2 center_point(const AABB& a) noexcept
	{
		return { a.x + (a.w * 0.5f), a.y + (a.h * 0.5f) };
	}
}