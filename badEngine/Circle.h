#pragma once

#include "vector.h"
#include "Color.h"
#include "Make_Shape.h"
#include "Sequence.h"

namespace badEngine
{
	class Circle
	{
		using Model = badCore::Sequence<badCore::float2>;
	public:
		Circle(badCore::float2 pos, float radius, badCore::float2 vel)
			:radius(radius), pos(pos), vel(vel)
		{
			mModel = badCore::make_poly(radius, radius, 16);
			for (auto& p : mModel) {
				p += pos;
			}
		}

		badCore::float2 get_pos()const {
			return pos;
		}

		float get_radius()const {
			return radius;
		}

		badCore::float2 get_vel()const {
			return vel;
		}

		void set_vel(badCore::float2 velo) {
			vel = velo;
		}

		const Model& get_model()const noexcept
		{
			return mModel;
		}

		badCore::Color get_col()const noexcept {
			return color;
		}

		void update(float dt)
		{
			translate_by(vel * dt);
		}
		void translate_by(badCore::float2 offset)noexcept
		{
			pos += offset;

			for (auto& p : mModel) {
				p += offset;
			}
		}
	private:





		Model mModel;
		badCore::float2 pos;
		badCore::float2 vel;
		badCore::Color color = badCore::Colors::Cyan;//default for now, doesn't matter
		float radius;
	};
}