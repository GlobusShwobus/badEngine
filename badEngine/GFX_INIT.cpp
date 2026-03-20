#include "pch.h"
#include "GFX_INIT.h"
#include <stdexcept>

bad::GFX_INIT::GFX_INIT(SDL_InitFlags flags)
{
	if (!SDL_Init(flags))
		throw std::runtime_error(std::string("Failed to init SDL: ") + SDL_GetError());
}

bad::GFX_INIT::~GFX_INIT()
{
	SDL_Quit();
}