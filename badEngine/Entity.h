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
		Entity(bad::Sequence<bad::Point> model, const bad::Point& pos, float scale, float radians)
			:mModel(std::move(model)), mTransform(pos, scale, radians)
		{
		}

		const bad::Sequence<bad::Point>& get_model()const noexcept { return mModel; }
		bad::Color get_color()const noexcept { return mColor; }
		bad::Transform mTransform;

	private:
		bad::Sequence<bad::Point> mModel;
		bad::Color mColor = bad::Colors::Magenta;
	};
}