#pragma once

#include "Float2.h"

namespace bad
{
	/**
	* \brief
	* not much to say about the circle currently.
	*/
	struct Circle
	{
		const bad::Point& get_center()const noexcept { return mCenter; }
		float get_radius()const noexcept { return mRadius; }

		bad::Point mCenter;
		float mRadius = 0;
	};
}