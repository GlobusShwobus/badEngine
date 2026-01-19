#include "pch.h"
#include "WindowContext.h"

namespace badEngine {

	WindowContext::WindowContext(const char* heading, uint32_t width, uint32_t height, std::size_t flags)
	{
		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
			return;

		SDL_Window* window = SDL_CreateWindow(
			heading,
			width,
			height,
			flags
		);

		if (!window) {
			SDL_QuitSubSystem(SDL_INIT_VIDEO);
			return;
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
		if (!renderer) {
			SDL_DestroyWindow(window);
			SDL_QuitSubSystem(SDL_INIT_VIDEO);
			return;
		}

		mWindow.reset(window);
		mRenderer.reset(renderer);
	}

	SDL_Window* const WindowContext::get_window()const noexcept {
		return mWindow.get();
	}
	SDL_Renderer* const WindowContext::get_render()const noexcept {
		return mRenderer.get();
	}

	bool WindowContext::is_good()const noexcept
	{
		return mWindow && mRenderer;
	}

	void WindowContext::reset()noexcept
	{
		mRenderer.reset();
		mWindow.reset();
		mDrawColor = Colors::Black;
		SDL_Quit();
	}

	bool WindowContext::system_present()const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_SetRenderTarget(ren, nullptr);//on setting to null should never error
		return SDL_RenderPresent(ren);
	}

	bool WindowContext::system_refresh()const noexcept
	{
		return SDL_RenderClear(mRenderer.get());
	}

	bool WindowContext::set_render_target(SDL_Texture* target)const noexcept
	{
		return SDL_SetRenderTarget(mRenderer.get(), target);
	}

	void WindowContext::set_default_color(Color color)noexcept
	{
		if (SDL_SetRenderDrawColor(mRenderer.get(), color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()))
			mDrawColor = color;
	}

	void WindowContext::draw_shape(const AABB& aabb, Color color, AABB* other, Color* otherCol)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		//set the drawing color
		SDL_SetRenderDrawColor(ren, color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
		//convert aabb to sdl type and draw it
		SDL_FRect sdlArea = SDL_FRect(aabb.x, aabb.y, aabb.w, aabb.h);
		SDL_RenderFillRect(ren, &sdlArea);

		//if other draw the other doing the same thing
		if (other) {
			if (otherCol) {
				SDL_SetRenderDrawColor(ren, otherCol->get_red(), otherCol->get_green(), otherCol->get_blue(), otherCol->get_alpha());
			}
			SDL_FRect sdlOther = SDL_FRect(other->x, other->y, other->w, other->h);
			SDL_RenderFillRect(ren, &sdlOther);
		}
		//reset the color
		SDL_SetRenderDrawColor(ren, mDrawColor.get_red(), mDrawColor.get_green(), mDrawColor.get_blue(), mDrawColor.get_alpha());
	}

	void WindowContext::draw_shape(const float2& start, const float2& end, std::size_t thickness, Color color)
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_SetRenderDrawColor(ren, color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());

		if (thickness <= 1) {
			SDL_RenderLine(ren, start.x, start.y, end.x, end.y);
		}
		else {
			// using a right triangle

			// find the deltas
			const float A = end.x - start.x;
			const float B = end.y - start.y;
			// pythagorean length of C
			const float C = std::sqrtf(A * A + B * B);
			if (C == 0) return;

			// create a perpendicular vector by swaping x, y and negating one. also make it unit length
			const float px = -B / C;
			const float py = A / C;

			const int iThickness = static_cast<int>(thickness);
			// draw lines on both sides perpendicular to original line
			for (int i = -iThickness / 2; i <= iThickness / 2; ++i)
			{
				float offsetX = px * i;
				float offsetY = py * i;
				SDL_RenderLine(ren,
					start.x + offsetX, start.y + offsetY,
					end.x + offsetX, end.y + offsetY
				);
			}
		}
		SDL_SetRenderDrawColor(ren, mDrawColor.get_red(), mDrawColor.get_green(), mDrawColor.get_blue(), mDrawColor.get_alpha());
	}

	void WindowContext::draw_texture(SDL_Texture* texture, const AABB& source, const AABB& dest)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_FRect sdlSrc(source.x, source.y, source.w, source.h);
		SDL_FRect sdlDest(dest.x, dest.y, dest.w, dest.h);

		SDL_RenderTexture(ren, texture, &sdlSrc, &sdlDest);
	}

	void WindowContext::draw_texture(SDL_Texture* texture)const noexcept
	{
		SDL_RenderTexture(mRenderer.get(), texture, nullptr, nullptr);
	}

	void WindowContext::draw_sprite(const BasicSprite& sprite)const noexcept
	{
		draw_texture(sprite.mTexture, sprite.mSource, sprite.mDest);
	}
	void WindowContext::draw_sprite(const Animation& sprite)const noexcept
	{
		draw_texture(sprite.mTexture, sprite.mSource, sprite.mDest);
	}
	void WindowContext::draw_sprite(const Font& sprite)const noexcept
	{
		auto it = sprite.begin();
		for (; it != sprite.end(); ++it) {
			draw_texture(sprite.mTexture, it->first, it->second);
		}
	}
}