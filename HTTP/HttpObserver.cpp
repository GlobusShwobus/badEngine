#include "pch.h"
#include "HttpObserver.h"
#include "HttpCurlError.h"

namespace badHTTP
{
	Observer::Observer(CURL* config)
		:curl(curl_easy_duphandle(config)), state(observed_state::idle), mCurlm(nullptr)
	{
		if (!curl)
			throw std::runtime_error("curl_easy_init returned nullptr");
	}

	Observer::~Observer()noexcept
	{
		CURLM* curlmRemove = mCurlm.exchange(nullptr, std::memory_order_acq_rel);
		if (curlmRemove) {
			curl_multi_remove_handle(curlmRemove, curl.get());
		}
	}

	const HttpResponse* const Observer::try_get_data()const noexcept
	{
		// NOTE:: return by value is probably the safest but requires copies. so if things giga break
		// in regards to  this getter, return by val
		if (!is_finalized())
			return nullptr;
		return &data;
	}

	void Observer::abort()
	{
		observed_state expected = observed_state::idle;
		if (state.compare_exchange_strong(expected, observed_state::finalized, std::memory_order_acq_rel, std::memory_order_acquire)) {
			curl_easy_reset(curl.get());
			data.reset();
		}
	}

	void Observer::handle_finish_info()
	{
		//NOTE:: consider removing this all together if libcurl offers more detailed ways of result gathering but that would require exposing inner curl
		if (!is_finalized())
			return;

		//changed finalized to true, if old value is true returns (only false once)
		if (handle_info_set.exchange(true, std::memory_order_acq_rel))
			return;

		//http code
		long httpcode = 0;
		if (curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &httpcode) == CURLE_OK) {
			data.httpcode = httpcode;
		}
		else {
			data.httpcode = 0;
		}

		//content type if that field is set by content
		char* ct = nullptr;
		if (curl_easy_getinfo(curl.get(), CURLINFO_CONTENT_TYPE, &ct) == CURLE_OK && ct) {
			data.contentType = ct;
		}

		//http value ok
		data.http_ok = data.httpcode >= 200 && data.httpcode < 300;
	}

	observed_state Observer::get_state() const noexcept
	{
		return state.load(std::memory_order_acquire);
	}

	bool Observer::is_idle() const noexcept
	{
		return get_state() == observed_state::idle;
	}

	bool Observer::is_bound() const noexcept
	{
		return get_state() == observed_state::bound;
	}

	bool Observer::is_finalized() const noexcept
	{
		return get_state() == observed_state::finalized;
	}
}