#pragma once
#include <string>
#include <memory>
#include <SDL3/SDL_render.h>

namespace badWindow
{
	/// <summary> Functor deleter for SDL_Renderer </summary>
	struct RendererDeleter {
		void operator()(SDL_Renderer* r)
		{
			if (r)
				SDL_DestroyRenderer(r);
		}
	};

	/// <summary> Wrapper for SDL_Renderer using a unique_ptr with a custom functor deleter </summary>
	using Renderer = std::unique_ptr<SDL_Renderer, RendererDeleter>;

	/**
	* Create a 2D rendering context for a window.
	* 
	* For detailed documentation consult SDL_CreateRenderer.
	* \param the window where rendering is displayed
	* \param the name of the rendering driver to initialize, or NULL to let SDL choose one
	* \returns a unique pointer with a custom deleter for a valid rendering context or NULL on failure; call SDL_GetError() for more info
	* \throws noexcept
	*/
	inline Renderer make_renderer(SDL_Window* window, const char* name) noexcept
	{
		//if it doesn't inline, whatever. not gonna make a cpp file just for this
		return Renderer{ SDL_CreateRenderer(window, name)};
	}
}