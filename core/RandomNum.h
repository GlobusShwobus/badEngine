#pragma once

#include <random>

namespace badCore
{
	/// <summary> Wrapper around std::mt19937 </summary>
	class RandomNum final
	{
	public:

		/**
		*Initializes std::mt19937 with random device
		*\throws if std::random_device somehow fails. 
		*/
		RandomNum();

		/**
		* Uses std::uniform_int_disribution<int> and generates a random number.
		* \param min minimum value
		* \param max maximum value
		* \returns a random int between min and max
		* \throws in DEBUG asserts min must be less or equal to max
		* \throws if for some reason uniform_int_distribution throws
		*/
		int get(int min, int max);


		/**
		* Uses std::uniform_real_distribution<float> and generates a random number.
		* \param min minimum value
		* \param max maximum value
		* \returns a random int between min and max
		* \throws in DEBUG asserts min must be less or equal to max
		* \throws if for some reason uniform_real_distribution throws
		*/
		float get(float min, float max);

	private:
		/// <summary> Heap allocated std::mt19937 </summary>
		std::mt19937 mt19937;
	};
}