#pragma once

#include <unordered_map>
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "Texture.h"
#include "json.hpp"
#include "BadExceptions.h"
#include "Sequence.h"

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
		void load_from_section(const nlohmann::json& manifest, const char* key, SDL_Renderer* renderer) {
			load(manifest.at(key), renderer);
		}

		const Texture& get_texture(const char* tag)const
		{
			auto it = mTextures.find(tag);
			return(it != mTextures.end()) ? it->second : mSentinel;
		}

		bool has(const char* tag)const noexcept
		{
			return mTextures.find(tag) != mTextures.end();
		}

		Sequence<std::string> get_tags()const {
			Sequence<std::string> tags;
			tags.set_capacity(mTextures.size());
			for (const auto& pair : mTextures) {
				tags.push_back(pair.first);
			}
			return tags;
		}

	private:

		void load(const nlohmann::json& manifest, SDL_Renderer* renderer)
		{
			if (!renderer) {
				throw BasicException("Missing renderer");
			}

			try {
				mTextures.reserve(manifest.size());
				for (auto it = manifest.begin(); it != manifest.end(); ++it) {

					// key type for mTextures
					const std::string& tag = it.key();

					// potential other info in the future
					const auto& info = it.value();

					std::string filepath = info.at("file");

					if (filepath.empty()) {
						throw BasicException("Texture file missing for tag: " + tag, "Check manifest");
					}

					// create a texture
					Texture texture(filepath.c_str(), renderer);

					if (texture.get() == nullptr) {
						throw BasicException("Texture created as nullptr: " + tag, "Check manifest");
					}

					auto [_, inserted] = mTextures.emplace(tag, std::move(texture));
					if (!inserted) {
						throw BasicException("Duplicate texture tag: " + tag);
					}
				}

			}
			catch (const nlohmann::json::exception& e) {
				const int err_code = e.id;
				const std::string my_message = "JSON exception INFO: [code: " + std::to_string(err_code) + "]";
				throw BasicException(my_message, e.what());
			}
			catch (const std::exception& e) {
				throw BasicException("Failed to load textures", e.what());
			}
		}

	private:
		Map mTextures;
		Texture mSentinel;
	};
}