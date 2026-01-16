#pragma once

#include <memory>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include "SDLCleanUp.h"

#include "Config_JSON.h"
#include "Color.h"
#include "Rectangle.h"
#include "Sprite.h"


namespace badEngine {
	class GraphicsSys {
		
		using Renderer = std::unique_ptr<SDL_Renderer, SDLDeleter<SDL_Renderer, SDL_DestroyRenderer>>;
		using Window = std::unique_ptr<SDL_Window, SDLDeleter<SDL_Window, SDL_DestroyWindow>>;

	public:

		GraphicsSys(const Config_JSON& window_config);
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

		// draws a texture with default specifiers
		void draw_texture(SDL_Texture* texture)const noexcept;

		// draws different sprite types
		void draw_sprite(const BasicSprite& sprite)const noexcept;
		void draw_sprite(const Animation& sprite)const noexcept;
		void draw_sprite(const Font& sprite)const noexcept;

		// sets target where all drawing operations will take place in. Target texture sould be created as a targetable texture (for example created with create_texture_targetable)
		// returns true of success, false on failure. call SDL_GetError on failure for details
		bool set_render_target(SDL_Texture* target)const noexcept;
		// sets the default color the renderer refreshes to or resets back to after drawing shapes
		void set_default_color(Color color)noexcept;


		SDL_Window* const get_window()const noexcept {
			return mWindow.get();
		}
		SDL_Renderer* const get_render()const noexcept {
			return mRenderer.get();
		}

	private:
		/* ORDER MATTERS BECAUSE OF DELETER! ALWAYS DELETE RENDERER BEFORE WINDOW */
		Renderer mRenderer;
		Window mWindow;

		Color mDrawColor = Colors::Black;
	};
}