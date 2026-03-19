#pragma once

#include <string>
#include <ostream>

namespace bad
{
	/**
	* bString is a combination of a std::string and a bool.
	* 
	* bString is meant to be used when an operation may or may not fail but on failure both a bool and a reason are desired.
	* 
	* To create a bString static methods ::success() and ::failure() are provided.
	* 
	* Success should be used if there was no error. 
	*
	* Failure should be used if an operation failed and a reason why it failed needs communicating.
	* 
	* bString can also be used with an output stream using operator <<.
	*/
	class bString
	{
		bString() = default;

		/**
		* Creates a bString taking std::string by value and moving it
		* \param str as message to be stored
		*/
		bString(std::string message) :message(std::move(message)) {}

	public:
		/// <summary> message stored. it's not mean to be const or private or antyhing. </summary>
		std::string message;

		/// <summary>
		/// Creates an empty bString.
		/// </summary>
		/// <returns> empty bString</returns>
		static bString success()noexcept
		{
			return bString{};
		}

		/// <summary>
		/// Creates a bString with a message describing failure.
		/// </summary>
		/// <param name="msg"> by value msg string description of the failure</param>
		/// <returns> bString with a specified message or "undocumented error" if empty msg passed</returns>
		static inline bString failure(std::string msg)noexcept
		{
			if (msg.empty())
				msg = "undocumented error";

			return { std::move(msg) };
		}

		/// <summary>
		/// Checks if the message is empty signaling success.
		/// </summary>
		/// <returns> true if empty, false if not</returns>
		bool succeeded()const noexcept
		{
			return message.empty();
		}

		/// <summary>
		/// Checks if the message contains anything signaling failure.
		/// </summary>
		/// <returns> true if not empty, false if empty </returns>
		bool failed()const noexcept
		{
			return !message.empty();
		}

		/// <summary> Convenience operator overload for simply checking if(bString) </summary>
		explicit operator bool() const noexcept
		{
			return succeeded();
		}

		/**
		* Overloaded std::ostream operator << for convenient streaming of the internal message.
		* 
		* Alternatively direct access to the internal message is also provided.
		* 
		* \param os std::ostream reference
		* \param s as bString
		* 
		* \returns std::ostream& with additional info "success" or "failure" attached
		*/
		friend std::ostream& operator<<(std::ostream& os, const bString& s) {
			if (s.succeeded())
				return os << "success";
			else
				return os << "failure: " << s.message;
		}
	};
}