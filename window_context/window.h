#pragma once

#include <string>
#include <memory>
#include <SDL3/SDL_video.h>

namespace badWindow
{
	// Functor deleter for SDL_Window.
	struct WindowDeleter
	{
		void operator()(SDL_Window* w) 
		{
			if (w)
				SDL_DestroyWindow(w);
		}
	};

	// Wrapper for SDL_Window using a unique_ptr with a custom functor deleter.
	using Window = std::unique_ptr<SDL_Window, WindowDeleter>;

	/**
	* Create a window with the specified dimensions and flags.
	* 
	* For detailed documentation constult SDL_CreateWindow.
	* \param title of the window, in UTF-8 encoding.
	* \param width of the window
	* \param height of the window
	* \param flags 0, or one or more SDL_WindowFlags using operator OR |
	* \returns a unique pointer with a custom deleter that was created or NULL on failure; call SDL_GetError() for more information.
	* \throws noexcept
	*/
	inline Window make_window(const std::string& title, uint32_t width, uint32_t height, SDL_WindowFlags flags) noexcept
	{
		//if it doesn't inline, whatever. not gonna make a cpp file just for this
		return Window{ SDL_CreateWindow(title.c_str(), width, height, flags)};
	}
}