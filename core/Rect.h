#pragma once

#include "Float2.h"
#include "Utility.h"
#include <assert.h>

namespace badCore
{
	class Rect
	{
	public:
		class float2 min;
		class float2 max;

		constexpr Rect()noexcept
			:min{}, max{}
		{
		}

		constexpr Rect(float2 min, float2 max) noexcept
			:min(min), max(max)
		{
			assert(max.x >= min.x);
			assert(max.y >= min.y);
		}

		constexpr Rect(float x, float y, float w, float h) noexcept
			:min(x, y), max(x + w, y + h)
		{
			assert(max.x >= min.x);
			assert(max.y >= min.y);
		}

		constexpr float2 get_pos()const noexcept { return min; }
		constexpr float  get_width()const noexcept { return max.x - min.x; }
		constexpr float  get_height()const noexcept { return max.y - min.y; }

		constexpr float  get_half_width()const noexcept { return (max.x - min.x) * 0.5f; }
		constexpr float  get_half_height()const noexcept { return (max.y - min.y) * 0.5f; }

		constexpr void set_width(float w)noexcept 
		{
			assert(w >= 0.0f);
			max.x = min.x + w;
		}

		constexpr void set_height(float h)noexcept 
		{
			assert(h >= 0.0f);
			max.y = min.y + h;
		}


		// [min, max)
		constexpr bool contains(const float2& point)const noexcept
		{
			return
				point.x >= min.x && point.x < max.x &&
				point.y >= min.y && point.y < max.y;
		}
		// [min, max)
		constexpr bool contains(const Rect& other)const noexcept
		{
			return
				other.min.x >= min.x &&
				other.min.y >= min.y &&
				other.max.x <= max.x &&
				other.max.y <= max.y;
		}
		// [min, max)
		constexpr bool intersects(const Rect& other)const noexcept
		{
			return
				!(other.max.x <= min.x || other.min.x >= max.x ||
			    other.max.y <= min.y || other.min.y >= max.y);
		}


		constexpr float get_perimeter()const noexcept
		{
			return 2.0f * (get_width() + get_height());
		}

		constexpr float2 get_center() const noexcept
		{
			return { 
				min.x + (get_width() * 0.5f),
				min.y + (get_height() * 0.5f)
			};
		}
	};


	constexpr bool overlap_test(const Rect& a, const Rect& b, float& overlapWidth, float& overlapHeight)noexcept
	{
		overlapWidth  = core_min(a.max.x, b.max.x) - core_max(a.min.x, b.min.x);
		overlapHeight = core_min(a.max.y, b.max.y) - core_max(a.min.y, b.min.y);

		return overlapWidth > 0.0f && overlapHeight > 0.0f;
	}
}