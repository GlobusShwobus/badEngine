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
		Circle(badCore::float2 pos, float radius)
			:radius(radius), pos(pos)
		{
			mModel = badCore::make_poly(radius, radius, 16);

			for (auto& p : mModel) {
				p += pos;
			}
		}

		void translate_by(const badCore::float2& offset)noexcept
		{
			pos += offset;

			for (auto& p : mModel) {
				p += offset;
			}
		}

		const Model& get_model()const noexcept
		{
			return mModel;
		}

		badCore::Color get_col()const noexcept {
			return color;
		}

	private:
		Model mModel;
		badCore::float2 pos;
		badCore::Color color = badCore::Colors::Cyan;//default for now, doesn't matter
		float radius;
	};
}