#include "FreeDraw.h"
#include "Sequence.h"

namespace badEngine
{
	// does not restore original color
	void draw_rect(SDL_Renderer* const renderer, const badCore::Rect& aabb, badCore::Color color) noexcept
	{
		if (!renderer) 
		{
			badCore::AsyncLogger::Global().log("Failed to draw_rect with nullptr renderer");
			return;
		}
		//set the drawing color
		SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());
		//convert aabb to sdl type and draw it
		const SDL_FRect sdlArea = rect_to_sdl_frect(aabb);
		SDL_RenderFillRect(renderer, &sdlArea);
	}

	// does not restore original color
	void draw_line(SDL_Renderer* const renderer, const badCore::float2& start, const badCore::float2& end, badCore::Color color) noexcept
	{
		if (!renderer) 
		{
			badCore::AsyncLogger::Global().log("Failed to draw_line with nullptr renderer");
			return;
		}

		SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());

		SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
	}

	// does not restore original color
	void draw_line(SDL_Renderer* const renderer, const badCore::Ray& ray, badCore::Color color) noexcept
	{
		draw_line(
			renderer,
			{ ray.mOrigin.x, ray.mOrigin.y },
			ray.get_vectorized(),
			color
		);
	}

	// does not restore original color
	void draw_model(SDL_Renderer* const renderer, const badCore::Sequence<badCore::Point>& model, const badCore::Mat3& point_transformation, badCore::Color color) noexcept
	{
		static constexpr std::size_t MAX_MODEL_SIZE = 64;

		struct DRAW_IMPL
		{
			void operator()(SDL_Renderer* renderer, SDL_FPoint* dest, const badCore::Sequence<badCore::Point>& model, const badCore::Mat3& point_transformation)
			{
				auto src = model.data();
				for (std::size_t i = 0; i < model.size(); ++i)
					dest[i] = logical_to_visual_point(src[i], point_transformation);

				dest[model.size()] = dest[0];
				SDL_RenderLines(renderer, dest, static_cast<int>(model.size() + 1));
			}
		}draw;

		if (model.size() < 2) {
			badCore::AsyncLogger::Global().log("draw_closed_model early exit due to low size");
			return;
		}

		if (!renderer) {
			badCore::AsyncLogger::Global().log("Failed to draw_closed_model with nullptr renderer");
			return;
		}

		SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());

		if (model.size() <= MAX_MODEL_SIZE)
		{
			SDL_FPoint stack_pts[MAX_MODEL_SIZE + 1];
			draw(renderer, stack_pts, model, point_transformation);
		}
		else
		{
			badCore::Sequence<SDL_FPoint> heap_pts(model.size() + 1, {});
			draw(renderer, heap_pts.data(), model, point_transformation);
		}
	}

	void draw_texture(SDL_Renderer* const renderer, SDL_Texture* const texture, const badCore::Rect& src, const badCore::Rect& dest) noexcept
	{
		if (!renderer) {
			badCore::AsyncLogger::Global().log("Failed to draw_texture with nullptr renderer");
			return;
		}

		if (!texture) {
			badCore::AsyncLogger::Global().log("Failed to draw_texture with nullptr texture");
			return;
		}

		SDL_FRect ssrc = rect_to_sdl_frect(src);
		SDL_FRect ddest = rect_to_sdl_frect(dest);
		SDL_RenderTexture(renderer, texture, &ssrc, &ddest);
	}
}