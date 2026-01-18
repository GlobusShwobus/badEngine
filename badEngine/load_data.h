#pragma once

#include <string>
#include <filesystem>
#include "json.hpp"
#include "Sequence.h"
#include "Texture.h"
#include "SDL3/SDL_render.h"

namespace badEngine {
	// assumes path if verified, provides no checks and can throw anything if malformed input
	// verify before or trust
	nlohmann::json load_json(const char* path);

	struct WindowContextDescription {
		std::string heading;
		uint32_t width;
		uint32_t height;
		uint64_t flags;
	};

	// assumes path if verified, provides no checks and can throw anything if malformed input
	// verify before or trust
	WindowContextDescription extract_WindowContext_desc(const nlohmann::json& manifest, const char* key);


	struct TextureDescription {
		std::string tag;
		std::filesystem::path file;
		//other fields?
	};

	// assumes path if verified, provides no checks and can throw anything if malformed input
	// verify before or trust
	Sequence<TextureDescription> extract_texture_descs(const nlohmann::json& texture_manifest, const char* key);

	// loads up textures, assumes is called with the result of extract_texture_descs
	Sequence<std::pair<std::string, Texture>> load_textures(const Sequence<TextureDescription>& descs, SDL_Renderer* renderer);
}