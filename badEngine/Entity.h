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
			return bad::Rect{ mTransform.mPos - radius, mTransform.mPos + radius };
		}
		
		bad::Transform mTransform;
		bad::Color mColor = bad::Colors::Magenta;
		float radius;

	private:
		bad::Sequence<bad::Point> mModel;
	};
}