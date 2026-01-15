#pragma once

#include <unordered_map>
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "Texture.h"
#include "Config_JSON.h"

namespace badEngine {

	class TextureLoader
	{
	public:

		TextureLoader(const Config_JSON& manifest, SDL_Renderer* renderer)
		{

			if (!renderer) {
				throw BasicException("Missing renderer");
			}
			const nlohmann::json& json = manifest.get();
			
			try {
				const auto& textures = json.at("textures");

				for (auto it = textures.begin(); it != textures.end(); ++it) {
					
					// key type for mTextures
					const std::string& tag = it.key();

					// potential other info in the future
					const auto& info = it.value();

					std::string filepath = info.at("file");

					if (filepath.empty()) {
						throw BasicException("Texture file missing for tag: " + tag, "Check manifest");
					}

					// create a texture
					SDL_Texture* sdlTexture = IMG_LoadTexture(renderer, filepath.c_str());

					if (!sdlTexture) {
						throw BasicException("Texture created as nullptr: " + tag, "Check manifest");
					}

					// wrap it in up
					mTextures.emplace(tag, sdlTexture);
				}
				
			}
			catch(const nlohmann::json::exception& e){
				const int err_code = e.id;
				const std::string my_message = "JSON exception INFO: [code: " + std::to_string(err_code) + "]";
				throw BasicException(my_message, e.what());
			}
			catch (const std::exception& e) {
				throw BasicException("Failed to load textures", e.what());
			}
		}

		//getter

	private:
		std::unordered_map<std::string, Texture> mTextures;
	};
}