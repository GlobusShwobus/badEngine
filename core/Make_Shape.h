#pragma once

#include "Float2.h"
#include "Rect.h"
#include "Sequence.h"

namespace badCore
{
	/**
	* Computes the bounding rectangle containing both input rectangles.
	*
	* The resulting rectangle is the minimal axis-aligned rectangle that
	* encloses both \p a and \p b. This operation is commonly used when
	* constructing or updating bounding volume hierarchies (BVH).
	*
	* \param a First rectangle.
	* \param b Second rectangle.
	* \return Rectangle that encloses both input rectangles.
	* 
	* \note no use case for it anymore atm...
	*/
	constexpr Rect make_union_rect(const Rect& a, const Rect& b)noexcept
	{
		const float minx = core_min(a.min.x, b.min.x);
		const float miny = core_min(a.min.y, b.min.y);
		const float maxx = core_max(a.max.x, b.max.x);
		const float maxy = core_max(a.max.y, b.max.y);

		return { {minx, miny},{maxx, maxy} };
	}

	/**
	* Generates a radial polyline with alternating outer and inner vertices.
	* 
	* The function produces a star-like shape by alternating between outer and inner radii
	* while stepping uniformly around a circle. The result contains 2x flares amount of vertices.
	* 
	* \param outerRadius Radius of the outer vertices.
	* \param innerRadius Radius of the inner vertices.
	* \param nFlares Number of flares (spikes) in the resulting shape.
	* 
	* \return Sequence of points resembling a star containing 2x Flares aount of vertices.
	*/
	Sequence<Point> make_poly(float outerRadius, float innerRadius, int nFlares);


	//#############################################################################################
	// under construction due not being sure this is how i wanna do it...
	// 
	// Generates a polyline around a center point with a given radius.
	//
	// The function uses integer based approximates for point count finding and making the circle.
	//#############################################################################################

	Sequence<float2> make_circle(const float2& center, float radius);
	
	//#############################################################################################
	//#############################################################################################
}