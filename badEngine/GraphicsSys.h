#pragma once

#include <memory>
#include "SDL3/SDL.h"
#include <SDL3_image/SDL_image.h>
#include "Color.h"
#include "Rectangle.h"
#include "json.hpp"
#include "BadExceptions.h"
#include "SDLCleanUp.h"

namespace badEngine {
#define BAD_RENDERER_EXCEPTION(type,note) BadException(__FILE__, __LINE__,type,note)

	class GraphicsSys {
		
		using Renderer = std::unique_ptr<SDL_Renderer, SDLDeleter<SDL_Renderer, SDL_DestroyRenderer>>;
		using Window = std::unique_ptr<SDL_Window, SDLDeleter<SDL_Window, SDL_DestroyWindow>>;

		inline SDL_FRect convert_rect(const AABB& rect)const noexcept {
			return SDL_FRect(rect.x, rect.y, rect.w, rect.h);
		}

	public:

		GraphicsSys(const nlohmann::json& windowConfig) {

			try {
				const auto& config = windowConfig["sys_config"];

				std::string heading = config["heading"];
				Uint32 width = config["window_width"];
				Uint32 height = config["window_height"];
				Uint64 engine = config["engine"];
				Uint64 windowMode = config["mode"];

				const bool goodInit = SDL_InitSubSystem(SDL_INIT_VIDEO);
				SDL_Window* window = SDL_CreateWindow(heading.data(), width, height, engine | windowMode);
				SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

				if (!goodInit && !window && !renderer) {
					throw BAD_RENDERER_EXCEPTION("SDL EXCEPTION", SDL_GetError());
				}

				mWindow.reset(window);
				mRenderer.reset(renderer);
			}
			catch (const nlohmann::json::exception& e) {
				throw BAD_RENDERER_EXCEPTION("JSON EXCEPTION", e.what());
			}
			catch (const BadException& e) {
				throw e;
			}

		}
		GraphicsSys(const GraphicsSys&) = delete;
		GraphicsSys(GraphicsSys&&)noexcept = delete;
		GraphicsSys& operator=(const GraphicsSys&) = delete;
		GraphicsSys& operator=(GraphicsSys&&)noexcept = delete;

		~GraphicsSys() {
			reset();
		}

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
		void draw_shape(const AABB& aabb, Color color, AABB* other = nullptr, Color* otherCol = nullptr)const noexcept;

		// renders a line on the screen with a given color and thickness
		void draw_shape(const float2& start, const float2& end, std::size_t thichness, Color color);


		// draws a texture with specified source and dest locations. SDL does automatic cliping.
		void draw_texture(SDL_Texture* texture, const AABB& source, const AABB& dest)const noexcept;

		template<std::input_iterator InputIt>
			requires std::same_as<const std::pair<AABB, AABB>&, std::iter_reference_t<InputIt>>
		void draw_texture(SDL_Texture* texture, InputIt begin, InputIt end)const noexcept
		{
			SDL_Renderer* ren = mRenderer.get();

			for (; begin != end; ++begin) {
				auto src = convert_rect(begin->first);
				auto dest = convert_rect(begin->second);
				SDL_RenderTexture(ren, texture, &src, &dest);
			}
		}

		// draws a texture
		void draw_texture(SDL_Texture* texture)const noexcept;


		// creates a static texture from an SDL_Surface
		SDL_Texture* create_texture_static(SDL_Surface* surface)const noexcept;

		// creates a static texture from a given path (currently png, jpeg, and maybe probably bmp, not sure)
		SDL_Texture* create_texture_static(std::string_view path)const noexcept;

		// creates a texture that can be used as a render target
		SDL_Texture* create_texture_targetable(Uint32 width, Uint32 height, SDL_Texture* copy_from = nullptr, AABB* src = nullptr, AABB* dest = nullptr)const noexcept;



		// sets target where all drawing operations will take place in. Target texture sould be created as a targetable texture (for example created with create_texture_targetable)
		// returns true of success, false on failure. call SDL_GetError on failure for details
		bool set_render_target(SDL_Texture* target)const noexcept;
		// sets the default color the renderer refreshes to or resets back to after drawing shapes
		void set_default_color(Color color)noexcept;


		//TODO:: isolate these with some percise functional do gooders
		SDL_Window* get_window_test()const noexcept {
			return mWindow.get();
		}
		SDL_Renderer* get_render_test()const noexcept {
			return mRenderer.get();
		}

	private:
		/* ORDER MATTERS BECAUSE OF DELETER! ALWAYS DELETE RENDERER BEFORE WINDOW */
		Renderer mRenderer;
		Window mWindow;

		Color mDrawColor = Colors::Black;
	};
}