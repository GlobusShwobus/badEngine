#pragma once

#include "Texture.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace badWindow
{
	/// <summary>
	/// A Wrapper for what is basically just an unordered_map of Textures with string keys to work well with json parsing.
	/// This class is responsible for the lifetime of Textures. 
	/// The Textures it shares with the world should never be deleted outside of the API provided by this class.
	/// </summary>
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
		TextureMap(const TextureMap&) = delete;
		TextureMap& operator=(const TextureMap&) = delete;
		~TextureMap()noexcept = default;

		/// <summary> Clears the internal storage of Textures. </summary>
		inline void clear()noexcept { mTextures.clear(); }

		/**
		* Adds a given texture to the internal storage with a given tag.
		* 
		* Denies textures that are nullptr, but does not throw.
		* 
		* Denies textures with a duplicate tag, but does not throw.
		* \param texture as rvalue
		* \param tag for association
		* \returns true of success or false on failure.
		* \throws insert itself does not throw but std::unordered_map may throw.
		*/
		bool insert(Texture&& texture, const std::string& tag);

		/**
		* Creates a texture with given parameters.
		* 
		* Internally calls make_texture provided in Texture.h.
		* 
		* Denies textures that are nullptr, but does not throw.
		* 
		* Denies textures with a duplicate tag, but does not throw.
		* \param renderer for rendering context
		* \param tag for association
		* \param surface from which to create a texture from
		* \return true on success or false on failure.
		* \throws insert itself does not throw but std::unordered_map may throw.
		*/
		bool insert(SDL_Renderer* const renderer, const std::string& tag, SDL_Surface* const surface);


		/**
		* Creates a texture with given parameters.
		*
		* Internally calls make_texture provided in Texture.h.
		*
		* Denies textures that are nullptr, but does not throw.
		*
		* Denies textures with a duplicate tag, but does not throw.
		* \param renderer for rendering context
		* \param tag for association
		* \param path on the filesystem from which to load a texture from.
		* \return true on success or false on failure.
		* \throws insert itself does not throw but std::unordered_map may throw.
		*/
		bool insert(SDL_Renderer* const renderer, const std::string& tag, const std::string& file_path);

		// deletes the texture associated with the tag, any references to the texture become invalidated.

		/// <summary>
		/// Deletes the texture associated with the tag, any references to the texture become invalidated.
		/// </summary>
		/// <param name="tag"> of deleted texture</param>
		void remove(const std::string& tag)noexcept;

		/// <summary>
		/// Checks if the asked tag is exists in the tree.
		/// </summary>
		/// <param name="tag"> for lookup</param>
		/// <returns> True if exists or false if not. </returns>
		bool has(const std::string& tag)const noexcept;

		/// <summary>
		/// Parses the tree collecting all of the tags and returns them.
		/// </summary>
		/// <returns> A list of all the tags. </returns>
		std::vector<std::string> get_tags()const;

		// returns SDL_Texture pointer. Given pointer should be used as a reference. Map owns the resource.
		// returns nullptr if tag does not exist in the map.

		/**
		* Parses the tree looking for the desired texture.
		* 
		* The given texture acts as a reference and should never be deleted manually in any way.
		* 
		* The cleanup is the responsibility of this class.
		* \param tag for lookup
		* \returns valid SDL_Texture or nullptr if the texture with given tag was not found.
		*/
		SDL_Texture* get_texture(const std::string& tag)const noexcept;

	private:
		/// <summary> A map of textures with strings as keys and Textures as values. </summary>
		Map mTextures;
	};
}