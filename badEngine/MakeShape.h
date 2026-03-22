#pragma once

#include "Float2.h"
#include "Sequence.h"
#include <array>
namespace bad
{
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
	bad::Sequence<bad::Point> make_poly(float outerRadius, float innerRadius, int nFlares);

	/**
	* Generates a 4 point shape object. Top left is always at { 0 , 0 }.
	* 
	* \param width the width of the shape
	* \param height the height of the shape
	* 
	* \returns an array of 4 points.
	*/
	bad::Sequence<bad::Point> make_4ptsShape_heap(float width, float height);


	/**
	* Generates a 4 point shape object. Top left is always at { 0 , 0 }.
	*
	* \param width the width of the shape
	* \param height the height of the shape
	*
	* \returns an array of 4 points.
	*/
	std::array<bad::Point, 4> make_4ptsShape_stack(float width, float height);
}