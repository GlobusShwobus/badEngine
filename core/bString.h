#pragma once

#include <string>
#include <ostream>

namespace badCore
{
	class bString
	{
		bString() = default;
		bString(std::string str) :message(std::move(str)) {}

	public:
		std::string message;

		constexpr static bString success()noexcept
		{
			return {};
		}

		static inline bString failure(std::string msg)noexcept
		{
			if (msg.empty())
				msg = "undocumented error";

			return { std::move(msg) };
		}

		constexpr bool succeeded()const noexcept
		{
			return message.empty();
		}

		constexpr bool failed()const noexcept
		{
			return !message.empty();
		}

		explicit constexpr operator bool() const noexcept//lol
		{
			return succeeded();
		}

		friend std::ostream& operator<<(std::ostream& os, const bString& s) {
			if (s.succeeded())
				return os << "success";
			else
				return os << "failure: " << s.message;
		}
	};
}