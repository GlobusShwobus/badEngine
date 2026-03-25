#include "CollisionRoutines.h"

void bad::collision::intersection_reflection_resolve(const bad::collision::HitInfo& result, bad::Point& pos, bad::Vector& velocity) noexcept
{
	if (result.is_hit) {
		auto reflection_dir = result.normal;

		float dot = dot_product(velocity, reflection_dir);


		if (dot > 0.0f)
			reflection_dir = -reflection_dir;

		velocity = reflection(velocity, reflection_dir);
		pos += reflection_dir * result.penetration;
	}
}