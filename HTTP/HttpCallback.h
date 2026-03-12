#pragma once
#include <string>

namespace badHTTP
{
	struct HttpResponse 
	{
		std::string body;
		std::string contentType;
		long httpcode = 0;
		bool http_ok = false;

		explicit operator bool() const noexcept
		{
			return http_ok;
		}

		inline void reset() noexcept
		{
			body.clear();
			contentType.clear();
			httpcode = 0;
			http_ok = false;
		}
	};

	static std::size_t callback(char* content, std::size_t size, std::size_t nmemb, void* clientp)
	{
		auto* resp = static_cast<HttpResponse*>(clientp);
		resp->body.append(content, size * nmemb);

		return size * nmemb;
	}
}