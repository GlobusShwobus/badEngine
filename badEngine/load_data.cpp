
#include "load_data.h"
#include <fstream>
#include "mySDL_utils.h"

namespace badEngine {

	nlohmann::json load_json(const char* path)
	{
		std::ifstream file(path);
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		return nlohmann::json::parse(file, nullptr, true, false);
	}

	// assumes the manifest was validated. provides no checks
	WindowContextDescription extract_WindowContext_desc(const nlohmann::json& manifest, const char* key)
	{
		const auto& config = manifest.at(key);

		WindowContextDescription desc;
		desc.heading = config.at("heading").get<std::string>();
		desc.width = config.at("window_width").get<uint32_t>();
		desc.height = config.at("window_height").get<uint32_t>();

		desc.flags = 0;
		for (const auto& flag : config.at("flags")) {
			badWindow::MapSDL_Flags_to_size_t_bitwise(flag.get<std::string>().c_str(), desc.flags);
		}
		return desc;
	}

	// assumes the manifest is valid. provides no checks. if it indeed throws, program will be terminated.
	badCore::Sequence<TextureDescription> extract_texture_descs(const nlohmann::json& texture_manifest, const char* key)
	{
		badCore::Sequence<TextureDescription>textures;

		const auto& config = texture_manifest.at(key);
		textures.set_capacity(config.size());

		for (const auto& [tag, info] : config.items()) {
			textures.emplace_back( tag, info.at("file").get<std::string>() );
		}

		return textures;
	}

	badCore::Sequence<std::pair<std::string, badWindow::Texture>> load_textures(const badCore::Sequence<TextureDescription>& descs, SDL_Renderer* renderer)
	{
		badCore::Sequence<std::pair<std::string, badWindow::Texture>> textures;

		if (!renderer) return textures;
		textures.set_capacity(descs.size());

		for (const auto& desc : descs) {
			badWindow::Texture tex(desc.file.string().c_str(), renderer);

			if (!tex.get()) {
				// log maybe?
				continue;
			}

			textures.emplace_back(desc.tag, std::move(tex));
		}
		return textures;
	}
}