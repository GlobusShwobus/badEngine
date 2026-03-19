#pragma once
#include <string>
#include <filesystem>
#include "json.hpp"
#include "Sequence.h"
#include "Texture.h"
#include "SDL3/SDL_render.h"

namespace bad
{
	// basic load file into a json function. assumes complete validity thus does no checks. will throw on failure
	nlohmann::json load_json(const char* path);

	// a handle used to describe how a window should be initialized
	struct WindowDescription {
		std::string heading;
		uint32_t width;
		uint32_t height;
		uint64_t flags;
	};

	// extracts data from a json and creates a description for how a window should be initalized. assumes complete validity thus does no checks. will throw on failure
	WindowDescription create_window_description(const nlohmann::json& manifest, const char* key);

	// a handle used to describe a texture giving it a lable and a location path
	struct TextureDescription {
		std::string tag;
		std::filesystem::path file;
		//other fields?
	};

	// extracts data from a json and creates descriptions for textures. assumes complete validity thus does no checks. will throw on failure
	bad::Sequence<TextureDescription> create_texture_descriptions(const nlohmann::json& texture_manifest, const char* key);

	// loads up textures, assumes is called with the result of extract_texture_descs
	bad::Sequence<std::pair<std::string, bad::Texture>> load_textures(const bad::Sequence<TextureDescription>& descs, SDL_Renderer* const renderer);
}