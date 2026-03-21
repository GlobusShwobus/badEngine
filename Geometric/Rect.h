#pragma once

#include "Float2.h"
#include "CoreUtils.h"
#include <assert.h>

namespace bad
{
	/**
	* \brief Rect represents an axis-aligned rectangle using two points:
	*
	*     min  -> lower-left corner
	*     max  -> upper-right corner
	*
	* The rectangle uses the half-open interval convention:
	*
	*     [min, max)
	*
	* Meaning:
	* - min is inclusive
	* - max is exclusive
	*
	* This convention is commonly used in geometry and spatial data structures
	* because it avoids ambiguity when rectangles share edges.
	*
	* Example:
	*
	*     point.x == max.x  -> outside
	*     point.x == min.x  -> inside
	*
	* The rectangle assumes:
	*
	*     max.x >= min.x
	*     max.y >= min.y
	*
	* In DEBUG builds constructors assert these conditions. In release builds
	* the behavior is undefined if violated.
	*
	* Rect is intended for lightweight geometric operations such as:
	*
	*  - spatial queries
	*  - collision tests
	*  - UI layout
	*  - bounding volumes
	*/
	class Rect
	{
	public:

		/// <summary> Lower-left corner of the rectangle. </summary>
		Point min;

		/// <summary> Upper-right corner of the rectangle. </summary>
		Point max;

		/// <summary> Default initalizes rect with all 0s. </summary>
		constexpr Rect()noexcept
			:min{}, max{}
		{
		}

		/**
		* Constructs a rectangle using explicit minimum and maximum points.
		*
		* \param min lower-left corner
		* \param max upper-right corner
		*
		* \throws In DEBUG builds asserts that max >= min.
		*/
		constexpr Rect(const Point& min, const Point& max) noexcept
			:min(min), max(max)
		{
			assert(max.x >= min.x);
			assert(max.y >= min.y);
		}

		/**
		* Constructs a rectangle using the position and dimensions.
		*
		* \param x minimum x coordinate
		* \param y minimum y coordinate
		* \param w width
		* \param h height
		*
		* \throws In DEBUG builds asserts width and height are non-negative.
		*/
		constexpr Rect(float x, float y, float w, float h) noexcept
			:min(x, y), max(x + w, y + h)
		{
			assert(max.x >= min.x);
			assert(max.y >= min.y);
		}

		/**
		* Constructs a rectangle using the given position and dimensions.
		* 
		* \param pos for min
		* \param w for width
		* \param h for height
		* 
		* \throws In DEBUG builds asserts width and height are non-negative.
		*/
		constexpr Rect(const Point& pos, float w, float h)noexcept
			:min(pos), max(pos.x + w, pos.y + h)
		{
			assert(max.x >= min.x);
			assert(max.y >= min.y);
		}

		/// <returns> returns the rectangle position ( min point ) </returns>
		constexpr float2 get_pos()const noexcept { return min; }

		/// <returns> returns the width of the rectangle </returns>
		constexpr float  get_width()const noexcept { return max.x - min.x; }

		/// <returns> returns the height of the rectangle </returns>
		constexpr float  get_height()const noexcept { return max.y - min.y; }

		/// <returns> retruns half width of the rectangle </returns>
		constexpr float  get_half_width()const noexcept { return (max.x - min.x) * 0.5f; }

		/// <returns> retruns half height of the rectangle </returns>
		constexpr float  get_half_height()const noexcept { return (max.y - min.y) * 0.5f; }

		/**
		* Sets the rectangle width.
		*
		* max.x is recomputed as:
		*
		*     max.x = min.x + width
		*
		* \throws In DEBUG builds asserts width >= 0.
		*/
		constexpr void set_width(float w)noexcept
		{
			assert(w >= 0.0f);
			max.x = min.x + w;
		}

		/**
		* Sets the rectangle height.
		*
		* max.y is recomputed as:
		*
		*     max.y = min.y + height
		*
		* \throws In DEBUG builds asserts height >= 0.
		*/
		constexpr void set_height(float h)noexcept
		{
			assert(h >= 0.0f);
			max.y = min.y + h;
		}

		/**
		* \brief Checks whether the rectangle contains a point.
		*
		* Uses the half-open interval rule [min, max).
		*
		* \returns true if contains, false if not
		*/
		constexpr bool contains(const Point& point)const noexcept
		{
			return
				point.x >= min.x && point.x < max.x &&
				point.y >= min.y && point.y < max.y;
		}

		/**
		* \brief Checks whether the rectangle completely contains another rectangle.
		*
		* Uses the half-open interval rule [min, max).
		*
		* \returns true if contains, false if not
		*/
		constexpr bool contains(const Rect& other)const noexcept
		{
			return
				other.min.x >= min.x &&
				other.min.y >= min.y &&
				other.max.x <= max.x &&
				other.max.y <= max.y;
		}

		/**
		* \brief Tests whether two rectangles overlap.
		*
		* \returns true if any area intersects.
		*/
		constexpr bool intersects(const Rect& other)const noexcept
		{
			return
				!(other.max.x <= min.x || other.min.x >= max.x ||
					other.max.y <= min.y || other.min.y >= max.y);
		}

		/**
		* Computes the area of the rectangle.
		*
		* \returns area = width * height
		*/
		constexpr float get_area() const noexcept
		{
			return get_width() * get_height();
		}

		/**
		* Computes the perimiter of the rectangle
		*
		* \returns perimiter = 2 * (width + height)
		*/
		constexpr float get_perimeter()const noexcept
		{
			return 2.0f * (get_width() + get_height());
		}

		/// <returns> retruns the center point of the rectangle </returns>
		constexpr float2 get_center() const noexcept
		{
			return {
				min.x + (get_width() * 0.5f),
				min.y + (get_height() * 0.5f)
			};
		}
	};

	/**
	* Computes the distances between to rectangles and returns bool if they overlap or not.
	*
	* \param a as rectangle one
	* \param b as rectangle two
	* \param overlapWidth reference output for overlap on the x axis
	* \param overlapHeight reference output for overlap on the y axis
	*
	* \returns true if overlapWidth and overlapHeight are both more than 0.
	*/
	constexpr bool overlap_test(const Rect& a, const Rect& b, float& overlapWidth, float& overlapHeight)noexcept
	{
		overlapWidth = core_min(a.max.x, b.max.x) - core_max(a.min.x, b.min.x);
		overlapHeight = core_min(a.max.y, b.max.y) - core_max(a.min.y, b.min.y);

		return overlapWidth > 0.0f && overlapHeight > 0.0f;
	}
}