#include "Validate_data.h"
#include <filesystem>
#include <SDL3/SDL.h>
namespace badEngine 
{
	badCore::bString validate_json_file(const char* path) noexcept
	{
		if (!path)
			return badCore::bString::failure("Null path");

		std::filesystem::path p(path);

		if (!std::filesystem::exists(p))
			return badCore::bString::failure("File does not exist: " + std::string(path));

		if (!std::filesystem::is_regular_file(p))
			return badCore::bString::failure("Path is not a regular file: " + std::string(path));

		if (!p.has_extension())
			return badCore::bString::failure("File has no extension: " + std::string(path));
		std::string ext = p.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext != ".json")
			return badCore::bString::failure("File is not a .json: " +std::string(path));

		if (std::filesystem::file_size(p) == 0)
			return badCore::bString::failure("File is empty: " + std::string(path));

		return badCore::bString::success();
	}

	badCore::bString validate_texture_manifest(const nlohmann::json& manifest, const char* key) noexcept
	{
		if (!manifest.is_object())
			return badCore::bString::failure("Malformed json");

		if (!manifest.contains(key))
			return badCore::bString::failure("Key not found: " + std::string(key));

		const auto& config = manifest.at(key);

		if (!config.is_object())
			return badCore::bString::failure("Malformed json at key: " + std::string(key));

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			const std::string& tag = it.key();
			const auto& info = it.value();

			if (!info.is_object())
				return badCore::bString::failure("Type not an object : " + tag);

			if (!info.contains("file") || !info["file"].is_string())
				return badCore::bString::failure("Malformed 'file' field: " + tag);

			const std::string filepath = info.at("file");

			if (filepath.empty())
				return badCore::bString::failure("Empty 'file' field: " + tag);

			const std::filesystem::path path(filepath);

			std::string extension = path.extension().string();

			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

			// check for specific type. can extend it in the future
			if (extension != ".png")
				return badCore::bString::failure("Type extension not a .png: " + tag);

			// other fields here...

			// check if file actually exists
			if (!std::filesystem::exists(path))
				return badCore::bString::failure("File does not exist: " + path.string());
		}

		return badCore::bString::success();
	}
	badCore::bString validate_WindowContext_manifest(const nlohmann::json& manifest, const char* key) noexcept
	{
		if (!manifest.is_object())
			return badCore::bString::failure("Malformed json");

		if (!manifest.contains(key))
			return badCore::bString::failure("Key not found: " + std::string(key));

		const auto& config = manifest.at(key);

		if (!config.is_object()) {
			return badCore::bString::failure("Malformed json at key: " + std::string(key));
		}
		// heading
		if (!config.contains("heading") || !config["heading"].is_string()) {
			return badCore::bString::failure("Malformed 'heading' field: " + std::string(key));
		}
		// width and heigth
		if (!config.contains("window_width") || !config["window_width"].is_number_unsigned())
			return badCore::bString::failure("Malformed 'window_width' field: " + std::string(key));

		if (!config.contains("window_height") || !config["window_height"].is_number_unsigned())
			return badCore::bString::failure("Malformed 'window_height' field: " + std::string(key));

		if (config["window_width"].get<uint32_t>() == 0 || config["window_height"].get<uint32_t>() == 0)
			return badCore::bString::failure("Window width and height minimum requirement fail: " + std::string(key));
		// flags
		if (!config.contains("flags") || !config["flags"].is_array())
			return badCore::bString::failure("Malformed 'flags' field: " + std::string(key));

		for (const auto& flag : config["flags"]) {
			std::size_t dummy = 0;
			if (!flag.is_string()) {
				return badCore::bString::failure("All elements in 'flags' must be strings for key: " + std::string(key));
			}
			if (!MapSDL_Flags_to_size_t_bitwise(flag.get<std::string>(), dummy)) {
				std::string flag_err = std::string("Unknown flag found: ") + flag.get<std::string>()+" at key: ";
				return badCore::bString::failure(flag_err + std::string(key));
			}
		}

		return badCore::bString::success();
	}

	bool MapSDL_Flags_to_size_t_bitwise(const std::string& key, std::size_t& flags)noexcept
	{
		bool good_flag = false;

		if (key == "SDL_WINDOW_OPENGL") {
			flags |= SDL_WINDOW_OPENGL;
			good_flag = true;
		}
		if (key == "SDL_WINDOW_RESIZABLE") {
			flags |= SDL_WINDOW_RESIZABLE;
			good_flag = true;
		}
		//other flags here

		return good_flag;
	}
}