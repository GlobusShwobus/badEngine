#pragma once

#include "Float2.h"
#include "Rect.h"
#include "Circle.h"

namespace bad
{
	/**
	* LineSegment represents a basic line segment with points p0 and p1.
	*/
	struct LineSegment
	{
		LineSegment() :p0{}, p1{} {}
		explicit LineSegment(const bad::Point& from, const bad::Point& to) :p0(from), p1(to) {}

		bad::Point p0;
		bad::Point p1;

		/// <returns> Returns the closest point on the line against the target </returns>
		bad::Point closest_point(const bad::Point& target)const noexcept;
	};

	/**
	* \brief
	* Ray is similar to a line segment except represented by the origin, direction vector, and length of the segment.
	* 
	* The ray may be used as an infinite ray but also as a fineite ray segment because the length of the original segment is cached.
	* 
	* Ray is not meant to be modifiable.
	*/
	class Ray
	{
	public:
		Ray() = delete;
		/// <summary> Constructs a ray from two given points. It computes the vector, takes its lenght and normalizes the vector. </summary>
		explicit Ray(const bad::Point& from, const bad::Point& to) noexcept;
		/// <summary> Constructs a ray from a given line segment. It computes the vector, takes its lenght and normalizes the vector. </summary>
		explicit Ray(const bad::LineSegment& line);

		/// <returns> Returns the closest point on the ray against the target </returns>
		constexpr bad::Point closest_point(const bad::Point& target)const noexcept
		{
			return bad::closest_point(mOrigin, mDir, mLength, target);
		}

		/// <returns> the origin point of the ray </returns>
		const bad::Point& get_origin()const noexcept { return mOrigin; }		

		/// <returns> normalized direction of the ray </returns>
		const bad::Point& get_dir()const noexcept { return mDir; }				

		/// <returns> the magnitude of the ray segment </returns>
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

		/**
		* Does a sweep test of an infinite ray against a target rectangle.
		* 
		* For a true collision decision an additional check is required externally:
		* 
		* SweepInfo.time < ray.get_length()
		* 
		* \param ray for the ray
		* \param target against which is sweeped
		* 
		* \returns
		* 
		* SweepInfo.is_hit - if there is a hit on an infinite plane
		* 
		* SweepInfo.time - 'when' the hit occurs in as a float
		* 
		* SweepInfo.contact_point - the exact point 
		*/
		SweepInfo sweep(const bad::Ray& ray, const bad::Rect& target) noexcept;

		/**
		* Does an intersection test of a ray against a circle.
		* 
		* \param ray for the ray
		* \param circle for the circle
		* 
		* \returns
		* HitInfo.is_hit if the ray and the circle overlap.
		*
		* HitInfo.penetration how much the circle and ray overlap;
		*
		* HitInfo.normal the direction from where the collision occured 
		*/
		HitInfo intersects(const bad::Ray& ray, const bad::Circle& circle) noexcept;

		/**
		* Does an intersection test of a ray against a circle. Internally uses the ray version.
		* 
		* \param line for the linesegment
		* \param circle for the circle
		* 
		* \returns
		* HitInfo.is_hit if the ray and the circle overlap.
		*
		* HitInfo.penetration how much the circle and ray overlap;
		*
		* HitInfo.normal the direction from where the collision occured 
		*/
		HitInfo intersects(const bad::LineSegment& line, const bad::Circle& circle) noexcept;
	}
}