#pragma once

#include <SDL3/SDL_init.h>

namespace badWindow
{
	class SDL_SYSTEM_RAII
	{
	public:
		SDL_SYSTEM_RAII(SDL_InitFlags flags);

		~SDL_SYSTEM_RAII();

		SDL_SYSTEM_RAII(const SDL_SYSTEM_RAII&) = delete;
		SDL_SYSTEM_RAII& operator=(const SDL_SYSTEM_RAII&) = delete;
		SDL_SYSTEM_RAII(SDL_SYSTEM_RAII&&) = delete;
		SDL_SYSTEM_RAII& operator=(SDL_SYSTEM_RAII&&) = delete;
	};
}