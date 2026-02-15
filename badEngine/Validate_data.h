#pragma once

#include "NoThrowException.h"
#include "json.hpp"

//TODO:: cache lookups

namespace badEngine {
	// using filesystem validates that the given path has a non empty .json file
	// throws BasicException on failure
	NoThrowException validate_json_file(const char* path) noexcept;

	// runs checks verifying if the json is of expected structure
	// throws BasicException on failure
	NoThrowException validate_texture_manifest(const nlohmann::json& manifest, const char* key) noexcept;

	// runs checks verifying if the json is of expected structure
	// throws BasicException on failure
	NoThrowException validate_WindowContext_manifest(const nlohmann::json& manifest, const char* key) noexcept;
}