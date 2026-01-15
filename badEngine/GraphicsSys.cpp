#include "GraphicsSys.h"
#include "BadExceptions.h"
#include <SDL3_image/SDL_image.h>

namespace badEngine {

	GraphicsSys::GraphicsSys(const Config_JSON& window_config) {
		const nlohmann::json& json = window_config.get();

		try {
			const auto& config = json["sys_config"];

			std::string heading = config["heading"];
			Uint32 width = config["window_width"];
			Uint32 height = config["window_height"];
			Uint64 engine = config["engine"];
			Uint64 windowMode = config["mode"];

			const bool goodInit = SDL_InitSubSystem(SDL_INIT_VIDEO);
			SDL_Window* window = SDL_CreateWindow(heading.data(), width, height, engine | windowMode);
			SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

			if (!goodInit && !window && !renderer) {
				bool isWindow = window != nullptr;
				bool isRenderer = renderer != nullptr;
				std::string message = "SDL initalization error INFO: [InitSubSystem: " + std::to_string(goodInit) + "; window status: " + std::to_string(isWindow) + "; renderer status: " + std::to_string(isRenderer) + "]\n";
				throw BasicException(message, SDL_GetError());
			}

			mWindow.reset(window);
			mRenderer.reset(renderer);
		}
		catch (const nlohmann::json::exception& e) {
			const int err_code = e.id;
			const std::string my_message = "JSON exception INFO: [code: " + std::to_string(err_code)+"]";
			throw BasicException(my_message, e.what());
		}
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
		SDL_FRect sdlSrc = convert_rect(source);
		SDL_FRect sdlDest = convert_rect(dest);

		SDL_RenderTexture(ren, texture, &sdlSrc, &sdlDest);
	}

	void GraphicsSys::draw_texture(SDL_Texture* texture)const noexcept
	{
		SDL_RenderTexture(mRenderer.get(), texture, nullptr, nullptr);
	}

	SDL_Texture* GraphicsSys::create_texture_static(SDL_Surface* surface)const noexcept
	{
		SDL_Texture* txtr = SDL_CreateTextureFromSurface(mRenderer.get(), surface);
		assert(txtr != nullptr);
		return txtr;
	}

	SDL_Texture* GraphicsSys::create_texture_static(std::string_view path)const noexcept
	{
		SDL_Texture* txtr = IMG_LoadTexture(mRenderer.get(), path.data());
		assert(txtr != nullptr);
		return txtr;
	}

	SDL_Texture* GraphicsSys::create_texture_targetable(Uint32 width, Uint32 height, SDL_Texture* copy_from, AABB* src, AABB* dest)const noexcept
	{
		SDL_Renderer* ren = mRenderer.get();
		//create texture
		SDL_Texture* texture = SDL_CreateTexture(
			ren,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
		);
		assert(texture != nullptr);

		//set blend mode to blend to read alpha channels as well
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		if (copy_from) {
			//get full size of the copy texture
			float copyTextureW, copyTextureH;
			SDL_GetTextureSize(copy_from, &copyTextureW, &copyTextureH);

			//set copied size, by default whole texture
			SDL_FRect cSrc = (src) ? convert_rect(*src) : SDL_FRect(0, 0, copyTextureW, copyTextureH);
			//set destination size, by default cSrc size
			SDL_FRect cDest = (dest) ? convert_rect(*dest) : cSrc;

			//store current target, if null is fine
			SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);
			//set this texture as target so we copy data onto it
			SDL_SetRenderTarget(ren, texture);
			//render copied texture onto current canvas
			SDL_RenderTexture(ren, copy_from, &cSrc, &cDest);
			//reset target
			SDL_SetRenderTarget(ren, oldTarget);
		}
		return texture;
	}
}