#pragma once

#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	// --------------------------------------
	// Particles emitter shape base interface
	// --------------------------------------
	class ParticlesEmitterShape: public ISerializable
	{
		SERIALIZABLE(ParticlesEmitterShape);

	public:
		virtual ~ParticlesEmitterShape() {}
		virtual Vec2F GetEmittinPoint();
	};

	// ---------------------------------
	// Circle with radius emitting shape
	// ---------------------------------
	class CircleParticlesEmitterShape: public ParticlesEmitterShape
	{
		SERIALIZABLE(CircleParticlesEmitterShape);

	public:
		float radius = 0;

		Vec2F GetEmittinPoint();
	};

	// ---------------------------------
	// Square with radius emitting shape
	// ---------------------------------
	class SquareParticlesEmitterShape: public ParticlesEmitterShape
	{
		SERIALIZABLE(SquareParticlesEmitterShape);

	public:
		Vec2F size;

		Vec2F GetEmittinPoint();
	};
}

CLASS_BASES_META(o2::ParticlesEmitterShape)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitterShape)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEmitterShape)
{

	PUBLIC_FUNCTION(Vec2F, GetEmittinPoint);
}
END_META;

CLASS_BASES_META(o2::CircleParticlesEmitterShape)
{
	BASE_CLASS(o2::ParticlesEmitterShape);
}
END_META;
CLASS_FIELDS_META(o2::CircleParticlesEmitterShape)
{
	PUBLIC_FIELD(radius);
}
END_META;
CLASS_METHODS_META(o2::CircleParticlesEmitterShape)
{

	PUBLIC_FUNCTION(Vec2F, GetEmittinPoint);
}
END_META;

CLASS_BASES_META(o2::SquareParticlesEmitterShape)
{
	BASE_CLASS(o2::ParticlesEmitterShape);
}
END_META;
CLASS_FIELDS_META(o2::SquareParticlesEmitterShape)
{
	PUBLIC_FIELD(size);
}
END_META;
CLASS_METHODS_META(o2::SquareParticlesEmitterShape)
{

	PUBLIC_FUNCTION(Vec2F, GetEmittinPoint);
}
END_META;
