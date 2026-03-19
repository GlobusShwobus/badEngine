#include "pch.h"
#include "badHTTP.h"
#include <stdexcept>

bad::HttpResponse bad::perform_writedata(CURL* template_handle)
{
	//user provided config must exist
	if (!template_handle)
		throw std::runtime_error("user provided config must exist. can not be nullptr");

	//duplicate it as to not mess with owners handle
	UCURL dupecurl(curl_easy_duphandle(template_handle));
	if (!dupecurl)
		throw std::runtime_error("curl_easy_duphandle failed");

	CURL* curl = dupecurl.get();

	//bind data
	HttpResponse handle;
	handle.body.reserve(16000);//reserve 16KB

	if (auto bind_function = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bad::write_callback); bind_function != CURLE_OK)
		throw std::runtime_error(std::string("Failed to set WRITEFUNCTION: ") + curl_easy_strerror(bind_function));

	if (auto bind_handle = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &handle); bind_handle != CURLE_OK)
		throw std::runtime_error(std::string("Failed to set CURLOPT_WRITEDATA: ") + curl_easy_strerror(bind_handle));

	//perform
	if (auto perform = curl_easy_perform(curl); perform != CURLE_OK)
		throw std::runtime_error(std::string("Failed to perform: ") + curl_easy_strerror(perform));

	//get info
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &handle.httpcode);
	handle.http_ok = handle.httpcode >= 200 && handle.httpcode < 300;

	char* ct = nullptr;
	if (curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct) == CURLE_OK && ct) {
		handle.contentType = ct;
	}

	handle.body.shrink_to_fit();//free some mem if any
	return handle;
}

bad::Observer::Observer(CURL* config)
	:curl(curl_easy_duphandle(config)), state(observed_state::idle), mCurlm(nullptr)
{
	if (!curl)
		throw std::runtime_error("curl_easy_init returned nullptr");
}

bad::Observer::~Observer()noexcept
{
	CURLM* curlmRemove = mCurlm.exchange(nullptr, std::memory_order_acq_rel);
	if (curlmRemove) {
		curl_multi_remove_handle(curlmRemove, curl.get());
	}
}

const bad::HttpResponse* const bad::Observer::try_get_data()const noexcept
{
	// NOTE:: return by value is probably the safest but requires copies. so if things giga break
	// in regards to  this getter, return by val
	if (!is_finalized())
		return nullptr;
	return &data;
}

void bad::Observer::abort()
{
	observed_state expected = observed_state::idle;
	if (state.compare_exchange_strong(expected, observed_state::finalized, std::memory_order_acq_rel, std::memory_order_acquire)) {
		curl_easy_reset(curl.get());
		data.reset();
	}
}

void bad::Observer::handle_finish_info()
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

bad::observed_state bad::Observer::get_state() const noexcept
{
	return state.load(std::memory_order_acquire);
}

bool bad::Observer::is_idle() const noexcept
{
	return get_state() == observed_state::idle;
}

bool bad::Observer::is_bound() const noexcept
{
	return get_state() == observed_state::bound;
}

bool bad::Observer::is_finalized() const noexcept
{
	return get_state() == observed_state::finalized;
}

bad::MultiCurl::MultiCurl()
	:mUcurlm(curl_multi_init())
{
	if (!mUcurlm)
		throw std::runtime_error(std::string("Failed to initalize curlm: "));

}

bad::MultiCurl::~MultiCurl() noexcept
{
	clear_handles();
}

void bad::MultiCurl::clear_handles()noexcept
{
	// in case MultiCurl goes out of scope of it's handles, and this is technically even realistic, then it's not enough from Observer destruction
	// because shared pointers get returned. this method provides guarantees that in any case CURL* gets removed from CURLM* before CURLM* goes out of scope
	if (mUcurlm) {
		for (auto& handle : mHandles) {
			if (!handle)
				continue;

			//only worry if the handle is for some magical reason bound
			observed_state expected = observed_state::bound;
			if (handle->state.compare_exchange_strong(expected, observed_state::finalized, std::memory_order_acq_rel, std::memory_order_acquire)) {
				// set the handles curlm to nullptr
				handle->mCurlm.exchange(nullptr, std::memory_order_acq_rel);
				// manually remove handle
				curl_multi_remove_handle(mUcurlm.get(), handle->curl.get());
			}
		}
	}
	//if idle or finalized or nullptr then safe to remove
	mHandles.clear();
}
std::shared_ptr<bad::Observer> bad::MultiCurl::add_handle(CURL* config)
{
	if (!config)
		throw std::runtime_error("Illegal config, config can't be nullptr");


	auto handle = std::shared_ptr<Observer>(new Observer(config));

	mHandles.push_back(handle);

	return handle;
}

void bad::MultiCurl::perform(std::size_t count, std::condition_variable* cv)
{
	static constexpr int MAX_CURLM_TIMEOUT = 1000; 			//TODO:: remove this hardcore sometime
	std::size_t bound = 0;

	for (auto& sp : mHandles) {
		if (bound >= count)           // we got our count so break
			break;
		if (sp == nullptr)            // for some reason nullptr so ignore
			continue;
		if (sp.use_count() == 1)      // multicurl is the only owner of a shared pointer so ignore
			continue;

		Observer& handle = *sp;
		// check if idle and set to bound if true
		observed_state expected = observed_state::idle;
		if (handle.state.compare_exchange_strong(expected, observed_state::bound, std::memory_order_acq_rel, std::memory_order_acquire)) {
			// reset and set writedata
			handle.data.reset();
			curl_easy_setopt(handle.curl.get(), CURLOPT_WRITEDATA, &handle.data);
			// set private ptr for O1 messages
			curl_easy_setopt(handle.curl.get(), CURLOPT_PRIVATE, sp.get());
			// set the handle info and store CURLM* in observer for emergency RAII
			handle.mCurlm.store(mUcurlm.get(), std::memory_order_relaxed);//not super important, can be relaxed since binding and unbinding is done manually in this object
			curl_multi_add_handle(mUcurlm.get(), handle.curl.get());
			bound++;
		}
	}
	// if nothing to do
	if (bound == 0)
		return;

	int running = 0;
	do {
		curl_multi_perform(mUcurlm.get(), &running);
		curl_multi_poll(mUcurlm.get(), nullptr, 0, MAX_CURLM_TIMEOUT, nullptr);

		int msgs = 0;

		while (CURLMsg* msg = curl_multi_info_read(mUcurlm.get(), &msgs))
		{
			if (msg->msg != CURLMSG_DONE)
				continue;

			CURL* easy = msg->easy_handle;
			if (!easy) // this should never happen under libcurls laws
				continue;

			Observer* handlePtr = nullptr;

			// consider throwing... TODO:: review if i should throw (or throw like after perform is done idk)
			if (curl_easy_getinfo(easy, CURLINFO_PRIVATE, &handlePtr) != CURLE_OK || !handlePtr)
				continue;

			handlePtr->mCurlm.store(nullptr, std::memory_order_relaxed);
			handlePtr->state.store(observed_state::finalized, std::memory_order_release);
			curl_multi_remove_handle(mUcurlm.get(), easy);
			curl_easy_setopt(easy, CURLOPT_PRIVATE, nullptr);

			if (cv)
				cv->notify_one();
		}

	} while (running > 0);

	// cleanup dead handles
	auto it = mHandles.begin();

	while (it != mHandles.end()) {

		auto& sp = *it;
		//if handle is nullptr, then we don't know what it is (though hard to imagine it happening at all), so remove it
		if (sp == nullptr) {
			it = mHandles.erase(it);
			continue;
		}
		//if ref count is 1, it means multicurl is the last holder of it, remove it
		if (sp.use_count() == 1) {
			it = mHandles.erase(it);
			continue;
		}

		const auto& handle = *sp;
		//only clean up finalized otherwise increment it
		if (handle.is_finalized()) {
			it = mHandles.erase(it);
		}
		else {
			++it;
		}
	}
}