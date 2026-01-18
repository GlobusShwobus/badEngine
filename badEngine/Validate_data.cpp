#include "Validate_data.h"
#include <filesystem>
#include "mySDL_utils.h"
#include "BadExceptions.h"
namespace badEngine {

	void validate_json_file(const char* path)
	{
		if (!path)
			throw BasicException("Null path");

		std::filesystem::path p(path);

		if (!p.has_extension())
			throw BasicException("File has no extension", path);
		std::string ext = p.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext != ".json")
			throw BasicException("File is not a .json", path);

		if (!std::filesystem::exists(p))
			throw BasicException("File does not exist", path);

		if (!std::filesystem::is_regular_file(p))
			throw BasicException("Path is not a regular file", path);

		if (std::filesystem::file_size(p) == 0)
			throw BasicException("File is empty", path);
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
	void validate_WindowContext_manifest(const nlohmann::json& manifest, const char* key)
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
			std::size_t dummy = 0;
			if (!flag.is_string()) {
				throw BasicException("All elements in 'flags' must be strings for key", key);
			}
			if (!MapSDL_Flags_to_size_t_bitwise(flag.get<std::string>(), dummy)) {
				throw BasicException(std::string("Unknown flag found: ") + flag.get<std::string>(), key);
			}
		}
	}
}