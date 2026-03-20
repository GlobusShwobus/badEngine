#include "FreeDraw.h"
#include "AsyncLogger.h"
#include "EngineUtils.h"

void bad::draw_closed_model_transformed(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& points, const bad::Mat3& transformer, bad::Color color)
{
	static constexpr int MAX_STACK_LIMIT = 64;
	const auto pSize = points.size();
	struct draw_lines_impl
	{
		void operator()(SDL_Renderer* const renderer, const bad::Sequence<bad::Point>& src, std::size_t size, SDL_FPoint* dst, const bad::Mat3& transformer)
		{
			std::size_t index = 0;
			for (; index < size; ++index) {
				dst[index] = transform_point_to_sdl_fpoint(src[index], transformer);
			}
			dst[size] = dst[0];
			SDL_RenderLines(renderer, dst, static_cast<int>(size + 1));
		}
	}draw_impl;

	//TODO:: maybe just asserting is better but this is way later to check
	//test errors, maybe asserts are better
	if (pSize < 2)
	{
		bad::AsyncLogger::Global().log("draw_closed_model early exit due to low size");
		return;
	}
	if (!renderer)
	{
		bad::AsyncLogger::Global().log("Failed to draw_closed_model with nullptr renderer");
		return;
	}
	//set col
	SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());

	//optimal path on the stack if low amount of points
	if (pSize <= MAX_STACK_LIMIT)
	{
		SDL_FPoint stack_pts[MAX_STACK_LIMIT + 1];
		draw_impl(renderer, points, pSize, stack_pts, transformer);
	}
	else//slower path on the heap if large amount of points
	{
		bad::Sequence<SDL_FPoint> heap_pts(pSize + 1, {});
		draw_impl(renderer, points, pSize, heap_pts.data(), transformer);
	}
}