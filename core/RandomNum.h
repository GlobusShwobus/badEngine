#pragma once

#include <random>
#include <assert.h>

namespace bad
{
	/// <summary> Wrapper around std::mt19937 </summary>
	class RandomNum final
	{
	public:
		/// <summary> This tells that I produce the same type as mt19937 does internally. Making it legal to work with distributions. </summary>
		using result_type = std::mt19937::result_type;

		/// <summary> Overloads for min/max that just call std::mt19937 min/max using the local type def for result_type which is the same result type. </summary>
		static constexpr result_type min() { return std::mt19937::min(); }
		static constexpr result_type max() { return std::mt19937::max(); }

		/// <summary> Because of local typedef of result_type and overloading min/max, distributions are able to accept RandomNum class itself instead of requiring to call randomnum.engine </summary>
		result_type operator()()
		{
			return engine();
		}

		/// <summary> Initializes std::mt19937 with random device </summary>
		RandomNum()
			:engine(std::random_device{}())
		{
		}

		/// <summary>
		/// Create a random number using uniform_int_distribution.
		/// </summary>
		/// <param name="min"> min minimum output value </param>
		/// <param name="max"> max maximum output value</param>
		/// <returns> random int between min and max </returns>
		int get(int min, int max)
		{
			assert(min <= max);
			return std::uniform_int_distribution<int>(min, max)(engine);
		}

		/// <summary>
		/// Create a random number using uniform_real_distribution.
		/// </summary>
		/// <param name="min"> min minimum output value </param>
		/// <param name="max"> max maximum output value</param>
		/// <returns> random float between min and max </returns>
		float get(float min, float max)
		{
			assert(min <= max);
			return std::uniform_real_distribution<float>(min, max)(engine);
		}

		/// <summary>
		/// Creates a uniform_int_distribution.
		/// </summary>
		/// <param name="min"> min minimum output value </param>
		/// <param name="max"> max maximum output value</param>
		/// <returns> std::uniform_int_distribution </returns>
		auto get_int_distribution(int min, int max)
		{
			assert(min <= max);
			return std::uniform_int_distribution<int>(min, max);
		}

		/// <summary>
		/// Creates a uniform_real_distribution.
		/// </summary>
		/// <param name="min"> min minimum output value </param>
		/// <param name="max"> max maximum output value</param>
		/// <returns> std::uniform_real_distribution </returns>
		auto get_real_distribution(float min, float max)
		{
			assert(min <= max);
			return std::uniform_real_distribution<float>(min, max);
		}

		/// <summary>
		/// Creates a normal_distribution.
		/// </summary>
		/// <param name="mean"> mean the mean value of output </param>
		/// <param name="deviation"> standard_deviation as the difference from the mean </param>
		/// <returns> std::normal_distribution </returns>
		auto get_normal_distribution(float mean, float standard_deviation)
		{
			return std::normal_distribution<float>(mean, standard_deviation);
		}

	private:

		std::mt19937 engine;
	};
}