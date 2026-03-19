#pragma once

#include "Float2.h"
#include "Rect.h"
#include "Sequence.h"

namespace badEngine
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
	badCore::Sequence<badCore::Point> make_poly(float outerRadius, float innerRadius, int nFlares);
}