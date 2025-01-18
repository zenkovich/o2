#pragma once

#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Math/Color.h"

namespace o2
{
    class Particle
    {
    public:
        int index = 0; // Index of particle in container

        Vec2F position; // Position of particle center
        Vec2F velocity; // Particle velocity

        float angle = 0;      // Particle angle in radians
        float angleSpeed = 0; // Angle speed in radians/sec

        Vec2F size; // Size of particle

        Color4 color; // Particle's color

        float timeLeft = 0; // Estimate life time
        float lifetime = 0; // Total life time

        bool alive = false; // Is particle alive

    public:
        bool operator==(const Particle& other) const
        {
            return position == position && velocity == velocity && Math::Equals(angle, other.angle) &&
                Math::Equals(angleSpeed, other.angleSpeed) && Math::Equals(timeLeft, other.timeLeft) && size == other.size &&
                color == other.color && alive == other.alive;
        }
    };
}
