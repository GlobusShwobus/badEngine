#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Renderer.h"
#include "Color.h"
#include "Mat3.h"

namespace bad
{

	void draw_lines_transformed(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& points, const bad::Mat3& transformer, bad::Color color)
	{
		static constexpr int MAX_STACK_LIMIT = 64;

		struct draw_lines_impl
		{
			void operator()(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& src, SDL_FPoint* dst, const bad::Mat3& transformer)
			{
				for()
			}
		};

	}

}