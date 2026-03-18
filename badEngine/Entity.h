#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Matrix3.h"
#include "Color.h"
#include "Rect.h"
#include "Ray.h"
#include "Transform.h"

namespace badEngine
{
	class Entity
	{
	public:
		//by val so we can either copy/move
		Entity(badCore::Sequence<badCore::float2> model, badCore::float2 pos, float av, badCore::Color col, float scalr_differential)
		:mModel(std::move(model)), transform(pos, 1,0), rotation_speed(av), col(col), scalr_differential(scalr_differential)
		{
			scalar_velocity = 1 + scalr_differential;
		}

		void pulse_scale() {
			auto& scale = transform.mScale;

			scale *= scalar_velocity;
			if (scale > 3) {
				scale = 3;
				scalar_velocity = 1 - scalr_differential;;
			}
			if (scale < 1) {
				scale = 1;
				scalar_velocity = 1+ scalr_differential;
			}
		}

		badCore::Mat3 get_transform()const noexcept
		{
			return transform.transform();
		}

		void rotate(float dt)
		{
			float radians = std::fmod(transform.get_radians() + rotation_speed * dt, badCore::TAU);
			transform.set_rotation(radians);
		}

	public:

		badCore::Transform transform;
		float scalar_velocity = 1.0;
		float scalr_differential = 0;
		float rotation_speed = 1;


		badCore::Sequence<badCore::float2> mModel;
		badCore::Color col = badCore::Colors::Magenta;
	};

}