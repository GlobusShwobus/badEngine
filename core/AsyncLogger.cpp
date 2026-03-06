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
}