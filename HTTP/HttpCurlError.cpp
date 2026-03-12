#include "pch.h"
#include "HttpCurlError.h"

namespace badHTTP 
{
	curl_error::curl_error(const std::string& what, CURLcode code)
		: std::runtime_error(what + " [ " + curl_easy_strerror(code) + " ]") {}

	curl_error::curl_error(const std::string& what, CURLMcode code)
		: std::runtime_error(what + " [ " + curl_multi_strerror(code) + " ]") {}

	curl_error::curl_error(const std::string& what)
		: std::runtime_error(what) {}
}
