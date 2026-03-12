#include "pch.h"
#include "HttpPerformMulti.h"
#include "HttpCurlError.h"
#include <atomic>
#include <condition_variable>

namespace badHTTP
{
	MultiCurl::MultiCurl()
		:mUcurlm(curl_multi_init())
	{
		if (!mUcurlm)
			throw curl_error("Failed to initalize curlm", CURLE_FAILED_INIT);

	}

	MultiCurl::~MultiCurl() noexcept
	{
		clear_handles();
	}

	void MultiCurl::clear_handles()noexcept
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

	bool MultiCurl::is_empty()const noexcept {
		return mHandles.isEmpty();
	}

	std::shared_ptr<Observer> MultiCurl::add_handle(CURL* config)
	{
		if (!config)
			throw curl_error("Illegal config, config can't be nullptr");


		auto handle = std::shared_ptr<Observer>(new Observer(config));

		mHandles.push_back(handle);

		return handle;
	}

	void MultiCurl::perform(std::size_t count, std::condition_variable* cv)
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
}
