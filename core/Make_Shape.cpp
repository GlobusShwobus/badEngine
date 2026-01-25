#include "pch.h"
#include "Make_Shape.h"

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
}