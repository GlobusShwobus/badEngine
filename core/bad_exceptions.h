#pragma once
#include <string>
#include <exception>
#include <assert.h>

namespace badCore
{
	class BadException final : public std::exception
	{
	public:

		explicit BadException(const char* my_message, const char* extra = nullptr);

		const char* what() const noexcept override;

		const std::string& message()const noexcept;

	private:
		std::string mDescription;
	};
}