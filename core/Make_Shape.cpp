#include "pch.h"
#include "Make_Shape.h"
#include "CoreUtils.h"
#include <assert.h>

namespace badCore
{
	Sequence<Point> make_poly(float outerRadius, float innerRadius, int nFlares)
	{
		assert(nFlares >= 1);
		Sequence<Point> polyline;
		polyline.reserve(static_cast<std::size_t>(nFlares * 2));

		const float dTheta = 2.0f * 3.14159f / float(nFlares * 2);
		for (int i = 0; i < nFlares * 2; i++) {
			const float rad = (i % 2 == 0) ? outerRadius : innerRadius;

			polyline.emplace_back(
				rad * cos(float(i) * dTheta),
				rad * sin(float(i) * dTheta)
			);
		}
		return polyline;
	}

	Sequence<float2> make_circle(const float2& center, float radius)
	{
		// an esimation of how many points are required.
		// radius * 8 over shoots
		// 35/49 is an approximation of how many pointers per radius unit, which is 0.714....
		// still a estimation but good enough.
		int size = radius * 8 * 35 / 49;
		int arrsize = (size + (8 - 1)) & -8;

		Sequence<float2> circle;
		circle.reserve(arrsize);

		const int32_t diameter = (radius * 2);

		// using the modpoint algorithm bullshit
		int32_t x = (radius - 1); // x starts near the rightmost point of the circle 
		int32_t y = 0; //y starts top most point of the circle
		int32_t tx = 1;  // increment trackers
		int32_t ty = 1;	 // increment trackers
		int32_t error = (tx - diameter); // decision variable that tells which variable to increment next step

		while (x >= y)
		{
			// each point on a circle has a point directly opposite of it. octant/circle symetry.
			// because of this Point(x , y) only need to be set once, then mirroring them we get 8 points per loop.
			circle.emplace_back(float2(center.x + x, center.y - y));
			circle.emplace_back(float2(center.x + x, center.y + y));

			circle.emplace_back(float2(center.x - x, center.y - y));
			circle.emplace_back(float2(center.x - x, center.y + y));

			circle.emplace_back(float2(center.x + y, center.y - x));
			circle.emplace_back(float2(center.x + y, center.y + x));

			circle.emplace_back(float2(center.x - y, center.y - x));
			circle.emplace_back(float2(center.x - y, center.y + x));


			// ensure the resterized circle stays as close as possible to a true circle
			// if err <= 0; y needs to move up to stay close to the circle -> ty keeps track of the change in y dir
			// if err > 0; x needs to move left to stay close to the circle -> tx keeps track of the change in x dir
			// tx and ty are incremented by 2 from the derivatives of the circle equation, but as integers...
			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}

		return circle;
	}
}