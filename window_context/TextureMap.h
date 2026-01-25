#pragma once

#include <unordered_map>
#include <string>
#include "Texture.h"
#include "Sequence.h"

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

		// Creates a textures, or takes ownership if passed as rvalue.
		// if the given texture is not nullptr tries to insert it. 
		// no insertion done if there is a tag collision.
		template <typename... Args>
		bool add(const std::string& tag, Args&&... args)
			requires std::constructible_from<Texture, Args...>
		{
			Texture texture(std::forward<Args>(args)...);
			auto find = mTextures.find(tag);
			if (texture.get() != nullptr && find != mTextures.end()) {
				mTextures.emplace(tag, std::move(texture));
				return true;
			}
			return false;
		}

		// deletes the texture associated with the tag, any references to the texture become invalidated.
		void remove(const char* tag)noexcept;

		// returns SDL_Texture pointer. Given pointer should be used as a reference. Map owns the resource.
		// returns nullptr if tag does not exist in the map.
		SDL_Texture* get_texture(const char* tag)const noexcept;

		// check if the given tag exists in the map
		bool has(const char* tag)const noexcept;

		// returns all currently stored tags in a heap array
		badCore::Sequence<std::string> get_tags()const;

	private:
		Map mTextures;
	};
}