#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Renderer.h"
#include "Rect.h"
#include "Color.h"
#include "Mat3.h"

namespace bad
{
	void draw_rect_lines(SDL_Renderer* const renderer, const bad::Rect& rect, const bad::Mat3& transformer, bad::Color color);

	//NOTE:: sets color, does not reset color
	void draw_closed_model_transformed(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& points, const bad::Mat3& transformer, bad::Color color);
}