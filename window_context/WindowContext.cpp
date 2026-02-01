#include "pch.h"
#include "WindowContext.h"
#include "Sequence.h"

namespace badWindow
{
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

	badCore::int2 WindowContext::get_window_size()const noexcept
	{
		badCore::int2 s;
		SDL_GetWindowSize(mWindow.get(), &s.x, &s.y);
		return s;
	}

	badCore::Mat3 WindowContext::get_transform()const noexcept
	{
		int width, height;
		SDL_GetWindowSize(mWindow.get(), &width, &height);
		return badCore::Mat3::translation({ width * 0.5f, height * 0.5f }) * badCore::Mat3::scale(1.0f, -1.0f);
	}

	bool WindowContext::is_good()const noexcept
	{
		return mWindow && mRenderer;
	}

	void WindowContext::reset()noexcept
	{
		mRenderer.reset();
		mWindow.reset();
		mDrawColor = badCore::Colors::Black;
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
		if (SDL_SetRenderDrawColor(mRenderer.get(), color.get_r(), color.get_g(), color.get_b(), color.get_a()))
			mDrawColor = color;
	}

	void WindowContext::draw_filled_AABB(const AABB& aabb, Color color)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		//set the drawing color
		SDL_SetRenderDrawColor(ren, color.get_r(), color.get_g(), color.get_b(), color.get_a());
		//convert aabb to sdl type and draw it
		SDL_FRect sdlArea = sdl_cast(aabb);
		SDL_RenderFillRect(ren, &sdlArea);

		//reset the color
		SDL_SetRenderDrawColor(ren, mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
	}

	void WindowContext::draw_line(float x1, float y1, float x2, float y2, Color color)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_SetRenderDrawColor(ren, color.get_r(), color.get_g(), color.get_b(), color.get_a());

		SDL_RenderLine(
			ren, x1, y1, x2, y2
		);

		SDL_SetRenderDrawColor(ren, mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
	}

	void WindowContext::draw_line(const badCore::float2& start, const badCore::float2& end, Color color)const noexcept
	{
		draw_line(start.x, start.y, end.x, end.y, color);
	}

	void WindowContext::draw_line(const badCore::Ray& ray, Color color)const noexcept
	{
		draw_line(
			ray.origin.x,
			ray.origin.y,
			ray.origin.x + ray.dir.x * ray.magnitude,
			ray.origin.y + ray.dir.y * ray.magnitude, color
		);
	}

	void WindowContext::draw_closed_model(std::span<const float2> model, const badCore::Mat3& transformation, Color color)const noexcept
	{
		if (model.size() < 3)
			return;

		badCore::Sequence<SDL_FPoint> tosdl;
		tosdl.set_capacity(model.size() + 1);

		for (auto& point : model) {
			tosdl.emplace_back(sdl_transform(point, transformation));
		}
		tosdl.emplace_back(tosdl.front());

		SDL_SetRenderDrawColor(mRenderer.get(), color.get_r(), color.get_g(), color.get_b(), color.get_a());
		SDL_RenderLines(mRenderer.get(), tosdl.data(), tosdl.size());
		SDL_SetRenderDrawColor(mRenderer.get(), mDrawColor.get_r(), mDrawColor.get_g(), mDrawColor.get_b(), mDrawColor.get_a());
	}

	void WindowContext::draw_texture(SDL_Texture* texture, const AABB& src, const AABB& dest)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_FRect ssrc  = sdl_cast(src);
		SDL_FRect ddest = sdl_cast(dest);
		SDL_RenderTexture(ren, texture, &ssrc, &ddest);
	}

	void WindowContext::draw_texture(SDL_Texture* texture, std::span<const std::pair<AABB, AABB>> src_dests)const noexcept
	{
		if (!texture)
			return;

		SDL_Renderer* ren = mRenderer.get();

		for (const auto& pair : src_dests) {
			SDL_FRect ssrc  = sdl_cast(pair.first);
			SDL_FRect ddest = sdl_cast(pair.second);
			SDL_RenderTexture(ren, texture, &ssrc, &ddest);
		}
	}
}