#pragma once

namespace badEngine {
	class Color final
	{
	public:
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

		unsigned int dword;
	};

	namespace Colors 
	{
		static constexpr Color makeRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)noexcept
		{
			return (a << 24u) | (b << 16u) | (g << 8u) | r;
		}

		static constexpr Color White = makeRGBA(255u, 255u, 255u, 255u);
		static constexpr Color Black = makeRGBA(0u, 0u, 0u, 255u);
		static constexpr Color Gray = makeRGBA(0x80u, 0x80u, 0x80u, 255u);
		static constexpr Color Red = makeRGBA(255u, 0u, 0u, 255u);
		static constexpr Color Green = makeRGBA(0u, 255u, 0u, 255u);
		static constexpr Color Blue = makeRGBA(0u, 0u, 255u, 255u);
		static constexpr Color Yellow = makeRGBA(255u, 255u, 0u, 255u);
		static constexpr Color Cyan = makeRGBA(0u, 255u, 255u, 255u);
		static constexpr Color Magenta = makeRGBA(255u, 0u, 255u, 255u);
	}
}