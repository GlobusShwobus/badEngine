#include "pch.h"
#include "SDL_SYSTEM_RAII.h"
#include <stdexcept>

badWindow::SDL_SYSTEM_RAII::SDL_SYSTEM_RAII(SDL_InitFlags flags)
{
	if (!SDL_Init(flags))
		throw std::runtime_error(std::string("Failed to init SDL: ") + SDL_GetError());
}

badWindow::SDL_SYSTEM_RAII::~SDL_SYSTEM_RAII()
{
	SDL_Quit();
}