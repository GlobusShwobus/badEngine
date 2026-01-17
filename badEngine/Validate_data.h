#pragma once

#include "json.hpp"

namespace badEngine {
	// basic json load up and validation-  check File IO related data and attempts to parse the file into a json
	// throws BasicException on file IO failure
	// throws json::parse_error if fails to parse into the json
	void load_and_validate_JSON(nlohmann::json& json, const char* path);

	// validates a json to fit exactly into a texture loader
	// throws BasicException on various checks related to json structure
	// may throw other json exceptions
	void validate_texture_manifest(const nlohmann::json& manifest, const char* key);

	// validates a json to fit exactly into a graphics system initalizer
	// throws BasicException on various checks related to json structure
	// flag field is checkd for existance and type but not contents
	void validate_GraphicsSys_manifest(const nlohmann::json& manifest, const char* key);

	//load and boot then pass
	struct ValidatedTextureData {

	};

	struct ValidatedGFXData {

	};
}