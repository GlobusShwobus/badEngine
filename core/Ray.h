#pragma once

#include "vector.h"

namespace badCore
{
	//NOTE:: ray should always store normalized dir
	struct Ray
	{
		Ray(const float2& origin_point, const float2& unit_vector, float magnitude)
			:origin(origin_point), dir(unit_vector), magnitude(magnitude)
		{
		}
		float2 origin;
		float2 dir;
		float magnitude;
	};
}