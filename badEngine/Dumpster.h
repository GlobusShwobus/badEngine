#pragma once

//#################################################################################################
//  DESCRIPTION: 
//              more of an idea than code. the idea is to run bookkeeping code like inits and destructs only when
//              a frame didn't actually take more than a limit allows.
//              this might be a fauly idea if frames are handled fully with steps not counters        
//#################################################################################################
//
//auto dt = frameTimer.MarkMilliSec();
//auto limit = frameTimer.getLimitMilliSec();
//if (dt < limit) {
//
//    const auto spareTime = limit - dt;
//
//    Stopwatch timeIntermediary;
//    //DO SHIT HERE.. ALSO IF LOOPITY ACTION THEN THE TIMER SHOULD PROBABLY BE PART OF THE LOOP INSTEAD
//
//    //#################################################################################
//
//    auto idt = timeIntermediary.MarkMilliSec();
//
//    const auto remainingTime = spareTime - idt;
//
//    if (remainingTime.count() > 0) {
//        std::this_thread::sleep_for(remainingTime);
//    }
//}


//#################################################################################################
//  DESCRIPTION: 
//              update the destinations of a font. currently font only provides source boxes... a TODO...
//#################################################################################################
// 
//	void Font::update()noexcept
//	{
//		if (mLetterPos.isEmpty()) {
//			return;
//		}
//		// store original base
//		const AABB originalBase = mLetterPos.front().second;
//
//		// calculate scale factors RELATIVE to original base
//		const float sx = mDest.w / originalBase.w;
//		const float sy = mDest.h / originalBase.h;
//
//		// calculate translation based on original base
//		const float dx = mDest.x - originalBase.x;
//		const float dy = mDest.y - originalBase.y;
//
//		for (auto& pair : mLetterPos) {
//			auto& d = pair.second;
//
//			// Apply transformation relative to original base
//			d.x = mDest.x + (d.x - originalBase.x) * sx;
//			d.y = mDest.y + (d.y - originalBase.y) * sy;
//
//			// Scale width and height
//			d.w = originalBase.w * sx;
//			d.h = originalBase.h * sy;
//		}
//	}
