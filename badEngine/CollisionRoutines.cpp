#include "CollisionRoutines.h"

void bad::reflection_routine_resolved(const bad::LineSegment& target_surface, bad::Position& pos, bad::Vector& velocity, float radius) noexcept
{
	auto result = target_surface.intersection_test(pos.get_pos(), radius);

	if (result.is_hit) {
		float dot = dot_product(velocity, result.normal);

		if (dot > 0.0f)
			result.normal = -result.normal;

		velocity = reflection(velocity, result.normal);
		pos.translate_by(result.normal * result.penetration);
	}
}