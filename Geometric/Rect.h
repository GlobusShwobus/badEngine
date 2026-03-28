#pragma once

#include "Float2.h"
#include <assert.h>

namespace bad
{
	/**
	* \brief
	* Rectangle represented by bad::float2 min and max points making it axis aligned.
	* 
	* Min is the corner where x and y is less than max x and y.
	* 
	* It is always assumed max > min. If not, the invarants are not respected and the rect is broken.
	*/
	class Rect
	{
	public:

		Point min;
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
		* \throws In DEBUG asserts that max >= min.
		*/
		constexpr explicit Rect(const Point& min, const Point& max) noexcept
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
		* \throws In DEBUG asserts width and height are non-negative.
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
			:Rect(pos.x, pos.y, w,h)
		{
		}

		/// <returns> returns the width of the rectangle </returns>
		constexpr float  get_width()const noexcept { return max.x - min.x; }

		/// <returns> returns the height of the rectangle </returns>
		constexpr float  get_height()const noexcept { return max.y - min.y; }

		/// <returns> retruns half width of the rectangle </returns>
		constexpr float  get_half_width()const noexcept { return get_width() * 0.5f; }

		/// <returns> retruns half height of the rectangle </returns>
		constexpr float  get_half_height()const noexcept { return get_height() * 0.5f; }

		/**
		* \brief
		* Sets the rectangles max.x varaible with: 
		*
		* min.x + w = max.x
		* \param w for width
		* \throws In DEBUG builds asserts width >= 0.
		*/
		constexpr void set_width(float w)noexcept
		{
			assert(w >= 0.0f);
			max.x = min.x + w;
		}

		/**
		* \brief
		* Sets the rectangles max.y variable with:
		* 
		* min.y + h = max.y
		*
		* \param h for height
		* \throws In DEBUG builds asserts height >= 0.
		*/
		constexpr void set_height(float h)noexcept
		{
			assert(h >= 0.0f);
			max.y = min.y + h;
		}

		/// <returns> width * height </returns>
		constexpr float get_area() const noexcept
		{
			return get_width() * get_height();
		}

		/// <returns> 2 * (width + height) </returns>
		constexpr float get_perimeter()const noexcept
		{
			return 2.0f * (get_width() + get_height());
		}

		/// <returns> retruns the center point of the rectangle </returns>
		constexpr Point get_center() const noexcept
		{
			return Point{
				min.x + (get_width() * 0.5f),
				min.y + (get_height() * 0.5f)
			};
		}
	};

	/**
	* Makes a rectangle from a given center point and width/height params.
	* 
	* Sets min = center - dimensions / 2
	* 
	* Sets max = min + dimensions
	* 
	* \param center as the center point
	* \param w as width
	* \param h as height
	* 
	* \returns rect
	*/
	constexpr Rect make_rect_from_center(const Point& center, float w, float h) noexcept {
		return Rect{
			center.x - (w * 0.5f), center.y - (h * 0.5f), w, h
		};
	}

	namespace collision
	{
		/**
		* \brief 
		* Checks if point p is in rectangle r.
		*
		* Uses the half-open interval rule [min, max).
		*
		* \returns true if contains, false if not
		*/
		constexpr bool contains(const bad::Rect& r, const Point& p) noexcept
		{
			return
				p.x >= r.min.x && p.x < r.max.x &&
				p.y >= r.min.y && p.y < r.max.y;
		}

		/**
		* \brief
		* Checks if left hand rectangle contains right hand rectangle.
		*
		* Uses the half-open interval rule [min, max).
		*
		* \returns true if contains, false if not
		*/
		constexpr bool contains(const Rect& lhs, const Rect& rhs) noexcept
		{
			return
				rhs.min.x >= lhs.min.x &&
				rhs.min.y >= lhs.min.y &&
				rhs.max.x <= lhs.max.x &&
				rhs.max.y <= lhs.max.y;
		}

		/**
		* \brief
		* Checks if lhs and rhs rectangle overlap on any side.
		*
		* \returns true if intersect, false if not
		*/
		constexpr bool intersects(const Rect& lhs, const Rect& rhs) noexcept
		{
			return
				!(rhs.max.x <= lhs.min.x || rhs.min.x >= lhs.max.x ||
					rhs.max.y <= lhs.min.y || rhs.min.y >= lhs.max.y);
		}
	}
}