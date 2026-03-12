#pragma once

#include <curl/curl.h>
#include <stdexcept>

namespace badHTTP
{
	struct curl_error :public std::runtime_error {
		explicit curl_error(const std::string& , CURLcode );

		explicit curl_error(const std::string& , CURLMcode );

		explicit curl_error(const std::string&);
	};
}