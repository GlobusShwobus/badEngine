#pragma once
/*
* For not store it here, this has to be handeled somewhere here
	void Font::update()noexcept
	{
		if (mLetterPos.isEmpty()) {
			return;
		}
		// store original base
		const AABB originalBase = mLetterPos.front().second;

		// calculate scale factors RELATIVE to original base
		const float sx = mDest.w / originalBase.w;
		const float sy = mDest.h / originalBase.h;

		// calculate translation based on original base
		const float dx = mDest.x - originalBase.x;
		const float dy = mDest.y - originalBase.y;

		for (auto& pair : mLetterPos) {
			auto& d = pair.second;

			// Apply transformation relative to original base
			d.x = mDest.x + (d.x - originalBase.x) * sx;
			d.y = mDest.y + (d.y - originalBase.y) * sy;

			// Scale width and height
			d.w = originalBase.w * sx;
			d.h = originalBase.h * sy;
		}
	}
*/