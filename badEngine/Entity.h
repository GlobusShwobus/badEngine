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
		Entity(Model model, Position pos = Position(0.0f,0.0f))
		:mModel(std::move(model)), mPos(pos)
		{
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

		badCore::Mat3 get_transform()const noexcept
		{
			return badCore::Mat3::translation(mPos) * badCore::Mat3::scale(mScale, mScale);
		}

		const Model& get_model()const noexcept
		{
			return mModel;
		}

	private:
		float mScale = 1.0f;
		Position mPos;
		Model mModel;
	};


	class PlankBro
	{
	public:
		PlankBro(badCore::float2 pos, badCore::vec2f vector)
			:pos(pos), vector(vector)
		{
		}


		void move_vector_x(float x) {
			vector.x += x;
		}

		void move_vector_y(float y) {
			vector.y += y;
		}

		badCore::Color get_color()const noexcept
		{
			return col;
		}

		badCore::float2 get_pos()const noexcept
		{
			return pos;
		}

		badCore::float2 get_vector()const noexcept
		{
			return vector;
		}


		badCore::Ray get_ray()const noexcept
		{
			float magnitude = badCore::length(vector);

			return badCore::Ray(pos, badCore::normalized(vector, magnitude), magnitude);
		}


	private:
		badCore::float2 pos;
		badCore::float2 vector;
		badCore::Color col = badCore::Colors::Magenta;
	};

}