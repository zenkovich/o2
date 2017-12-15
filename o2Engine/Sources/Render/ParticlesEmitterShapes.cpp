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

CLASS_META(o2::ParticlesEmitterShape)
{
	BASE_CLASS(o2::ISerializable);


	PUBLIC_FUNCTION(Vec2F, GetEmittinPoint);
}
END_META;

CLASS_META(o2::CircleParticlesEmitterShape)
{
	BASE_CLASS(o2::ParticlesEmitterShape);

	PUBLIC_FIELD(radius);

	PUBLIC_FUNCTION(Vec2F, GetEmittinPoint);
}
END_META;

CLASS_META(o2::SquareParticlesEmitterShape)
{
	BASE_CLASS(o2::ParticlesEmitterShape);

	PUBLIC_FIELD(size);

	PUBLIC_FUNCTION(Vec2F, GetEmittinPoint);
}
END_META;
