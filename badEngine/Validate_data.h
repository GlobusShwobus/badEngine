#pragma once

#include "json.hpp"

namespace badEngine {
	// using filesystem validates that the given path has a non empty .json file
	// throws BasicException on failure
	void validate_json_file(const char* path);

	// runs checks verifying if the json is of expected structure
	// throws BasicException on failure
	void validate_texture_manifest(const nlohmann::json& manifest, const char* key);

	// runs checks verifying if the json is of expected structure
	// throws BasicException on failure
	void validate_WindowContext_manifest(const nlohmann::json& manifest, const char* key);
}