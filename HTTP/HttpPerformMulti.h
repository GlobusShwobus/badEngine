#pragma once
#include "HttpUCURL.h"
#include "HttpObserver.h"
#include "Sequence.h"

namespace badHTTP
{
	//MultiCurl is not threadsafe
	class MultiCurl final
	{
	public:
		explicit MultiCurl();

		void clear_handles()noexcept;

		bool is_empty()const noexcept;

		//attempts to duplicate the provided config. unlike a single curl_perform_get MultiCurl does no option setting except CURLOPT_WRITEDATA
		//everything else from cert, url and other options are user resp.
		std::shared_ptr<Observer> add_handle(CURL* config);

		//count is how many handles to perform, first in first out
		//optional condition variable for listening from another thread
		void perform(std::size_t count, std::condition_variable* cv = nullptr);

		~MultiCurl() noexcept;
		MultiCurl(const MultiCurl&) = delete;
		MultiCurl& operator=(const MultiCurl&) = delete;
		MultiCurl(MultiCurl&&) noexcept = delete;
		MultiCurl& operator=(MultiCurl&&) noexcept = delete;

	private:
		UCURLM mUcurlm;
		badCore::Sequence<std::shared_ptr<Observer>> mHandles;
	};
}