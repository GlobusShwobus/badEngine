#pragma once

#include <fstream>
#include <string_view>
#include "json.hpp"
#include "BadExceptions.h"

/*
TODO: can do something interesting with JSON custom parsing using parser_callback_t and parse_event_t combination, more on wiki
*/

namespace badEngine {

	class Config_JSON
	{
	public:
		Config_JSON(std::string_view path)
		{
			if (path.empty()) {
				throw BasicException("Config error", "Configuration path is empty");
			}

			std::string path_str(path);

			try {
				std::ifstream file(path_str);
				file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				file.seekg(0, std::ios::end);
				if (file.tellg() == 0) {
					throw BasicException(
						"Config error",
						"Configuration file is empty [" + path_str + "]"
					);
				}
				file.seekg(0, std::ios::beg);

				//PARAMS: no custom parser, allow exceptions, don't ignore trailing commas
				data = nlohmann::json::parse(file, nullptr, true, false);

			}
			catch (const std::ios_base::failure& e) {
				throw BasicException(
					"File IO error [" + path_str + "]: ", e.what()
				);
			}
			catch (const nlohmann::json::parse_error& e) {
				const int err_code = e.id;
				const std::size_t byte = e.byte;
				const std::string my_message = "JSON parse_error INFO: [code: " + std::to_string(err_code) + "; byte: " + std::to_string(byte) + "] PATH: [" + path_str + "]\n";
				throw BasicException(my_message, e.what());
			}
		}

		const nlohmann::json& get()const noexcept {
			return data;
		}
	private:
		nlohmann::json data;
	};
}