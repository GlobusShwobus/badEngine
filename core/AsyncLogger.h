#pragma once

#include <iostream>
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

		AsyncLogger() {
			mWorker = std::thread([this]() { this->process(); });
		}

		~AsyncLogger() {
			mRunning = false;
			mCondition.notify_all();
			mWorker.join();
		}

		void log(std::string msg) {
			{
				std::lock_guard<std::mutex> lock(mSync);
				mQueue.push(std::move(msg));
			}
			mCondition.notify_one();
		}

	private:

		void process() {
			while (mRunning || !mQueue.empty()) {
				std::unique_lock<std::mutex> lock(mSync);

				mCondition.wait(lock, [this]() {
					return !mQueue.empty() || !mRunning;
					});

				while (!mQueue.empty()) {
					std::string msg = std::move(mQueue.front());
					mQueue.pop();

					lock.unlock();
					std::cout << msg << '\n';
					lock.lock();
				}
			}
		}
	};
}