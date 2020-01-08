#include "o2/stdafx.h"
#include "ParticlesEmitterShapes.h"

namespace o2
{
	Vec2F ParticlesEmitterShape::GetEmittinPoint()
	{
		return Vec2F();
	}

	Vec2F CircleParticlesEmitterShape::GetEmittinPoint()
	{
		return Vec2F::Rotated(Math::Random(0.0f, Math::PI()*2.0f))*radius;
	}

	Vec2F SquareParticlesEmitterShape::GetEmittinPoint()
	{
		Vec2F hs = size*0.5f;
		return Vec2F(Math::Random(-hs.x, hs.x), Math::Random(-hs.y, hs.y));
	}
}

DECLARE_CLASS(o2::ParticlesEmitterShape);

DECLARE_CLASS(o2::CircleParticlesEmitterShape);

DECLARE_CLASS(o2::SquareParticlesEmitterShape);
