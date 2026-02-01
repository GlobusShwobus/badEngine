#pragma once

#include "Sequence.h"
#include "vector.h"
#include "Matrix3.h"
#include "Color.h"
#include "AABB.h"
#include "Ray.h"
#include "bad_geometry.h"

namespace badEngine
{
	class Entity
	{
		using Model = badCore::Sequence<badCore::float2>;
		using Position = badCore::float2;
	public:
		//by val so we can either copy/move
		Entity(Model model, Position pos, float av, badCore::Color col, float scalr_differential)
		:mModel(std::move(model)), mPos(pos), angular_velocity(av), col(col), scalr_differential(scalr_differential)
		{
			scalar_velocity = 1 + scalr_differential;
		}

		const Position& get_pos()const noexcept
		{
			return mPos;
		}

		void set_pos(const badCore::float2& pos)noexcept
		{
			mPos = pos;
		}

		void translate_by(const badCore::float2& offset)noexcept
		{
			mPos += offset;
		}

		float get_scale()const noexcept
		{
			return mScale;
		}

		void set_scale(float scale)noexcept
		{
			mScale = scale;
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

		const Model& get_model()const noexcept
		{
			return mModel;
		}
		badCore::Color get_color()const noexcept
		{
			return col;
		}

		void rotate(float dt)
		{
			angle += angular_velocity * dt;
		}

	private:
		float mScale = 1.0f;
		float scalar_velocity = 1.0;
		float scalr_differential = 0;


		Position mPos;
		float angular_velocity = 1;
		float angle= 0;
		Model mModel;
		badCore::Color col = badCore::Colors::Magenta;
	};

}