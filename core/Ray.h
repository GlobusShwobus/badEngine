#pragma once

#include "vector.h"

namespace badCore
{
	//NOTE:: ray should always store normalized dir
	struct Ray
	{
		float2 origin;
		float2 dir;
		float magnitude;
	};
}