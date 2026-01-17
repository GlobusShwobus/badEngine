#pragma once

#include "json.hpp"

namespace badEngine {


	struct GFX_loadup {
		void* window = nullptr;
		void* renderer = nullptr;
	};


	// assumes a validity check has been run. if it fails it will fail silently returning data as nullptr
	GFX_loadup load_GraphicsSys_data(const nlohmann::json& manifest, const char* key) noexcept;

	void SDL_Flag_string_to_uint64(const nlohmann::json_abi_v3_11_3::json& key, std::size_t& flags);
}