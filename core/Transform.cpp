#include "pch.h"
#include "Transform.h"

namespace badCore
{
	Mat3 Transform::transform()const noexcept
	{
		return
			badCore::Mat3::translation(pos) *
			badCore::Mat3::rotation(radians) *
			badCore::Mat3::scale(scale, scale);
	}

	Mat3 Transform::transform_inverse()const noexcept
	{
		const float invScale = 1.0f / scale;
		return
			Mat3::scale(invScale, invScale) *
			Mat3::rotation(-radians) *
			Mat3::translation(-pos);
	}
}