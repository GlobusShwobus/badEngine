#pragma once


#include "Sequence.h"
#include "vector.h"

namespace badEngine
{

	class TransformModel
	{
		using Model = badCore::Sequence<badCore::float2>;
		using float2 = badCore::float2;
	public:

		TransformModel(Model model)
			:mModel(std::move(model))
		{
		}

		void translate(float2 translate_in)
		{
			mTranslation += translate_in;
		}

		void scale(float scale_in)
		{
			mScale_x *= scale_in;
			mScale_y *= scale_in;
			mTranslation *= scale_in;
		}

		void scale_independent(float scale_in_x, float scale_in_y)
		{
			mScale_x *= scale_in_x;
			mScale_y *= scale_in_y;
			mTranslation.x *= scale_in_x;
			mTranslation.y *= scale_in_y;
		}

		void finalize() {
			for (auto& v : mModel) {
				v.x *= mScale_x;
				v.y *= mScale_y;
				v += mTranslation;
			}
		}

		const Model& get_model()const {
			return mModel;
		}

	private:

		Model mModel;
		float2 mTranslation = { 0.0f, 0.0f };
		float mScale_x = 1.0f;
		float mScale_y = 1.0f;
	};

	//CLEAN DIS MESS UP
	static badCore::Sequence<badCore::float2> make_poly(float outerRadius, float innerRadius, int nFlares = 5) {
		badCore::Sequence<badCore::float2> star;
		star.set_capacity(nFlares * 2);

		const float dTheta = 2.0f * 3.14159f / float(nFlares * 2);
		for (int i = 0; i < nFlares * 2; i++) {
			const float rad = (i % 2 == 0) ? outerRadius : innerRadius;

			star.emplace_back(
				rad * cos(float(i) * dTheta),
				rad * sin(float(i) * dTheta)
			);
		}
		return star;
	}


	static void translate_in_window(TransformModel& model, const badCore::int2& window_size) {
		badCore::float2 offset = badCore::float2(window_size.x * 0.5f, window_size.y * 0.5f);
		model.scale_independent(1.0f, -1.0f);
		model.translate(offset);
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


}