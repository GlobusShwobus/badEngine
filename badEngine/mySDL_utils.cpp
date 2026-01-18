#include "mySDL_utils.h"

namespace badEngine {
	bool MapSDL_Flags_to_size_t_bitwise(const std::string& key, std::size_t& flags)noexcept
	{
		bool good_flag = false;

		if (key == "SDL_WINDOW_OPENGL") {
			flags |= SDL_WINDOW_OPENGL;
			good_flag = true;
		}
		if (key == "SDL_WINDOW_RESIZABLE") {
			flags |= SDL_WINDOW_RESIZABLE;
			good_flag = true;
		}
		//other flags here

		return good_flag;
	}
}