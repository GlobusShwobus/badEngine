#include "load_data.h"

#include <string>
#include <SDL3/SDL_init.h>
#include <SDL3//SDL_render.h>
#include <SDL3/SDL_video.h>

namespace badEngine {
	GFX_loadup load_GraphicsSys_data(const nlohmann::json& manifest, const char* key) noexcept
	{
		if (!manifest.contains(key))
			return {};
		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
			return {};

		const auto& config = manifest.at(key);

		//default values
		std::string heading("default");
		Uint32 width = 960;
		Uint32 height = 540;
		std::size_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		if (config.contains("heading") && config["heading"].is_string())
			heading = config.at("heading");

		if (config.contains("window_width") && config["window_width"].is_number_integer()) {
			width = config["window_width"].get<Uint32>();
		}
		if (config.contains("window_height") && config["window_height"].is_number_integer()) {
			height = config["window_height"].get<Uint32>();
		}
		if (config.contains("flags") && config["flags"].is_array()) {
			const auto& flag_arry = config.at("flags");
			for (const auto& flag : flag_arry) {
				if (flag.is_string()) {
					SDL_Flag_string_to_uint64(flag, flags);
				}
			}
		}
		SDL_Window* window = SDL_CreateWindow(heading.data(), width, height, flags);
		if (!window) {
			return {};
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
		if (!renderer) {
			SDL_DestroyWindow(window);
			return {};
		}
		return { window, renderer };
	}

	void SDL_Flag_string_to_uint64(const nlohmann::json_abi_v3_11_3::json& key, std::size_t& flags)
	{
		if (key == "SDL_WINDOW_OPENGL")
			flags |= SDL_WINDOW_OPENGL;
		if (key == "SDL_WINDOW_RESIZABLE")
			flags |= SDL_WINDOW_RESIZABLE;
	}
}