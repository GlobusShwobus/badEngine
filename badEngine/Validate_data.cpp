#include "Validate_data.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <execution>

#include <SDL3/SDL.h>
#include "EngineUtils.h"

namespace badEngine 
{
	badCore::bString validate_json_file(const char* path, expected_file_type type) noexcept
	{
		// phase 1: test basic file info
		if (!path)
			return badCore::bString::failure("Null path provided");

		std::filesystem::path p(path);

		if (!std::filesystem::exists(p))
			return badCore::bString::failure("File does not exist: " + std::string(path));

		if (!std::filesystem::is_regular_file(p))
			return badCore::bString::failure("Path is not a regular file: " + std::string(path));

		if (std::filesystem::file_size(p) == 0)
			return badCore::bString::failure("File is empty: " + std::string(path));

		// phase 2: test json fr
		try {
			std::ifstream in(path);
			in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			if (!in.is_open())
				return badCore::bString::failure("Cannot open file: " + std::string(path));

			nlohmann::json test = nlohmann::json::parse(in, nullptr, true, false);
			// phase 3: try to read specific json bodies expected by the system
			switch (type) {
			case expected_file_type::DEFAULT_JSON:
				break;
			case expected_file_type::TEXTURE_JSON:
				validate_details::validate_texture_manifest(test, "textures");
				break;
			case expected_file_type::WINDOW_JSON:
				validate_details::validate_window_manifest(test, "window");
				break;
			default:
				return badCore::bString::failure("Unknown file type specified");
			}

		}
		catch (const nlohmann::json::parse_error& e) {
			return badCore::bString::failure(
				"Invalid JSON format at byte " + std::to_string(e.byte) +
				": " + e.what());
		}
		catch (const std::exception& e) {
			return badCore::bString::failure(
				"JSON parsing error: " + std::string(e.what()));
		}
		catch (...) {
			return badCore::bString::failure("Unknown error while parsing JSON");
		}

		return badCore::bString::success();
	}

	namespace validate_details
	{
		void validate_texture_manifest(const nlohmann::json& manifest, const char* key)
		{
			const auto& textures = manifest.at(key);
			//testing out for each
			std::for_each(std::execution::seq, textures.begin(), textures.end(), [](const auto& item) {
				const std::string& tag = item.key();
				const auto& info = item.value();

				//try to grab the texture path
				std::string filepath = info.at("file");

				//check if path exists
				std::filesystem::path path(filepath);
				if (!std::filesystem::exists(path))
					throw std::runtime_error("Texture file does not exist: " + filepath + " for tag: " + tag);

				//check extension
				std::string ext = path.extension().string();
				std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

				if (ext != ".png")
					throw std::runtime_error("Texture must be PNG: " + filepath + " for tag: " + tag);
				}//lambda delimiter not if
			);
		}

		void validate_window_manifest(const nlohmann::json& manifest, const char* key)
		{
			const auto& window = manifest.at(key);

			//try to grab the values required
			std::string heading = window.at("heading");
			uint32_t width = window.at("window_width");
			uint32_t height = window.at("window_height");

			//check the value is not too dumb (maybe make some minimum resolution in the future)
			if (width == 0 || height == 0)
				throw std::runtime_error("Window dimensions must be positive");

			//validate falgs
			for (const auto& flag : window.at("flags")) {
				std::string flag_str = flag;
				std::size_t dummy = 0;
				if (!json_key_to_sdl_flags(flag_str, dummy))
					throw std::runtime_error("Unknown window flag: " + flag_str);
			}
		}
	}
}