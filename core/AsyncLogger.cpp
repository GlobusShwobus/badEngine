#include "pch.h"
#include "AsyncLogger.h"
#include <iostream>

namespace badCore
{
	AsyncLogger::AsyncLogger() {
		mWorker = std::thread([this]() { this->process(); });
	}

	AsyncLogger::~AsyncLogger() {
		mRunning = false;
		mCondition.notify_all();
		mWorker.join();
	}

	void AsyncLogger::log(std::string msg) {
		{
			std::lock_guard<std::mutex> lock(mSync);
			mQueue.push(std::move(msg));
		}
		mCondition.notify_one();
	}

	void AsyncLogger::process() {
		while (true) {
			std::queue<std::string> localQueue;
			{
				std::unique_lock<std::mutex> lock(mSync);
				mCondition.wait(lock, [this]() {
					return !mQueue.empty() || !mRunning;
					});

				if (!mRunning && mQueue.empty()) break;

				//swap to not fiddle around with locks
				std::swap(mQueue, localQueue);
			}

			// Process logs outside the lock!
			while (!localQueue.empty()) {
				std::cout << localQueue.front() << std::endl;
				localQueue.pop();
			}
		}
	}
}