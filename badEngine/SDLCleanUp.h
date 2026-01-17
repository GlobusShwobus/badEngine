#pragma once

namespace badEngine {

	template <typename T, void(*func)(T*)>
	struct SDLDeleter {
		void operator()(T* ptr)const noexcept {
			if (ptr)
				func(ptr);
		}
	};
}