#pragma once

#include "vector.h"
#include "Canvas.h"
#include "Color.h"
#include "SDL3/SDL_render.h"
#include "Make_Shape.h"
#include "Sequence.h"

namespace badEngine
{
	class Circle
	{
		//NOTE:: SCALING MAKES THE CIRCLE LOOK BLURRY
		//		 A FIX WOULD BE TO RENDER THE CIRLCE AT A MAX SIZE FIRST, THEN ONLY DOWNSCALE.
		//NOTE2: yeah this is good but not flexible AT ALL. or at least only fexible if it only ever changes size rarely. pos is fine
	public:

		Circle(badCore::float2 pos, float radius, SDL_Renderer* const renderer)
		:pos(pos),radius(radius), mCirlce(radius*2, radius*2, renderer)
		{
			int size = radius * 8 * 35 / 49;
			int arrsize = (size + (8 - 1)) & -8;

			badCore::Sequence<SDL_FPoint> points;
			points.set_capacity(arrsize);

			const int32_t diameter = (radius * 2);

			int32_t x = (radius - 1);
			int32_t y = 0;
			int32_t tx = 1;
			int32_t ty = 1;
			int32_t error = (tx - diameter);

			while (x >= y)
			{
				// Each of the following renders an octant of the circle
				points.emplace_back(radius + x, radius - y);
				points.emplace_back(radius + x, radius + y);
				points.emplace_back(radius - x, radius - y);
				points.emplace_back(radius - x, radius + y);
				points.emplace_back(radius + y, radius - x);
				points.emplace_back(radius + y, radius + x);
				points.emplace_back(radius - y, radius - x);
				points.emplace_back(radius - y, radius + x);

				if (error <= 0)
				{
					++y;
					error += ty;
					ty += 2;
				}

				if (error > 0)
				{
					--x;
					tx += 2;
					error += (tx - diameter);
				}
			}

			mCirlce.start_drawing(renderer);
			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());
			SDL_RenderPoints(renderer, points.data(), points.size());
			SDL_SetRenderDrawColor(renderer,r, g, b, a);//kind of dumb... for now
			mCirlce.end_drawing(renderer);
		}

		SDL_Texture* const get()const noexcept {
			return mCirlce.get();
		}

		SDL_FRect get_dest()const noexcept
		{
			return {pos.x, pos.y, radius*2, radius*2};
		}

		void translate_by(const badCore::float2& offset)noexcept
		{
			pos += offset;
		}


	private:
		badWindow::Canvas mCirlce;
		badCore::float2 pos;
		badCore::Color color = badCore::Colors::Magenta;//default for now, doesn't matter
		float radius;
	};


	class Circle2
	{
		using Model = badCore::Sequence<badCore::float2>;
	public:
		Circle2(badCore::float2 pos, float radius)
			:radius(radius), pos(pos)
		{
			mModel = badCore::make_poly(radius, radius, 16);
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