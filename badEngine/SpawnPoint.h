#pragma once

#include "Circle.h"
#include "vector.h"
#include "RandomNum.h"
#include "Sequence.h"

namespace badEngine
{

	class SpawnPoint
	{
	public:

		SpawnPoint(badCore::Sequence<Circle>& container, float r,  float period)
			: period(period), container(container), radius(r) {
		}

		void update(float dt)
		{
			time += dt;
			if (time >= period) {
				time -= period;

				badCore::float2 pos = { rng.rFloat(100,600), 600 };
				badCore::float2 vel = { rng.rFloat(-50,50), -150 };

				container.emplace_back(
				pos, radius, vel
				);
			}
		}


	private:

		badCore::RandomNum rng;
		float period;
		float radius;
		float time = 0;
		badCore::Sequence<Circle>& container;
	};

}