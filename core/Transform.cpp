#include "pch.h"
#include "Transform.h"
#include <cmath>
namespace badCore
{
	Transform::Transform(const float2& pos, float scale, float radians)
		:mPos(pos), mScale(scale), mRadians(radians)
	{
	}

	Mat3 Transform::transform()const noexcept
	{
		return
			badCore::Mat3::translation(mPos) *
			badCore::Mat3::rotation(mSin, mCos) *
			badCore::Mat3::scale(mScale, mScale);
	}

	Mat3 Transform::transform_inverse()const noexcept
	{
		const float invScale = 1.0f / mScale;
		return
			Mat3::scale(invScale, invScale) *
			Mat3::rotation(-mSin, mCos) *
			Mat3::translation(-mPos);
	}

	void Transform::set_rotation(float radians)noexcept
	{
		mRadians = radians;
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