#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Renderer.h"
#include "Rect.h"
#include "Color.h"
#include "Mat3.h"
#include "Transform.h"

namespace bad
{
	void draw_line_transformed(SDL_Renderer* const renderer, const bad::Point& p0, const bad::Point& p1, const bad::Mat3& transformer, bad::Color color);

	void draw_rect_lines_transformed(SDL_Renderer* const renderer, const bad::Rect& rect, const bad::Mat3& transformer, bad::Color color);

	void draw_closed_model_transformed(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& points, const bad::Mat3& transformer, bad::Color color);
}