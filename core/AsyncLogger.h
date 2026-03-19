#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <string>
#include <atomic>

namespace bad
{
	class AsyncLogger {
	private:

		/// <summary> Queue storing log messages waiting to be processed. </summary>
		std::queue<std::string> mQueue;			

		/// <summary> Mutex protecting access to the queue. </summary>
		std::mutex mSync;						

		/// <summary> Condition variable used to wake the worker thread when new logs arrive. </summary>
		std::condition_variable mCondition;		

		/// <summary> Controls the lifetime of the worker thread loop. </summary>
		std::atomic<bool> mRunning{ true };		

		/// <summary> Worker thread responsible for processing log messages. </summary>
		std::thread mWorker;					

	public:
		/**
		* Returns the global singleton logger instance.
		* 
		* The instance is lazily created on the first call and lives until program shutdown.
		* 
		* \returns reference to the global AsyncLogger instance
		*/
		static AsyncLogger& Global() {
			static AsyncLogger logger;
			return logger;
		}

		/// <summary> Constructs the AsyncLogger and starts the worker thread </summary>
		AsyncLogger();

		/// <summary> Stops the worker thread and flushes any remaining log messages. </summary>
		~AsyncLogger();

		AsyncLogger(const AsyncLogger&) = delete;
		AsyncLogger& operator=(const AsyncLogger&) = delete;

		/**
		* Queues a message for async logging.
		* 
		* The message is pushed into the internal queue and the worker thread is notified that work is available.
		* 
		* \param msg log messsage to queue (by value)
		*/
		void log(std::string msg);

	private:

		/**
		* Worker thread loop that processes queued log messages
		* 
		* The function waits for either:
		* - new messages to appear in the queue
		* - a shutdown signal
		* 
		* Messages are transferred to a temporary queue while holding the lock,
		* then processed outside the lock to minimize blocking of producer threads.
		*/
		void process();
	};
}