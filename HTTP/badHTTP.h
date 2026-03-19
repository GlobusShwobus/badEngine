#pragma once
#include <curl/curl.h>
#include <memory>
#include <string>
#include <atomic>
#include <condition_variable>
#include <vector>

namespace bad
{
	/// <summary> Functor deleter for CURL handle </summary>
	struct CurlDeleter {
		void operator()(CURL* c)const {
			if (c)
				curl_easy_cleanup(c);
		}
	};

	/// <summary> Wrapper for CURL using a unique_ptr with a custom functor deleter </summary>
	using UCURL = std::unique_ptr<CURL, CurlDeleter>;

	/// <summary> Functor deleter for CURLM handle. Does not check if curlm contains curls. </summary>
	struct CurlmDeleter {
		void operator()(CURLM* m) {
			if (m)
				curl_multi_cleanup(m);
		}
	};

	/// <summary> Wrapper for CURLM using a unique_ptr with a custom functor deleter. Does not check if curlm contains curls. </summary>
	using UCURLM = std::unique_ptr<CURLM, CurlmDeleter>;

	/// <summary> A struct for grouping all relevant data in http communication. </summary>
	struct HttpResponse
	{
		std::string body;		/// <summary> If a call sends data it is stored as byte info using UTF-8 in a string which supports being generic and mem safe. </summary>
		std::string contentType;/// <summary> content type header </summary>
		long httpcode = 0;		/// <summary> http result code </summary>
		bool http_ok = false;	/// <summary> Convenience bool for checking if call was successful. </summary>

		/// <summary> Convenience method to easily weed out bad results. </summary>
		explicit operator bool() const noexcept
		{
			return http_ok;
		}

		/// <summary> Resets HttpResponce to base value. Without resetting, adding new data ontop of old, is UB. </summary>
		inline void reset() noexcept
		{
			body.clear();
			contentType.clear();
			httpcode = 0;
			http_ok = false;
		}
	};

	/**
	* Called by libcurl whenever a chunk of response data is received. This function is a replica of fwrite(...).
	*
	* \param content Pointer to the received bytes
	* \param size Size of one data element (usually 1)
	* \param nmemb Number of elements received
	* \param clientp User pointer passed via CURLOPT_WRITEDATA (though param is void*, internally hardcoded to HttpResponse)
	* \returns total size of bytes recieved using ( size * nmemb )
	*/
	static std::size_t write_callback(char* content, std::size_t size, std::size_t nmemb, void* clientp)
	{
		auto* resp = static_cast<HttpResponse*>(clientp);
		resp->body.append(content, size * nmemb);

		return size * nmemb;
	}
	/**
	* Performs a web write call using a given curl as config.
	* \param template_handle CURL handle. Internally this function makes a duplicate of the CURL leaving the original unchanged.
	* \throws std::runtime_error if:
	*   - config is nullptr
	*   - curl_easy_duphandle fails
	*   - curl_easy_setopt fails
	*   - curl_easy_perform fails
	* \returns HttpResponse with all members set
	*/
	HttpResponse perform_writedata(CURL* template_handle);

	/// <summary> Observer states </summary>
	enum class observed_state :uint8_t {
		idle,			//eligible but not yet performed, also default setting
		bound,			//currently executing in inside CURLM, once bound, can not stop execution
		finalized		//finished execution
	};

	class MultiCurl;

	//TODO:: consider observer exposing curl for setops. should only be legal to setopt if is_idle
	//TODO:: consider resetting if is_complete/is_idle and manually reattach to multicurl (kinda ehh considering currently observer constructor is private) but a good todo in the future

	/**
	* Observer is a class that is meant to behave much like the name implies.
	* Observers may not be created manually. The constructor is private and only MultiCurl is allowed to create Observers.
	* This class observes the state of the CURL call. This class is built to be used in a multi threaded enviorment.
	*/
	class Observer final
	{
	private:
		friend class MultiCurl;

		/**
		* Constructor. Initally set as idle and not connected to CURLM.
		* \param template_handle which is duplicated
		* \throws std::runtime_error if duplicated curl is nullptr
		*/
		explicit Observer(CURL* template_handle);

	public:

		/// <returns> If the state is observed_state::finalized returns a ptr to handle, otherwise nullptr </returns>
		const HttpResponse* const try_get_data()const noexcept;

		/// <summary>
		/// If the state is observed_state::idle then it sets state as observed_state::finalized 
		/// signaling to multicurl that it may not be performed on. Does nothing if observed_state::bound or observed_state::finalized.
		/// </summary>
		void abort();

		// does nothing if !is_complete() and only finalizes data once ever
		// sets the httpcode, http_ok and content type info for the handle
		// if this function is not called the body if the handle is still retrievable
		void handle_finish_info();

		/// <returns> Current state of the observer </returns>
		observed_state get_state() const noexcept;

		/// <returns> true if observed_state::idle </returns>
		bool is_idle() const noexcept;

		/// <returns> true if observed_state::bound </returns>
		bool is_bound() const noexcept;

		/// <returns> true if observed_state::finalized </returns>
		bool is_finalized() const noexcept;

		/// <summary> If CURL is connected to CURLM, the destructor will disconnect them. </summary>
		~Observer()noexcept;

		Observer(const Observer&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(Observer&&) = delete;

	private:

		UCURL curl;                              /// <summary> Internal CURL which is duplicated from the constructor. </summary>
		HttpResponse data;                       /// <summary> Cached handle for response assignment. </summary>
		std::atomic<observed_state> state;		 /// <summary> Atomic state handle for multithreaded support. Used for behavior guarantees. </summary>
		std::atomic<CURLM*> mCurlm;              /// <summary> Atomic ptr to the CURLM handle once the observer gets bound and is performing. Used for resource management guarantees. </summary>
		std::atomic_bool handle_info_set = false;/// <summary> Atomic bool for only setting the data once. A bit optional as reading info from the same valid CURL is fine. </summary>
	};

	/// <summary> Class for staging bulk requests. </summary>
	class MultiCurl final
	{
	public:
		/**
		* Initializes CURLM.
		* \throws std::runtime_error if it failed to initialize CURLM
		*/
		explicit MultiCurl();

		/// <summary> All handles that are NOT bound (including handles with ref count as 1) </summary>
		void clear_handles()noexcept;

		/// <summary>
		/// Checks if there are any handles stored.
		/// </summary>
		/// <returns>True if empty</returns>
		inline bool is_empty()const noexcept { return mHandles.empty(); }

		//attempts to duplicate the provided config. unlike a single curl_perform_get MultiCurl does no option setting except CURLOPT_WRITEDATA
		//everything else from cert, url and other options are user resp.

		/**
		* Attempts to create a handle with the given template_handle duplicating it.
		* \param template_handle gets duplicated and creates an observer
		* \throws std::runtime_error if given template is nullptr
		* \throws if shared_ptr or vector pushback fails...
		* \returns a shared pointer for the observer so both the user can observe the handle and act on it and MultiCurl can manage state.
		*/
		std::shared_ptr<Observer> add_handle(CURL* template_handle);

		//count is how many handles to perform, first in first out
		//optional condition variable for listening from another thread

		/**
		* Binds observers to CURLM in first in first out order.
		*
		* Skips any observes with ref count 1 or state that is not observer_state::idle.
		*
		* Once an observer is set as bound, stoping performing on it is not possible nor allowed thanks to shared_ptr semantics and atomics.
		*
		* Unbinds after finishing performing setting the state as observer_state::finalized but does not set HttpResponse info.
		*
		* Cleans up handles after finishing giving up.
		*
		* \param count count how many handles to perform (doesn't change default CURLM settings meaning large values may not fire all at once)
		*
		* \param optional cv condition variable. Intended to wake another thread signaling some observer is finished.
		* \throws perform does not throw itself but anything internally may.
		*/
		void perform(std::size_t count, std::condition_variable* cv = nullptr);

		/// <summary> Clears all handles making sure everything indeed gets disconnected. A failsafe </summary>
		~MultiCurl() noexcept;
		MultiCurl(const MultiCurl&) = delete;
		MultiCurl& operator=(const MultiCurl&) = delete;
		MultiCurl(MultiCurl&&) noexcept = delete;
		MultiCurl& operator=(MultiCurl&&) noexcept = delete;

	private:
		/// <summary> CURLM for multiperforming </summary>
		UCURLM mUcurlm;

		/// <summary> List of observers. Doesn't neccessarily act as the final owner. It will let go of handles once done or if requested by observers abort() method </summary>
		std::vector<std::shared_ptr<Observer>> mHandles;
	};
}