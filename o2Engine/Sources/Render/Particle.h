#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/Math/Color.h"

namespace o2
{
	class Particle
	{
	public:
		Vec2F  position;   // Position of particle center
		Vec2F  velocity;   // Particle velocity
		float  angle;      // Particle angle in radians
		float  angleSpeed; // Angle speed in radians/sec
		Vec2F  size;       // Size of particle
		Color4 color;      // Particle's color
		float  time;       // Estimate life time
		bool   alive;      // Is particle alive

		bool operator==(const Particle& other) const
		{
			return position == position && velocity == velocity && Math::Equals(angle, other.angle) &&
				Math::Equals(angleSpeed, other.angleSpeed) && Math::Equals(time, other.time) && size == other.size &&
				color == other.color && alive == other.alive;
		}
	};
}
