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


	template <typename T>
	constexpr auto closest_point(const Ray& ray, const vector<T>& point)noexcept
	{
		auto vector_between_objects = point - ray.origin;
		float t = dot(vector_between_objects, ray.dir);
		// handle cases where point would be on the same infinite line, but not the line segment. clamp it to the line segment
		t = core_clamp(t, 0.0f, ray.magnitude);
		return ray.origin + ray.dir * t;
	}
}