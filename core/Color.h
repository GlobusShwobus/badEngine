#pragma once

namespace bad
{
	/**
	* \brief Lightweight 32-bit RGBA color container.
	*
	* Color stores a single pixel using a packed 32-bit integer. Each color
	* component occupies one byte of the internal dword in the following layout:
	*
	*      31 .............. 24 | 23 .............. 16 | 15 .............. 8 | 7 .............. 0
	*           Alpha (A)          Blue (B)                Green (G)            Red (R)
	*
	* Bit packing format:
	*
	*      dword = (A << 24) | (B << 16) | (G << 8) | R
	*
	* This layout corresponds to an **RGBA byte ordering in little-endian
	* memory**, meaning the bytes in memory appear as:
	*
	*      [R][G][B][A]
	*
	* Each component ranges from 0–255 and can be accessed or modified
	* individually through the provided getters and setters.
	*
	* \note The alpha channel defaults to 255 (fully opaque) when not specified.
	*/
	class Color final
	{
	public:
		unsigned int dword;

		constexpr Color()                 noexcept :dword(0) {}
		constexpr Color(unsigned int dw)  noexcept :dword(dw) {}

		constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)noexcept
			: dword((a << 24u) | (b << 16u) | (g << 8u) | r)
		{
		}

		constexpr Color(unsigned char r, unsigned char g, unsigned char b)noexcept
			: dword((255u << 24u) | (b << 16u) | (g << 8u) | r) 
		{
		}

		constexpr Color& operator=(unsigned int color) noexcept
		{
			dword = color;
			return *this;
		}

		constexpr bool operator==(const Color& color)const noexcept
		{
			return dword == color.dword;
		}

		constexpr bool operator!=(const Color& color)const noexcept 
		{
			return !(*this == color);
		}

		constexpr void set_a(unsigned char alpha)noexcept 
		{
			dword = (dword & 0xFFFFFFu) | (alpha << 24u);
		}

		constexpr void set_b(unsigned char blue)noexcept
		{
			dword = (dword & 0xFF00FFFFu) | (blue << 16u);
		}

		constexpr void set_g(unsigned char green)noexcept
		{
			dword = (dword & 0xFFFF00FFu) | (green << 8u);
		}

		constexpr void set_r(unsigned char red)noexcept 
		{
			dword = (dword & 0xFFFFFF00u) | red;
		}

		constexpr unsigned char get_a()const noexcept
		{
			return (dword >> 24u) & 0xFFu;
		}

		constexpr unsigned char get_b()const noexcept
		{
			return (dword >> 16u) & 0xFFu;
		}

		constexpr unsigned char get_g()const noexcept
		{
			return (dword >> 8u) & 0xFFu;
		}

		constexpr unsigned char get_r()const noexcept
		{
			return dword & 0xFFu;
		}
	};

	namespace Colors 
	{
		static constexpr Color makeRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)noexcept
		{
			return (a << 24u) | (b << 16u) | (g << 8u) | r;
		}

		static constexpr Color White   = makeRGBA(255u, 255u, 255u, 255u);
		static constexpr Color Black   = makeRGBA(0u, 0u, 0u, 255u);
		static constexpr Color Gray    = makeRGBA(0x80u, 0x80u, 0x80u, 255u);
		static constexpr Color Red     = makeRGBA(255u, 0u, 0u, 255u);
		static constexpr Color Green   = makeRGBA(0u, 255u, 0u, 255u);
		static constexpr Color Blue    = makeRGBA(0u, 0u, 255u, 255u);
		static constexpr Color Yellow  = makeRGBA(255u, 255u, 0u, 255u);
		static constexpr Color Cyan    = makeRGBA(0u, 255u, 255u, 255u);
		static constexpr Color Magenta = makeRGBA(255u, 0u, 255u, 255u);
	}
}