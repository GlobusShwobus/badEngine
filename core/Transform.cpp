#include "pch.h"
#include "Transform.h"

namespace badCore
{
	Mat3 Transform::transform()const noexcept
	{
		//ORDER MATTERS
		return badCore::Mat3::translation(mPos) * badCore::Mat3::rotation(mAngle) * badCore::Mat3::scale(mScale, mScale);
	}

	Mat3 Transform::transform_inverse()const noexcept
	{
		const float invScale = 1.0f / mScale;
		//camera transform are inverse of object transforms (perspective)
		return Mat3::scale(invScale, invScale) * Mat3::rotation(-mAngle) * Mat3::translation(-mPos);
	}
}