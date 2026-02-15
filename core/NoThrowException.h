#pragma once

#include <string>

namespace badEngine
{
	struct NoThrowException
	{
		NoThrowException(bool is_good, const std::string& msg)
			:isGood(is_good), message(msg)
		{
		}


		static NoThrowException success()noexcept
		{
			return { true, "" };
		}

		static NoThrowException failure(std::string msg)noexcept
		{
			return { false, std::move(msg) };
		}

		bool isGood;
		std::string message;

		explicit operator bool() const noexcept
		{
			return isGood;
		}
	};
}