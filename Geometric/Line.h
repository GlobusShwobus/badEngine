#pragma once

#include "Float2.h"
#include "Rect.h"
#include "Circle.h"

namespace bad
{
	struct LineSegment
	{
		LineSegment() :p0{}, p1{} {}
		explicit LineSegment(const bad::Point& from, const bad::Point& to) :p0(from), p1(to) {}

		bad::Point p0;
		bad::Point p1;

		bad::Point closest_point(const bad::Point& target)const noexcept;
	};

	class Ray
	{
	public:
		Ray() = delete;

		explicit Ray(const bad::Point& from, const bad::Point& to) noexcept;
		explicit Ray(const bad::LineSegment& line);

		void update_ray(const bad::Point& from, const bad::Point& to)noexcept;
		void update_ray(const bad::LineSegment& line)noexcept;

		constexpr bad::Point closest_point(const bad::Point& target)const noexcept
		{
			return bad::closest_point(mOrigin, mDir, mLength, target);
		}

		const bad::Point& get_origin()const noexcept { return mOrigin; }
		const bad::Point& get_dir()const noexcept { return mDir; }
		float get_length()const noexcept { return mLength; }

	private:

		bad::Point mOrigin;
		bad::Point mDir;
		float mLength;
	};

	namespace collision
	{
		struct SweepInfo
		{
			Point contact_point;
			float time;
			bool is_hit;
		};

		struct HitInfo
		{
			Vector normal;
			float penetration;
			bool is_hit;
		};

		SweepInfo sweep(const bad::Ray& ray, const bad::Rect& target) noexcept;

		HitInfo intersects(const bad::Ray& ray, const bad::Circle& circle) noexcept;

		HitInfo intersects(const bad::LineSegment& line, const bad::Circle& circle) noexcept;
	}
}