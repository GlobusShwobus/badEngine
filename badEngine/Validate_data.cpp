#include "Validate_data.h"
#include <fstream>
#include <filesystem>
#include "BadExceptions.h"
namespace badEngine {

	void load_and_validate_JSON(nlohmann::json& json, const char* path) {
		if (!path)
			throw BasicException("Nullptr path entered");

		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path);

		if (!file.is_open())
			throw BasicException("File failed to open", path);

		file.seekg(0, std::ios::end);
		if (file.tellg() == 0)
			throw BasicException("Empty file", path);

		file.seekg(0, std::ios::beg);

		//PARAMS: no custom parser, allow exceptions, don't ignore trailing commas
		json = nlohmann::json::parse(file, nullptr, true, false);
	}

	void validate_texture_manifest(const nlohmann::json& manifest, const char* key)
	{
		if (!manifest.is_object())
			throw BasicException("type not an object");

		if (!manifest.contains(key))
			throw BasicException("does not contain key", key);

		const auto& config = manifest.at(key);

		if (!config.is_object())
			throw BasicException("malformed key type, must be object", key);

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			const std::string& tag = it.key();
			const auto& info = it.value();

			if (!info.is_object())
				throw BasicException("type not an object", tag);

			if (!info.contains("file") || !info["file"].is_string())
				throw BasicException("malformed 'file' field", tag);

			const std::string filepath = info.at("file");

			if (filepath.empty())
				throw BasicException("empty 'file' field", tag);

			const std::filesystem::path path(filepath);

			std::string extension = path.extension().string();

			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

			// check for specific type. can extend it in the future
			if (extension != ".png")
				throw BasicException("Type check fail: file is not a .png", tag);

			// other fields here...

			// check if file actually exists
			if (!std::filesystem::exists(path))
				throw BasicException("File does not exist", path.string());
		}
	}
	void validate_GraphicsSys_manifest(const nlohmann::json& manifest, const char* key)
	{
		if (!manifest.is_object())
			throw BasicException("type not an object");

		if (!manifest.contains(key))
			throw BasicException("does not contain key", key);

		const auto& config = manifest.at(key);

		if (!config.is_object()) {
			throw BasicException("type not an object", key);
		}
		// heading
		if (!config.contains("heading") || !config["heading"].is_string()) {
			throw BasicException("malformed 'heading' field", key);
		}
		// width and heigth
		if (!config.contains("window_width") || !config["window_width"].is_number_unsigned())
			throw BasicException("malformed 'window_width' field", key);

		if (!config.contains("window_height") || !config["window_height"].is_number_unsigned())
			throw BasicException("malformed 'window_height' field", key);

		if (config["window_width"].get<uint32_t>() == 0 || config["window_height"].get<uint32_t>() == 0)
			throw BasicException("window width and height minimum requirement fail", key);
		// flags
		if (!config.contains("flags") || !config["flags"].is_array())
			throw BasicException("malformed 'flags' field", key);

		for (const auto& flag : config["flags"]) {
			if (!flag.is_string()) {
				throw BasicException("All elements in 'flags' must be strings for key", key);
			}
		}
	}
}