#pragma once

#include "Sequence.h"
#include "Float2.h"
#include "Renderer.h"
#include "Color.h"
#include "Mat3.h"

namespace bad
{
	//NOTE:: sets color, does not reset color
	void draw_closed_model_transformed(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& points, const bad::Mat3& transformer, bad::Color color);
}