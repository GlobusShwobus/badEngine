#pragma once

#include <string>
#include <memory>
#include <SDL3/SDL_video.h>

namespace badWindow
{
	struct WindowDeleter
	{
		void operator()(SDL_Window* w) 
		{
			if (w)
				SDL_DestroyWindow(w);
		}
	};

	using Window = std::unique_ptr<SDL_Window, WindowDeleter>;

	inline Window make_window(const std::string& title, uint32_t width, uint32_t height, Uint64 flags) noexcept
	{
		//if it doesn't inline, whatever. not gonna make a cpp file just for this
		return Window{ SDL_CreateWindow(title.c_str(), width, height, flags)};
	}
}