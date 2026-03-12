#include "pch.h"
#include "HttpTestCon.h"
#include "HttpUCURL.h"

namespace badHTTP
{
	CURLcode test_connection(const std::string& url, const std::string& pem)
	{
		UCURL ucurl(curl_easy_init());
		CURL* curl = ucurl.get();

		if (!curl)
			return CURLE_FAILED_INIT;

		CURLcode code;

		if (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); code != CURLE_OK)
			return code;

		if (code = curl_easy_setopt(curl, CURLOPT_CAINFO, pem.c_str()); code != CURLE_OK)
			return code;

		if (code = curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L); code != CURLE_OK)
			return code;

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

		return curl_easy_perform(curl);
	}
}