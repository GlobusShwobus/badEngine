#pragma once

#include <unordered_map>
#include <string>
#include <span>
#include "Texture.h"
#include "Sequence.h"

//TODO::load single texture
//		remove a single texture
//		review exception logic
//		documentation and cleaup

namespace badEngine {

	class TextureMap
	{
		// required to avoid look up bs with const char*. by default std::equal_to<std::string>, so set it manually turn it off
		using Map = std::unordered_map<
			std::string,
			Texture,
			std::hash<std::string>,
			std::equal_to<>
		>;
	public:
		TextureMap() = default;

		TextureMap(std::span<std::pair<std::string, Texture>> textures)
		{
			load_bulk(textures);
		}

		void clear() noexcept
		{
			mTextures.clear();
		}

		void load_bulk(std::span<std::pair<std::string, Texture>> textures)noexcept
		{
			for (auto& [tag, texture] : textures) {
				if (texture.get() != nullptr) {
					mTextures.try_emplace(tag, std::move(texture));
				}
			}
		}
		// can throw
		const Texture& get(const char* tag)const
		{
			auto it = mTextures.find(tag);
			if (it == mTextures.end()) {
				throw BasicException("Missing or invalid tag: ", tag);
			}
			return it->second;
		}

		// returns nullptr on fail
		const Texture* const try_get(const char* tag) const noexcept {
			auto it = mTextures.find(tag);
			return it == mTextures.end() ? nullptr : &it->second;
		}

		bool has(const char* tag)const noexcept
		{
			return mTextures.find(tag) != mTextures.end();
		}

		Sequence<std::string> get_tags()const noexcept {
			Sequence<std::string> tags;
			tags.set_capacity(mTextures.size());
			for (const auto& pair : mTextures) {
				tags.push_back(pair.first);
			}
			return tags;
		}

	private:
		Map mTextures;
	};
}