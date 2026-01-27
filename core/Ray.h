#pragma once

#include "vector.h"

namespace badCore
{
	// Rays constructor derives a unit vector and magnitude from the given vector
	class Ray
	{
	public:
		Ray(const float2& origin, const vec2f& vector);
		float2 origin;
		float2 dir;
		float magnitude;
	};
}