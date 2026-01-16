#pragma once

#include <memory>
#include <SDL3/SDL_render.h>
#include "SDLCleanUp.h"
#include "Rectangle.h"

namespace badEngine {

	//Canvas is a texture type that is modifiable by rendering onto it. call start_drawing - > render something - > end_drawing

	class Canvas
	{
		using CanvasType = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;

	public:
		// constructs a blank canvas
		Canvas(Uint32 width, Uint32 height, SDL_Renderer* const renderer);

		// sets current rendering target as the texture
		bool start_drawing(SDL_Renderer* const renderer)const noexcept;
	
		// resets the current rendering target of the renderer
		bool end_drawing(SDL_Renderer* const renderer)const noexcept;

		// ref accessor for SDL_Texture
		SDL_Texture* const get()const noexcept;

	private:
		
		CanvasType mTexture;
	};
}
