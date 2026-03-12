#include "pch.h"
#include "HttpPerform.h"
#include <stdexcept>
#include "HttpCurlError.h"
#include "HttpUCURL.h"
namespace badHTTP
{
	HttpResponse perform_custom(CURL* config)
	{
		//user provided config must exist
		if (!config)
			throw std::runtime_error("user provided config must exist. can not be nullptr");

		//duplicate it as to not mess with owners handle
		UCURL dupecurl(curl_easy_duphandle(config));
		if (!dupecurl)
			throw std::runtime_error("curl_easy_duphandle failed");

		CURL* curl = dupecurl.get();

		//bind data
		HttpResponse handle;

		if (auto bind_function = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback); bind_function != CURLE_OK)
			throw curl_error("Failed to set WRITEFUNCTION: ", bind_function);

		if (auto bind_handle = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &handle); bind_handle != CURLE_OK)
			throw curl_error("Failed to set CURLOPT_WRITEDATA: ", bind_handle);

		//perform
		if (auto perform = curl_easy_perform(curl); perform != CURLE_OK)
			throw curl_error("Failed to perform: ", perform);

		//get info
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &handle.httpcode);
		handle.http_ok = handle.httpcode >= 200 && handle.httpcode < 300;

		char* ct = nullptr;
		if (curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct) == CURLE_OK && ct) {
			handle.contentType = ct;
		}
		return handle;
	}
}