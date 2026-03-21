#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Transform.h"
#include "Color.h"

namespace rnd
{
	class Entity
	{
	public:
		Entity(bad::Sequence<bad::Point> model, float radius, const bad::Point& pos, float scale, float radians, bad::Color col)
			:mModel(std::move(model)), mTransform(pos, scale, radians), mColor(col),radius(radius)
		{
		}

		const bad::Sequence<bad::Point>& get_model()const noexcept { return mModel; }

		bad::Rect get_bb()const noexcept
		{
			return bad::Rect{ mTransform.mPos - (radius * mTransform.mScale), mTransform.mPos + (radius * mTransform.mScale) };
		}
		
		void pulse_effect(float dt) noexcept
		{

			pulse_size += dt * pulse_dir;

			if (pulse_size > 5) {
				pulse_dir *= -1;
				pulse_size = 5;
			}
			else if (pulse_size < 1) {
				pulse_dir *= -1;
				pulse_size = 1;
			}

			mTransform.mScale = pulse_size;
		}

		void rotate(float dt)noexcept
		{
			auto rads = mTransform.get_radians();
			mTransform.set_rotation_and_update(rads + (dt * rotational_velocity));
		}

		bad::Transform mTransform;
		bad::Color mColor = bad::Colors::Magenta;
		float radius;

		float pulse_size = 1;
		float pulse_dir = 1;
		float rotational_velocity = 1;

	private:
		bad::Sequence<bad::Point> mModel;
	};
}