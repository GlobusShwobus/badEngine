#include "CollisionRoutines.h"

void bad::intersection_reflection_resolve(const IntersectionInfo& result, bad::Position& pos, bad::Vector& velocity) noexcept
{
	if (result.is_hit) {
		auto reflection_dir = result.normal;

		float dot = dot_product(velocity, reflection_dir);


		if (dot > 0.0f)
			reflection_dir = -reflection_dir;

		velocity = reflection(velocity, reflection_dir);
		pos.translate_by(reflection_dir * result.penetration);
	}
}