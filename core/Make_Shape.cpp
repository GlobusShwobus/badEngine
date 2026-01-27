#include "pch.h"
#include "Make_Shape.h"
#include "bad_utility.h"
#include <assert.h>
namespace badCore
{
	AABB make_union(const AABB& a, const AABB& b)noexcept
	{
		const float minx = core_min(a.x, b.x);
		const float miny = core_min(a.y, b.y);
		const float maxx = core_max(a.x + a.w, b.x + b.w);
		const float maxy = core_max(a.y + a.h, b.y + b.h);

		return { minx, miny, maxx - minx, maxy - miny };
	}

	Sequence<float2> make_poly(float outerRadius, float innerRadius, int nFlares)
	{
		assert(nFlares >= 1);
		Sequence<float2> polyline;
		polyline.set_capacity(nFlares * 2);

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
		int size = radius * 8 * 35 / 49;
		int arrsize = (size + (8 - 1)) & -8;

		Sequence<float2> circle;
		circle.set_capacity(arrsize);

		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		while (x >= y)
		{
			// Each of the following renders an octant of the circle
			circle.emplace_back(float2(center.x + x, center.y - y));
			circle.emplace_back(float2(center.x + x, center.y + y));
			circle.emplace_back(float2(center.x - x, center.y - y));
			circle.emplace_back(float2(center.x - x, center.y + y));
			circle.emplace_back(float2(center.x + y, center.y - x));
			circle.emplace_back(float2(center.x + y, center.y + x));
			circle.emplace_back(float2(center.x - y, center.y - x));
			circle.emplace_back(float2(center.x - y, center.y + x));

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