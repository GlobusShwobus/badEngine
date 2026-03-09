#pragma once

#include <vector>
#include "Float2.h"
#include "Matrix3.h"
#include "Color.h"
#include "Rect.h"
#include "Ray.h"

namespace badEngine
{
	class Entity
	{
	public:
		//by val so we can either copy/move
		Entity(std::vector<badCore::float2> model, badCore::float2 pos, float av, badCore::Color col, float scalr_differential)
		:mModel(std::move(model)), mPos(pos), angular_velocity(av), col(col), scalr_differential(scalr_differential)
		{
			scalar_velocity = 1 + scalr_differential;
		}

		void pulse_scale() {
			mScale *= scalar_velocity;
			if (mScale > 3) {
				mScale = 3;
				scalar_velocity = 1 - scalr_differential;;
			}
			if (mScale < 1) {
				mScale = 1;
				scalar_velocity = 1+ scalr_differential;
			}
		}

		badCore::Mat3 get_transform()const noexcept
		{
			//ORDER MATTERS
			return badCore::Mat3::translation(mPos) * badCore::Mat3::rotation(angle) * badCore::Mat3::scale(mScale, mScale);
		}

		void rotate(float dt)
		{
			angle += angular_velocity * dt;
		}

	public:
		float mScale = 1.0f;
		float scalar_velocity = 1.0;
		float scalr_differential = 0;


		badCore::float2 mPos;
		float angular_velocity = 1;
		float angle= 0;
		std::vector<badCore::float2> mModel;
		badCore::Color col = badCore::Colors::Magenta;
	};

}