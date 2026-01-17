#pragma once

#include <unordered_map>
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "Texture.h"
#include "json.hpp"
#include "BadExceptions.h"
#include "Sequence.h"


//TODO::load single texture
//		remove a single texture
//		review exception logic
//		documentation and cleaup

namespace badEngine {

	class TextureLoader
	{
		// required to avoid look up bs with const char*. by default std::equal_to<std::string>, so set it manually turn it off
		using Map = std::unordered_map<
			std::string,
			Texture,
			std::hash<std::string>,
			std::equal_to<>
		>;
	public:

		TextureLoader(const nlohmann::json& manifest, SDL_Renderer* renderer)
		{
			load(manifest, renderer);
		}

		void clear() noexcept
		{
			mTextures.clear();
		}

		void reload(const nlohmann::json& manifest, SDL_Renderer* renderer) {
			clear();
			load(manifest, renderer);
		}
		void load(const nlohmann::json& manifest, SDL_Renderer* renderer)
		{
			if (!renderer)
				throw BasicException("Missing renderer");

			if (mTextures.size() < manifest.size())
				mTextures.reserve(manifest.size());

			for (auto it = manifest.begin(); it != manifest.end(); ++it)
			{
				const std::string& tag = it.key();
				const auto& info = it.value();

				std::string filepath = info.at("file");
				//other fields here

				// create a texture
				Texture texture(filepath.c_str(), renderer);

				if (texture.get() == nullptr)
					throw BasicException("Texture created as nullptr: " + tag, "Check manifest");

				auto [_, inserted] = mTextures.emplace(tag, std::move(texture));
				if (!inserted)
					throw BasicException("Duplicate texture tag: " + tag);
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