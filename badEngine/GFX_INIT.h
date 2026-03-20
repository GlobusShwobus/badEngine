#pragma once

#include <SDL3/SDL_init.h>
#include "Window.h"
#include "Renderer.h"

namespace bad
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
	class GFX_INIT
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
		GFX_INIT(SDL_InitFlags flags);


		/**
		 * Shuts down the SDL system.
		 *
		 * Calls SDL_Quit().
		 */
		~GFX_INIT();

		GFX_INIT(const GFX_INIT&) = delete;
		GFX_INIT& operator=(const GFX_INIT&) = delete;
		GFX_INIT(GFX_INIT&&) = delete;
		GFX_INIT& operator=(GFX_INIT&&) = delete;
	};
}