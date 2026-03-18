#pragma once

#include <SDL3/SDL_render.h>
#include <iterator>
#include "Rect.h"
#include "Color.h"
#include "Ray.h"
#include "EngineUtils.h"
#include "AsyncLogger.h"
#include "Sequence.h"

namespace badEngine
{
	// does not restore original color
	void draw_rect(SDL_Renderer* const renderer, const badCore::Rect& aabb, badCore::Color color) noexcept;
	
	// does not restore original color
	void draw_line(SDL_Renderer* const renderer, const badCore::float2& start, const badCore::float2& end, badCore::Color color) noexcept;
	
	// does not restore original color
	void draw_line(SDL_Renderer* const renderer, const badCore::Ray& ray, badCore::Color color) noexcept;

	// does not restore original color
	void draw_model(SDL_Renderer* const renderer, const badCore::Sequence<badCore::Point>& model, const badCore::Mat3& point_transformation, badCore::Color color) noexcept;

	void draw_texture(SDL_Renderer* const renderer, SDL_Texture* const texture, const badCore::Rect& src, const badCore::Rect& dest) noexcept;

	// DEPRICATED
	template <std::input_iterator PairIt>
	void draw_textures(SDL_Renderer* const renderer, SDL_Texture* const texture, PairIt begin, PairIt end)
		requires std::same_as<std::pair<badCore::Rect, badCore::Rect>, std::iter_value_t<PairIt>>
	{
		if (!renderer) {
			badCore::AsyncLogger::Global().log("Failed to draw_textures with nullptr renderer");
			return;
		}

		if (!texture) {
			badCore::AsyncLogger::Global().log("Failed to draw_textures with nullptr texture");
			return;
		}

		while (begin != end) {
			const auto& pair = *begin;
			SDL_FRect ssrc = rect_to_sdl_frect(pair.first);
			SDL_FRect ddest = rect_to_sdl_frect(pair.second);
			SDL_RenderTexture(renderer, texture, &ssrc, &ddest);
			++begin;
		}
	}
}