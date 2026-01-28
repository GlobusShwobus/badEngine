#pragma once

#include <memory>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <span>

#include "mySDL_utils.h"
#include "vector.h"
#include "Color.h"
#include "AABB.h"
#include "Ray.h"
#include "Matrix3.h"
#include "Sequence.h"
#include <bit>

namespace badWindow
{
	class WindowContext {

		using Renderer = std::unique_ptr<SDL_Renderer, SDLDeleter<SDL_Renderer, SDL_DestroyRenderer>>;
		using Window = std::unique_ptr<SDL_Window, SDLDeleter<SDL_Window, SDL_DestroyWindow>>;
		using Color = badCore::Color;
		using AABB = badCore::AABB;
		using float2 = badCore::float2;

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
		void draw_AABB(const AABB& aabb, Color color, AABB* other = nullptr, Color* otherCol = nullptr)const noexcept;

		// renders a line on the screen with a given color
		void draw_line(const badCore::Ray& ray, Color color)const noexcept;

		// renders multiple lines with a given color
		void draw_lines(std::span<const float2> points, Color color)const noexcept {
			static_assert(sizeof(float2) == sizeof(SDL_FPoint));
			static_assert(alignof(float2) == alignof(SDL_FPoint));
			static_assert(std::is_standard_layout_v<float2>);
			static_assert(std::is_trivially_copyable_v<float2>);
			
			SDL_SetRenderDrawColor(mRenderer.get(), color.get_r(), color.get_g(), color.get_b(), color.get_a());
			const SDL_FPoint* sdlpoints = reinterpret_cast<const SDL_FPoint*>(points.data());
			SDL_RenderLines(mRenderer.get(), sdlpoints, points.size());
			SDL_SetRenderDrawColor(mRenderer.get(), mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
		}

		void draw_lines2(std::span<const float2> points, Color color)const noexcept {
			badCore::Sequence<SDL_FPoint> tosdl;
			tosdl.set_capacity(points.size());

			for (auto& p : points) {
				tosdl.emplace_back(p.x, p.y);
			}

			SDL_SetRenderDrawColor(mRenderer.get(), color.get_r(), color.get_g(), color.get_b(), color.get_a());
			SDL_RenderLines(mRenderer.get(), tosdl.data(), points.size());
			SDL_SetRenderDrawColor(mRenderer.get(), mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
		}

		void draw_lines3(std::span<const float2> points, Color color)const noexcept {
			badCore::Sequence<SDL_FPoint> tosdl;
			tosdl.set_capacity(points.size());

			for (auto& p : points) {
				tosdl.emplace_back(std::bit_cast<SDL_FPoint>(p));
			}

			SDL_SetRenderDrawColor(mRenderer.get(), color.get_r(), color.get_g(), color.get_b(), color.get_a());
			SDL_RenderLines(mRenderer.get(), tosdl.data(), points.size());
			SDL_SetRenderDrawColor(mRenderer.get(), mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
		}

		// draws a texture with specified source and dest locations. SDL does automatic cliping.
		void draw_texture(SDL_Texture* texture, const AABB& src, const AABB& dest)const noexcept;

		void draw_texture(SDL_Texture* texture, std::span<const std::pair<AABB,AABB>> src_dests)const noexcept;

		// sets target where all drawing operations will take place in. Target texture sould be created as a targetable texture (for example created with create_texture_targetable)
		// returns true of success, false on failure. call SDL_GetError on failure for details
		bool set_render_target(SDL_Texture* target)const noexcept;
		// sets the default color the renderer refreshes to or resets back to after drawing shapes
		void set_default_color(Color color)noexcept;


		SDL_Window* const get_window()const noexcept;
		SDL_Renderer* const get_render()const noexcept;

		badCore::int2 get_window_size()const noexcept;

		badCore::Mat3 window_transform()const noexcept;

	private:
		/* ORDER MATTERS BECAUSE OF DELETER! ALWAYS DELETE RENDERER BEFORE WINDOW */
		Renderer mRenderer;
		Window mWindow;

		Color mDrawColor = badCore::Colors::Black;
	};
}