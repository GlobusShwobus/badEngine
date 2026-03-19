#pragma once

#include <string>
#include <memory>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <assert.h>

namespace bad
{
	/// <summary> Functor deleter for SDL_Texture </summary>
	struct TextureDeleter {
		void operator()(SDL_Texture* t)
		{
			if (t)
				SDL_DestroyTexture(t);
		}
	};

	/// <summary> Wrapper for SDL_Texture using a unique_ptr with a custom functor deleter </summary>
	using Texture = std::unique_ptr<SDL_Texture, TextureDeleter>;

	/**
	* Create a texture from an existing surface.
	*
	* The surface is not modified or freed by this function.
	*
	* Created Texure is static- it cannot be modified.
	*
	* For more detailed documentation consult SDL_CreateTextureFromSurface.
	* \param renderer the rendering context
	* \param surface the SDL_Surface structure containing pixel data used to fill the texture
	* \returns a unique_ptr texture or NULL on failure; call SDL_GetError() for more info.
	* \throws in DEBUG asserts renderer and source to not be nullptr
	* \throws noexcept
	*/
	inline Texture make_texture(SDL_Renderer* const renderer, SDL_Surface* const surface) noexcept
	{
		assert(renderer != nullptr);
		assert(surface != nullptr);
		return bad::Texture{ SDL_CreateTextureFromSurface(renderer, surface) };
	}

	/**
	* Create a texture from a filesystem path.
	*
	* Created Texure is static- it cannot be modified.
	*
	* Currently only .jpeg and .png are supported. For more extensions install SDL_Image sub repos.
	*
	* For more detailed documentation consult SDL_Image.h library IMG_LoadTexture function.
	* \param renderer the rendering context
	* \param path for a file on the filesystem to load the image
	* \returns a unique_ptr texture or NULL on failure; call SDL_GetError() for more info.
	* \throws in DEBUG asserts renderer to not be nullptr
	* \throws noexcept
	*/
	inline Texture make_texture(SDL_Renderer* const renderer, const std::string& path) noexcept
	{
		assert(renderer != nullptr);
		return bad::Texture{ IMG_LoadTexture(renderer, path.c_str()) };
	}
}