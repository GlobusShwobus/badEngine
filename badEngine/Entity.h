#pragma once

#include "Sequence.h"
#include "vector.h"

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

		void TranslateBy(const badCore::float2& offset)
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


		Model get_model()const noexcept
		{
			Model copy_model = mModel;

			for (auto& v : copy_model) {
				v *= mScale;//entity scaleing
				v += mPos;//entity translation
			}
			return copy_model;
		}


	private:
		float mScale = 1.0f;
		Position mPos;
		Model mModel;
	};
}