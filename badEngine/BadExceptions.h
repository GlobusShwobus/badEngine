#pragma once
#include <string>
#include <string_view>
#include <stdexcept>
#include <cassert>

namespace badEngine {

	class BasicException : public std::exception
	{

	public:

		BasicException(std::string_view my_message, std::string_view external_message = {})
			:mDescription(my_message)
		{
			if (!external_message.empty()) {
				mDescription += " [EXTERNAL MESSAGE: ";
				mDescription += external_message;
				mDescription += "]";
			}
		}

		const char* what() const noexcept override {
			return mDescription.c_str();
		}
		const std::string& message()const noexcept {
			return mDescription;
		}
		std::string_view message_view()const noexcept {
			return mDescription;
		}

	private:
		std::string mDescription;
	};
}