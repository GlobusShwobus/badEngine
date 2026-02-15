#pragma once

#include <string>

namespace badEngine
{
	struct NoThrowException
	{
		bool isGood;
		std::string message;

		explicit operator bool() const noexcept
		{
			return isGood;
		}
	};
}