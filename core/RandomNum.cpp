#include "pch.h"
#include "RandomNum.h"
#include <assert.h>

badCore::RandomNum::RandomNum()
	:mt19937(std::random_device{}())
{
}

int badCore::RandomNum::get(int min, int max)
{
	assert(min <= max && "Invalid logic: min is more than max");
	return std::uniform_int_distribution<int>(min, max)(mt19937);
}

float badCore::RandomNum::get(float min, float max)
{
	assert(min <= max && "Invalid logic: min is more than max");
	return std::uniform_real_distribution<float>(min, max)(mt19937);
}