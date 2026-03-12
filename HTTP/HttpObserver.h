#pragma once

#include <curl/curl.h>
#include <atomic>
#include <condition_variable>
#include "HttpCallback.h"
#include "HttpUCURL.h"

namespace badHTTP
{
	enum class observed_state :uint8_t {
		idle,			//eligible but not yet performed, also default setting
		bound,			//currently executing in inside CURLM, once bound, can not stop execution
		finalized		//finished execution
	};

	class MultiCurl;
	
	//TODO:: consider observer exposing curl for setops. should only be legal to setopt if is_idle
	//TODO:: consider resetting if is_complete/is_idle and manually reattach to multicurl (kinda ehh considering currently observer constructor is private) but a good todo in the future
	
	//Observer is thread safe and is designed to be observable on multiple threads.
	//Observer mostly observes its own state. The state is managed by the MultiCurl that created it
	//except once finalized observer may finish up HttpResponse and recieve it
	class Observer final
	{
	private:

		friend class MultiCurl;
		explicit Observer(CURL* config);

	public:

		// if observed call is done (either successful run or released before performing) returns pointer to data
		// otherwise returns nullptr
		const HttpResponse* const try_get_data()const noexcept;

		// if the state is idle, meaning it is not yet bound nor complete, it will set the sate to complete
		// does nothing if is bound or complete because of guarantees that it gets detached from CURLM
		void abort();

		// does nothing if !is_complete() and only finalizes data once ever
		// sets the httpcode, http_ok and content type info for the handle
		// if this function is not called the body if the handle is still retrievable
		void handle_finish_info();
		
		// observed states
		observed_state get_state() const noexcept;
		bool is_idle() const noexcept;
		bool is_bound() const noexcept;
		bool is_finalized() const noexcept;

		~Observer()noexcept;

		Observer(const Observer&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(Observer&&) = delete;

	private:
		UCURL curl;
		HttpResponse data;
		std::atomic<observed_state> state;         //atomic state read write
		std::atomic<CURLM*> mCurlm;                //it is atomic as a failsafe mechanism. generally binding/unbinding is done by the creator of observer
		std::atomic_bool handle_info_set = false;  //a one time use atomic (depending on future TODO, should be removed)
	};
}