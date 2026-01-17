#include "GraphicsSys.h"
#include "BadExceptions.h"
#include <SDL3/SDL_init.h>
namespace badEngine {

	GraphicsSys::GraphicsSys(const nlohmann::json& window_config)
	{
		std::string heading = window_config["heading"];
		Uint32 width = window_config["window_width"];
		Uint32 height = window_config["window_height"];

		std::size_t flags = 0;

		const auto& flags_config = window_config.at("flags");

		for (const auto& each_flag : flags_config) {
			SDL_Flag_string_to_uint64(each_flag, flags);
		}
		// default falgs
		if (flags == 0)
			flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
			throw BasicException("SDL video subsystem failed to initialize", SDL_GetError());

		SDL_Window* window = SDL_CreateWindow(heading.data(), width, height, flags);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

		if (!window && !renderer) {
			std::string message = "Failed init: window [" + std::to_string(window != nullptr) + "] renderer [" + std::to_string(renderer != nullptr) + "]\n";
			throw BasicException(message, SDL_GetError());
		}

		mWindow.reset(window);
		mRenderer.reset(renderer);
	}

	void GraphicsSys::reset()noexcept
	{
		mRenderer.reset();
		mWindow.reset();
		mDrawColor = Colors::Black;
		SDL_Quit();
	}

	bool GraphicsSys::system_present()const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_SetRenderTarget(ren, nullptr);//on setting to null should never error
		return SDL_RenderPresent(ren);
	}

	bool GraphicsSys::system_refresh()const noexcept
	{
		return SDL_RenderClear(mRenderer.get());
	}

	bool GraphicsSys::set_render_target(SDL_Texture* target)const noexcept
	{
		return SDL_SetRenderTarget(mRenderer.get(), target);
	}

	void GraphicsSys::set_default_color(Color color)noexcept
	{
		if (SDL_SetRenderDrawColor(mRenderer.get(), color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()))
			mDrawColor = color;
	}

	void GraphicsSys::draw_shape(const AABB& aabb, Color color, AABB* other, Color* otherCol)const noexcept
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

	void GraphicsSys::draw_shape(const float2& start, const float2& end, std::size_t thickness, Color color)
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

	void GraphicsSys::draw_texture(SDL_Texture* texture, const AABB& source, const AABB& dest)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		SDL_FRect sdlSrc(source.x, source.y, source.w, source.h);
		SDL_FRect sdlDest(dest.x, dest.y, dest.w, dest.h);

		SDL_RenderTexture(ren, texture, &sdlSrc, &sdlDest);
	}

	void GraphicsSys::draw_texture(SDL_Texture* texture)const noexcept
	{
		SDL_RenderTexture(mRenderer.get(), texture, nullptr, nullptr);
	}

	void GraphicsSys::draw_sprite(const BasicSprite& sprite)const noexcept
	{
		draw_texture(sprite.mTexture, sprite.mSource, sprite.mDest);
	}
	void GraphicsSys::draw_sprite(const Animation& sprite)const noexcept
	{
		draw_texture(sprite.mTexture, sprite.mSource, sprite.mDest);
	}
	void GraphicsSys::draw_sprite(const Font& sprite)const noexcept
	{
		auto it = sprite.begin();
		for (; it != sprite.end(); ++it) {
			draw_texture(sprite.mTexture, it->first, it->second);
		}
	}



	// LOCAL HELPERS

	bool SDL_Flag_string_to_uint64(const std::string& key, std::size_t& flags)
	{
		if (key == "SDL_WINDOW_OPENGL") {
			flags |= SDL_WINDOW_OPENGL;
			return true;
		}
		if (key == "SDL_WINDOW_RESIZABLE") {

			flags |= SDL_WINDOW_RESIZABLE;
			return true;
		}
		// more flags here
		return false;
	}
}