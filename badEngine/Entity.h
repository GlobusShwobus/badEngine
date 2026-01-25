#pragma once

#include "Sequence.h"
#include "vector.h"
#include "Matrix3.h"
#include "Color.h"
#include "AABB.h"
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


	class StarBro : public Entity
	{
		using Color = badCore::Color;
	public:
		StarBro(badCore::Sequence<badCore::float2> model, float rad, badCore::float2 pos = badCore::float2(0.0f, 0.0f), Color c = badCore::Colors::Magenta, float vel = 1.0f)
			:Entity(std::move(model), pos), mColor(c), mVelocity(vel), radius(rad)
		{

		}

		Color get_color()const noexcept
		{
			return mColor;
		}

		void set_color(Color color)noexcept
		{
			mColor = color;
		}
		void update(float dt) noexcept
		{
			float s = get_scale();
			s += mVelocity * dt;

			set_scale(s);
			if (s >= 2) {
				set_scale(2.f);
				mVelocity = -std::abs(mVelocity);
			}
			if (s <= 0.1) {
				set_scale(0.1f);
				mVelocity = std::abs(mVelocity);
			}
		}

		badCore::AABB get_aabb()const {
			return badCore::make_aabb(get_pos(), radius, radius);
		}

	private:
		float radius;
		Color mColor;
		float mVelocity = 1.0f;
	};
}

// For not store it here, this has to be handeled somewhere here
//	void Font::update()noexcept
//	{
//		if (mLetterPos.isEmpty()) {
//			return;
//		}
//		// store original base
//		const AABB originalBase = mLetterPos.front().second;
//
//		// calculate scale factors RELATIVE to original base
//		const float sx = mDest.w / originalBase.w;
//		const float sy = mDest.h / originalBase.h;
//
//		// calculate translation based on original base
//		const float dx = mDest.x - originalBase.x;
//		const float dy = mDest.y - originalBase.y;
//
//		for (auto& pair : mLetterPos) {
//			auto& d = pair.second;
//
//			// Apply transformation relative to original base
//			d.x = mDest.x + (d.x - originalBase.x) * sx;
//			d.y = mDest.y + (d.y - originalBase.y) * sy;
//
//			// Scale width and height
//			d.w = originalBase.w * sx;
//			d.h = originalBase.h * sy;
//		}
//	}