#include "load_data.h"
#include <fstream>
#include "validate_data.h"
#include "EngineUtils.h"
#include <ranges>
#include <execution>

nlohmann::json bad::load_json(const char* path)
{
	std::ifstream file(path);
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	return nlohmann::json::parse(file, nullptr, true, false);
}

bad::WindowDescription bad::create_window_description(const nlohmann::json& manifest, const char* key)
{
	const auto& config = manifest.at(key);

	WindowDescription desc;
	desc.heading = config.at("heading").get<std::string>();
	desc.width = config.at("window_width").get<uint32_t>();
	desc.height = config.at("window_height").get<uint32_t>();

	desc.flags = 0;
	for (const auto& flag : config.at("flags"))
		json_key_to_sdl_flags(flag.get<std::string>().c_str(), desc.flags);

	return desc;
}

	// assumes the manifest is valid. provides no checks. if it indeed throws, program will be terminated.
bad::Sequence<bad::TextureDescription> bad::create_texture_descriptions(const nlohmann::json& texture_manifest, const char* key)
{
	const auto& config = texture_manifest.at(key);
	//make a view that makes texture descriptions and if it throws, it throws
	auto view = config.items() | std::views::transform([](const auto& item) {
		const auto& [tag, info] = item;
		return TextureDescription{ tag, info.at("file").get<std::string>() };
		}
	);

	return bad::Sequence<TextureDescription>(view.begin(), view.end());
}

bad::Sequence<std::pair<std::string, bad::Texture>> bad::load_textures(const bad::Sequence<bad::TextureDescription>& descs, SDL_Renderer* const renderer)
{
	if (!renderer)
		return {};

	bad::Sequence<std::pair<std::string, bad::Texture>> cont;
	cont.reserve(descs.size());

	for (const auto& desc : descs) {
		auto tex = bad::make_texture(renderer, desc.file.string().c_str());
		if (tex.get()) {
			cont.emplace_back(desc.tag, std::move(tex));
		}
	}

	return cont;
}