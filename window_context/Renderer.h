#pragma once
#include <string>
#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	struct RendererDeleter {
		void operator()(SDL_Renderer* r)
		{
			if (r)
				SDL_DestroyRenderer(r);
		}
	};

	using Renderer = std::unique_ptr<SDL_Renderer, RendererDeleter>;

	inline Renderer make_renderer(SDL_Window* window, const std::string& name) noexcept
	{
		//if it doesn't inline, whatever. not gonna make a cpp file just for this
		return Renderer{ SDL_CreateRenderer(window, name.c_str())};
	}
}