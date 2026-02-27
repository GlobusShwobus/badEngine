#pragma once

#include <memory>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <span>
#include <bit>

//#include "mySDL_utils.h"
#include "vector.h"
#include "Color.h"
#include "AABB.h"
#include "Ray.h"
#include "Matrix3.h"

namespace badWindow
{
	class WindowContext
	{
		struct RDeleter {
			void operator()(SDL_Renderer* r) {
				if (r)
					SDL_DestroyRenderer(r);
			}
		};
		struct WDeleter {
			void operator()(SDL_Window* w) {
				if (w)
					SDL_DestroyWindow(w);
			}
		};

		using Renderer = std::unique_ptr<SDL_Renderer, RDeleter>;
		using Window = std::unique_ptr<SDL_Window, WDeleter>;

		constexpr SDL_FRect sdl_cast(const badCore::AABB& aabb) const noexcept
		{
			return std::bit_cast<SDL_FRect>(aabb);
		}

		constexpr SDL_FPoint sdl_cast(const badCore::float2& point) const noexcept
		{
			return std::bit_cast<SDL_FPoint>(point);
		}

		constexpr SDL_FPoint sdl_transform(const badCore::float2& point, const badCore::Mat3& transform)const noexcept
		{
			return
			{
			transform.a * point.x + transform.b * point.y + transform.tx,
			transform.c * point.x + transform.d * point.y + transform.ty
			};
		}

	public:

		WindowContext(const char* heading, uint32_t width, uint32_t height, std::size_t flags);

		// not sure yet if enable or disable explicitly

		//WindowContext(const WindowContext&) = delete;
		//WindowContext(WindowContext&&)noexcept = delete;
		//WindowContext& operator=(const WindowContext&) = delete;
		//WindowContext& operator=(WindowContext&&)noexcept = delete;

		~WindowContext() {
			reset();
		}
		// checks if window and renderer are running
		bool is_good()const noexcept;

		// shuts down all sub systems and SDL
		void reset()noexcept;

		// updates the screen why any rendering done since the last update
		// returns true of success, false on failure. call SDL_GetError on failure for details
		bool system_present()const noexcept;

		// clears the entire screen and sets it to the current drawing color
		// returns true of success, false on failure. call SDL_GetError on failure for details
		bool system_refresh()const noexcept;

		// renders a rectangle on the screen with a given color
		// optional other rectangle for example hollowing out the rectangle, by default draws base color of renderer
		void draw_filled_AABB(const badCore::AABB& aabb, badCore::Color color)const noexcept;

		// renders a line on the screen with a given color
		void draw_line(float x1, float y1, float x2, float y2, badCore::Color color)const noexcept;
		void draw_line(const badCore::float2& start, const badCore::float2& end, badCore::Color color)const noexcept;
		void draw_line(const badCore::Ray& ray, badCore::Color color)const noexcept;

		// draws a model from given points and connects the last point to first to close the shape
		// does nothing if given model is less than 3 points
		//TODO:: a single buffer version with a list of entities, not just points
		//TODO:: add a stack variable, if there aren't a lot of points, do a stack version instead of heap allocation 
		//TODO:: fuck span, take iterators
		void draw_closed_model(std::span<const badCore::float2> model, const badCore::Mat3& transform, badCore::Color color)const noexcept;

		// draws a texture with specified source and dest locations. SDL does automatic cliping.
		void draw_texture(SDL_Texture* texture, const badCore::AABB& src, const badCore::AABB& dest)const noexcept;

		void draw_texture(SDL_Texture* texture, std::span<const std::pair< badCore::AABB, badCore::AABB>> src_dests)const noexcept;

		// sets target where all drawing operations will take place in. Target texture sould be created as a targetable texture (for example created with create_texture_targetable)
		// returns true of success, false on failure. call SDL_GetError on failure for details
		bool set_render_target(SDL_Texture* target)const noexcept;
		// sets the default color the renderer refreshes to or resets back to after drawing shapes
		void set_default_color(badCore::Color color)noexcept;


		SDL_Window* const get_window()const noexcept;
		SDL_Renderer* const get_render()const noexcept;

		badCore::int2 get_window_size()const noexcept;

		badCore::Mat3 get_transform()const noexcept;

	private:
		/* ORDER MATTERS BECAUSE OF DELETER! ALWAYS DELETE RENDERER BEFORE WINDOW */
		Renderer mRenderer;
		Window mWindow;

		badCore::Color mDrawColor = badCore::Colors::Black;
	};
}


/*
void WindowContext::draw_lines(std::span<const float2> points, Color color)const noexcept
{
	if (points.size() < 2)
		return;

	badCore::Sequence<SDL_FPoint> tosdl;
	tosdl.set_capacity(points.size());

	for (auto& p : points) {
		tosdl.emplace_back(sdl_cast(p));
	}

	SDL_SetRenderDrawColor(mRenderer.get(), color.get_r(), color.get_g(), color.get_b(), color.get_a());
	SDL_RenderLines(mRenderer.get(), tosdl.data(), points.size());
	SDL_SetRenderDrawColor(mRenderer.get(), mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
}
*/