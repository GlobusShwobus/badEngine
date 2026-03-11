#pragma once

#include <SDL3/SDL_init.h>

namespace badWindow
{
	
	/// <summary>
	/// RAII wrapper that manages SDL system initialization and shutdown.
	///
	/// Creating an instance of this class calls SDL_Init() with the specified
	/// subsystem flags. When the object is destroyed, SDL_Quit() is called
	/// automatically.
	///
	/// The intended usage is to create a single instance in a scope that outlives
	/// all SDL objects (windows, renderers, etc.), typically near the beginning of
	/// `main()`.
	///
	/// This type is non-copyable and non-movable to ensure a single controlling
	/// instance.
	/// </summary>
	class SDL_SYSTEM_RAII
	{
	public:
		/**
	     * Initializes the SDL system.
	     *
	     * Calls SDL_Init() with the provided flags. If initialization fails,
	     * a std::runtime_error is thrown containing the SDL error message.
	     *
	     * \param flags SDL subsystem flags passed to SDL_Init().
	     *
	     * \throws std::runtime_error if SDL initialization fails.
	     */
		SDL_SYSTEM_RAII(SDL_InitFlags flags);


		/**
		 * Shuts down the SDL system.
		 *
		 * Calls SDL_Quit().
		 */
		~SDL_SYSTEM_RAII();

		SDL_SYSTEM_RAII(const SDL_SYSTEM_RAII&) = delete;
		SDL_SYSTEM_RAII& operator=(const SDL_SYSTEM_RAII&) = delete;
		SDL_SYSTEM_RAII(SDL_SYSTEM_RAII&&) = delete;
		SDL_SYSTEM_RAII& operator=(SDL_SYSTEM_RAII&&) = delete;
	};
}