#include "load_data.h"
#include <fstream>
#include "validate_data.h"
#include "EngineUtils.h"
#include <ranges>
#include <execution>

namespace badEngine {

	nlohmann::json load_json(const char* path)
	{
		std::ifstream file(path);
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		return nlohmann::json::parse(file, nullptr, true, false);
	}

	WindowDescription create_window_description(const nlohmann::json& manifest, const char* key)
	{
		const auto& config = manifest.at(key);

		WindowDescription desc;
		desc.heading = config.at("heading").get<std::string>();
		desc.width   = config.at("window_width").get<uint32_t>();
		desc.height  = config.at("window_height").get<uint32_t>();

		desc.flags = 0;
		for (const auto& flag : config.at("flags"))
			json_key_to_sdl_flags(flag.get<std::string>().c_str(), desc.flags);

		return desc;
	}

	// assumes the manifest is valid. provides no checks. if it indeed throws, program will be terminated.
	badCore::Sequence<TextureDescription> create_texture_descriptions(const nlohmann::json& texture_manifest, const char* key)
	{
		const auto& config = texture_manifest.at(key);
		//make a view that makes texture descriptions and if it throws, it throws
		auto view = config.items() | std::views::transform([](const auto& item) {
			const auto& [tag, info] = item;
			return TextureDescription{ tag, info.at("file").get<std::string>() };
			}
		);

		return badCore::Sequence<TextureDescription>(std::move(view));
	}

	badCore::Sequence<std::pair<std::string, badWindow::Texture>> load_textures(const badCore::Sequence<TextureDescription>& descs, SDL_Renderer* const renderer)
	{
		if (!renderer)
			return {};

		//make a view that tries to call a texture constructor and makes a pair out of a tag and texture. secondly it filters out nullptr
		auto view = descs | std::views::transform([renderer](const TextureDescription& desc) {
			badWindow::Texture tex = badWindow::make_texture(renderer, desc.file.string().c_str());
			//log maybe?
			return std::make_pair(desc.tag, std::move(tex));
			}
		) | std::views::filter([](const auto& pair) {return pair.second.get(); });
		
		
		return badCore::Sequence<std::pair<std::string, badWindow::Texture>>(std::move(view));
	}
}