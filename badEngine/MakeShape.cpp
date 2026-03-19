#include "pch.h"
#include "MakeShape.h"
#include "CoreUtils.h"
#include <assert.h>

bad::Sequence<bad::Point> bad::make_poly(float outerRadius, float innerRadius, int nFlares)
{
	assert(nFlares >= 1);
	bad::Sequence<bad::Point> polyline;
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