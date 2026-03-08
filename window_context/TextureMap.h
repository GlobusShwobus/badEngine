#pragma once

#include <unordered_map>
#include <string>
#include "Texture.h"
#include <vector>

namespace badWindow
{
	class TextureMap final
	{
		//turn off std::equal_to<std::string> default to allow lookups with const char*
		using Map = std::unordered_map<
			std::string,
			Texture,
			std::hash<std::string>,
			std::equal_to<>
		>;

	public:
		TextureMap() = default;

		// removes all elements from the map. all references to textures get invalidated. 
		void clear();

		bool insert(SDL_Renderer* const renderer, const std::string& tag, SDL_Surface* const surface);

		bool insert(SDL_Renderer* const renderer, const std::string& tag, const char* file_path);

		bool insert(Texture&& textr, const std::string& tag);

		// deletes the texture associated with the tag, any references to the texture become invalidated.
		void remove(const char* tag)noexcept;

		// returns SDL_Texture pointer. Given pointer should be used as a reference. Map owns the resource.
		// returns nullptr if tag does not exist in the map.
		SDL_Texture* get_texture(const char* tag)const noexcept;

		// check if the given tag exists in the map
		bool has(const char* tag)const noexcept;

		// returns all currently stored tags in a heap array
		std::vector<std::string> get_tags()const;

	private:
		Map mTextures;
	};
}