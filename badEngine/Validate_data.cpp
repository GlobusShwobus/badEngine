#include "Validate_data.h"
#include <filesystem>
#include "mySDL_utils.h"

namespace badEngine 
{
	NoThrowException validate_json_file(const char* path) noexcept
	{
		if (!path)
			return NoThrowException::failure("Null path");

		std::filesystem::path p(path);

		if (!std::filesystem::exists(p))
			return NoThrowException::failure("File does not exist: " + std::string(path));

		if (!std::filesystem::is_regular_file(p))
			return NoThrowException::failure("Path is not a regular file: " + std::string(path));

		if (!p.has_extension())
			return NoThrowException(false, "File has no extension: " + std::string(path));
		std::string ext = p.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext != ".json")
			return NoThrowException::failure("File is not a .json: " +std::string(path));

		if (std::filesystem::file_size(p) == 0)
			return NoThrowException::failure("File is empty: " + std::string(path));

		return NoThrowException::success();
	}

	NoThrowException validate_texture_manifest(const nlohmann::json& manifest, const char* key) noexcept
	{
		if (!manifest.is_object())
			return NoThrowException::failure("Malformed json");

		if (!manifest.contains(key))
			return NoThrowException::failure("Key not found: " + std::string(key));

		const auto& config = manifest.at(key);

		if (!config.is_object())
			return NoThrowException::failure("Malformed json at key: " + std::string(key));

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			const std::string& tag = it.key();
			const auto& info = it.value();

			if (!info.is_object())
				return NoThrowException::failure("Type not an object : " + tag);

			if (!info.contains("file") || !info["file"].is_string())
				return NoThrowException::failure("Malformed 'file' field: " + tag);

			const std::string filepath = info.at("file");

			if (filepath.empty())
				return NoThrowException::failure("Empty 'file' field: " + tag);

			const std::filesystem::path path(filepath);

			std::string extension = path.extension().string();

			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

			// check for specific type. can extend it in the future
			if (extension != ".png")
				return NoThrowException::failure("Type extension not a .png: " + tag);

			// other fields here...

			// check if file actually exists
			if (!std::filesystem::exists(path))
				return NoThrowException::failure("File does not exist: " + path.string());
		}

		return NoThrowException::success();
	}
	NoThrowException validate_WindowContext_manifest(const nlohmann::json& manifest, const char* key) noexcept
	{
		if (!manifest.is_object())
			return NoThrowException::failure("Malformed json");

		if (!manifest.contains(key))
			return NoThrowException::failure("Key not found: " + std::string(key));

		const auto& config = manifest.at(key);

		if (!config.is_object()) {
			return NoThrowException::failure("Malformed json at key: " + std::string(key));
		}
		// heading
		if (!config.contains("heading") || !config["heading"].is_string()) {
			return NoThrowException::failure("Malformed 'heading' field: " + std::string(key));
		}
		// width and heigth
		if (!config.contains("window_width") || !config["window_width"].is_number_unsigned())
			return NoThrowException::failure("Malformed 'window_width' field: " + std::string(key));

		if (!config.contains("window_height") || !config["window_height"].is_number_unsigned())
			return NoThrowException::failure("Malformed 'window_height' field: " + std::string(key));

		if (config["window_width"].get<uint32_t>() == 0 || config["window_height"].get<uint32_t>() == 0)
			return NoThrowException::failure("Window width and height minimum requirement fail: " + std::string(key));
		// flags
		if (!config.contains("flags") || !config["flags"].is_array())
			return NoThrowException::failure("Malformed 'flags' field: " + std::string(key));

		for (const auto& flag : config["flags"]) {
			std::size_t dummy = 0;
			if (!flag.is_string()) {
				return NoThrowException::failure("All elements in 'flags' must be strings for key: " + std::string(key));
			}
			if (!badWindow::MapSDL_Flags_to_size_t_bitwise(flag.get<std::string>(), dummy)) {
				std::string flag_err = std::string("Unknown flag found: ") + flag.get<std::string>()+" at key: ";
				return NoThrowException::failure(flag_err + std::string(key));
			}
		}

		return NoThrowException::success();
	}
}