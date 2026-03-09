#include "pch.h"
#include "Transform.h"

namespace badCore
{
	Mat3 Transform::transform()const noexcept
	{
		return
			badCore::Mat3::translation(mPos) *
			badCore::Mat3::rotation(mRadians) *
			badCore::Mat3::scale(mScale, mScale);
	}

	Mat3 Transform::transform_inverse()const noexcept
	{
		const float invScale = 1.0f / mScale;
		return
			Mat3::scale(invScale, invScale) *
			Mat3::rotation(-mRadians) *
			Mat3::translation(-mPos);
	}
}