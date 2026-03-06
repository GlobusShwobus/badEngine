#pragma once

#include <random>
#include <memory>

namespace badCore
{
	class RandomNum final
	{
	public:

		RandomNum();

		int get(int min, int max)const noexcept;

		float get(float min, float max)const noexcept;

	private:
		std::unique_ptr<std::mt19937> rng;
	};
}