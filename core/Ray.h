#pragma once

#include "Float2.h"
#include "Rect.h"

namespace badCore
{
	//##############################################################################################################
	// under construction
	//##############################################################################################################
	struct SweepInfo
	{
		class float2 contact_point;
		float time;
		bool is_hit;
	};
	
	struct IntersectionInfo
	{
		class float2 normal;
		float penetration;
		bool is_hit;
	};
	//##############################################################################################################
	//##############################################################################################################

	/**
	* Ray represents a finite 2D ray (line segment) defined by:
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
	* The constructor computes both the length and normalized direction
	* using a single square root operation for efficiency.
	*/
	class Ray final
	{
	public:
		
		/**
		* Constructs a ray from an origin and a vector.
		*
		* The vector defines both the direction and the length of the ray.
		*
		* Internally:
		*
		*     length = |vector|
		*     dir    = normalized(vector)
		*
		* Only one square root operation is performed during construction.
		*
		* \param origin starting point of the ray
		* \param vector vector defining ray direction and length
		*/
		Ray(const float2& origin, const float2& vector) noexcept;
		
		//NOTE: ORDER MATTERS FOR CONSTRUCTOR!

		/// <summary> Lenght of the ray. </summary>
		const float length;

		/// <summary> Origin point of the ray. </summary>
		const class float2 origin;

		/// <summary> Normalized direction vector of the ray. </summary>
		const class float2 dir;

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
		* Convenience function to validate a sweep time value.
		*
		* A valid hit must satisfy:
		*
		*     0 <= time < length
		*
		* \param time distance along the ray
		* \returns true if the time represents a valid hit
		*/
		constexpr bool is_hit(float time)const noexcept
		{
			return time >= 0.0f && time < length;
		}

		/**
		* Computes the closest point on the ray to a given point.
		*
		* The result is clamped to the ray segment.
		*
		* \param point target point
		*
		* \returns closest point on the ray segment
		*/
		constexpr auto closest_point_on_ray(const float2& point)const noexcept
		{
			float2 vector_between_objects = point - origin;
			float t = dot_product(vector_between_objects, dir);
			// handle cases where point would be on the same infinite line, but not the line segment. clamp it to the line segment
			t = core_clamp(t, 0.0f, length);
			return origin + dir * t;
		}

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
		IntersectionInfo intersection_test(const float2& point, float radius)const noexcept;

		/**
		* Converts the ray direction and length back into a vector.
		*
		* \returns vector representing the ray
		*/
		constexpr float2 get_vectorized()const noexcept
		{
			return {
				dir.x * length,
				dir.y * length 
			};
		}
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
	void reflection_routine_resolved(const Ray& target_surface, float2& point, float2& velocity, float radius) noexcept;
}