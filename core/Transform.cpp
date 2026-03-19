#include "pch.h"
#include "Transform.h"
#include <cmath>
namespace badCore
{
	Transform::Transform(const Point& pos, float scale, float radians)
		:mPos(pos), mScale(scale), mRadians(radians), mSin(std::sin(radians)), mCos(std::cos(radians))
	{
	}

	void Transform::set_rotation_and_update(float radians) noexcept
	{
		mRadians = radians;
		update_sincos();
	}

	void Transform::update_sincos()noexcept
	{
		mSin = std::sin(mRadians);
		mCos = std::cos(mRadians);
	}
}