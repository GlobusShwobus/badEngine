#pragma once
#include <string>
#include <curl/curl.h>

namespace badHTTP
{
	CURLcode test_connection(const std::string& url, const std::string& pem);
}