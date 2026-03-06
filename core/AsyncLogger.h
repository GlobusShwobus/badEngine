#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <string>
#include <atomic>

namespace badCore
{
	class AsyncLogger {
	private:
		std::queue<std::string> mQueue;			// collect logs
		std::mutex mSync;						// for locking pushes into queue and process
		std::condition_variable mCondition;		// for communication between log and process
		std::atomic<bool> mRunning{ true };		// for process loop (currently main constructor is hidden though)
		std::thread mWorker;					// worker thread where to do the outputs

	public:

		static AsyncLogger& Global() {
			static AsyncLogger logger;
			return logger;
		}

		AsyncLogger();

		~AsyncLogger();

		void log(std::string msg);

	private:
		void process();
	};
}