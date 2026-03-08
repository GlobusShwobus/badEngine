#pragma once

#include "bString.h"
#include "json.hpp"

namespace badEngine
{
	enum class expected_file_type : uint_fast8_t
	{
		DEFAULT_JSON,
		TEXTURE_JSON,
		WINDOW_JSON
	};

	namespace validate_details {
		//NOTE: these functions are deliberately vulgar and are meant to throw on failure
		void validate_texture_manifest(const nlohmann::json& manifest, const char* key);
		void validate_window_manifest(const nlohmann::json& manifest, const char* key);
		//more here in the future
	}

	//validate a json
	badCore::bString validate_json_file(const char* path, expected_file_type type = expected_file_type::DEFAULT_JSON) noexcept;
}