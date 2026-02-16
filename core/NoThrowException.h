#pragma once

#include <string>

namespace badEngine
{
	struct NoThrowException
	{
		bool isGood;
		std::string message;

		NoThrowException(bool is_good, const std::string& msg)
			:isGood(is_good), message(msg)
		{
		}


		static NoThrowException success(std::string msg = {})noexcept
		{
			return { true, std::move(msg) };
		}

		static NoThrowException failure(std::string msg)noexcept
		{
			return { false, std::move(msg) };
		}

		explicit operator bool() const noexcept
		{
			return isGood;
		}
	};
}