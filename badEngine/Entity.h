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
		Entity(bad::Sequence<bad::Point> model, float radius, bad::Transform&& transform, bad::Color col)
			:mModel(std::move(model)), mTransform(std::move(transform)), mColor(col), mRadius(radius)
		{
		}

		const bad::Sequence<bad::Point>& get_model()const noexcept { return mModel; }

		bad::Rect get_bb()const noexcept
		{
			return bad::Rect{ mTransform.mPos - (mRadius * mTransform.mScale), mTransform.mPos + (mRadius * mTransform.mScale) };
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


		bad::Mat3 get_transform_matrix()const noexcept { return mTransform.make_transformed(); }

		const bad::Transform& get_transform()const noexcept { return mTransform; }

		const bad::Point& get_pos()const noexcept { return mTransform.mPos; }

		bad::Color get_color()const noexcept { return mColor; }

		float get_radius()const noexcept { return mRadius; }



		void set_pulse_oscillation(float oscillation) { pulse_dir = oscillation; }

		void set_rotational_velocity(float velocity) { rotational_velocity = velocity; }

	protected:
		bad::Sequence<bad::Point> mModel;
		bad::Transform mTransform;
		bad::Color mColor = bad::Colors::Magenta;
		float mRadius;

		float pulse_size = 1;
		float pulse_dir = 1;
		float rotational_velocity = 1;
	};


	class Plank : public Entity
	{
	public:
		Plank(Entity&& ent)
			:Entity(std::move(ent))
		{
			movable_pnt1 = &mModel[1];
			movable_pnt2 = &mModel[2];
		}

		void set_offset_y(float offset_y) noexcept
		{
			movable_pnt1->y += offset_y;
			movable_pnt2->y += offset_y;
		}

		void set_offset_x(float offset_x) noexcept
		{
			movable_pnt1->x += offset_x;
			movable_pnt2->x += offset_x;
		}


	private:
		bad::Point* movable_pnt1 = nullptr;
		bad::Point* movable_pnt2 = nullptr;
	};
}