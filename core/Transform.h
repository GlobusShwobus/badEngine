#pragma once

#include "Float2.h"
#include "Matrix3.h"

namespace badCore
{
	struct Transform 
	{
		Transform(const float2& pos, float scale, float radians)
			:pos(pos), scale(scale), radians(radians)
		{
		}

		Mat3 transform()const noexcept;

		Mat3 transform_inverse()const noexcept;

		class float2 pos;
		float scale;
		float radians;;
	};
}