#pragma once

#include "Float2.h"
#include "Rect.h"

namespace bad
{
	//##############################################################################################################
	// under construction
	//##############################################################################################################
	struct SweepInfo
	{
		Point contact_point;
		float time;
		bool is_hit;
	};

	struct IntersectionInfo
	{
		Vector normal;
		float penetration;
		bool is_hit;
	};
	//##############################################################################################################
	//##############################################################################################################

	/**
	* LineSegment represents a finite 2D line segment defined by:
	*
	*     origin + dir * t
	*
	* where:
	*
	*     origin  -> starting point
	*     dir     -> normalized direction vector
	*     t       -> distance along the ray
	*
	* The ray has a fixed length and therefore represents a line segment
	* rather than an infinite ray.
	*
	* Internally the class stores:
	*
	*     origin  -> starting point
	*     dir     -> normalized direction
	*     length  -> length of the ray
	*
	* The constructor computes both the length and normalized direction.
	* 
	*/
	class LineSegment final
	{
	public:

		/**
		* Constructs a LineSegment from an begin and end points.
		*
		* Internally finds the vecor of the line segment doing end - begin;
		* derives the lenght and normalized from the result
		*
		* \param origin starting point of the ray
		* \param vector vector defining ray direction and length
		* 
		*/
		LineSegment(const Point& begin, const Point& end) noexcept;

		const bad::Point& get_origin()const noexcept { return mOrigin; }
		const bad::Point& get_dir()const noexcept { return mDir; }
		float get_length()const noexcept { return mLength; }

		/**
		* Computes the closest point on the ray to a given point.
		*
		* The result is clamped to the ray segment.
		*
		* \param point target point
		*
		* \returns closest point on the ray segment
		*/
		bad::Point closest_point(const Point& point)const noexcept;

		/**
		* Performs a sweep test against an axis-aligned rectangle.
		*
		* The ray is treated as a moving point traveling along the ray
		* direction until its maximum length.
		*
		* \param target rectangle to test against
		*
		* \returns SweepInfo containing:
		*     - contact_point : location of the hit
		*     - time          : distance along the ray
		*     - is_hit        : true if intersection occurred
		*
		* NOTE: If is_hit is false then contact_point default constructed and time is INFINITY.
		*/
		SweepInfo sweep_test(const Rect& target)const noexcept;

		/**
		* Performs an intersection test between the ray and a circle.
		*
		* The circle is defined by a center point and a radius.
		*
		* \param point circle center
		* \param radius circle radius
		*
		* \returns IntersectionInfo containing:
		*     - normal       : collision normal
		*     - penetration  : penetration depth
		*     - is_hit       : true if intersection occurred
		* NOTE: if is_hit is false then normal is default constructed and penetration is 0.
		*/
		IntersectionInfo intersection_test(const Point& point, float radius)const noexcept;

		/**
		* Converts the ray direction and length back into a vector.
		*
		* \returns vector representing the ray
		*/
		constexpr Vector get_vectorized()const noexcept
		{
			return {
				mDir.x * mLength,
				mDir.y * mLength
			};
		}

	private:

		Point mOrigin; 		/// <summary> Origin point of the ray. </summary>
		Vector mDir; 		/// <summary> Normalized direction vector of the ray. </summary>
		float mLength; 		/// <summary> Lenght of the ray. </summary>
	};

	/**
	 * Performs a collision test against a surface and resolves the collision
	 * by correcting the position and reflecting the velocity.
	 *
	 * If the object intersects the target surface:
	 * - The velocity is reflected using the collision normal.
	 * - The position is moved out of the surface along the normal by the
	 *   reported penetration depth.
	 *
	 * The function also ensures the normal is oriented against the incoming
	 * velocity before applying the reflection.
	 *
	 * \param target_surface Surface (ray/segment) used for the collision test.
	 * \param point Position of the object. Modified if penetration occurs.
	 * \param velocity Velocity of the object. Reflected if a collision occurs.
	 * \param radius Radius of the object used for the intersection test.
	 */
	void reflection_routine_resolved(const LineSegment& target_surface, Point& point, Vector& velocity, float radius) noexcept;
}


/*
		
		Convenience function to validate a sweep time value.
		
		A valid hit must satisfy:
		
		    0 <= time < length
		
		\param time distance along the ray
		\returns true if the time represents a valid hit
		
constexpr bool is_hit(float time)const noexcept
{
	return time >= 0.0f && time < mLength;
}
*/