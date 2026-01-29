#include "pch.h"
#include "Ray.h"
#include "bad_geometry.h"
namespace badCore
{
	Ray::Ray(const float2& origin, const vec2f& vector)
		:origin(origin)
	{
		magnitude = length(vector);
		dir = normalize(vector, magnitude);
	}
}