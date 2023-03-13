#pragma once

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	// --------------------------------------
	// Particles emitter shape base interface
	// --------------------------------------
	class ParticlesEmitterShape : public ISerializable, public RefCounterable
	{
		SERIALIZABLE(ParticlesEmitterShape);

	public:
		// Virtual destructor
		virtual ~ParticlesEmitterShape() {}

		// Returns next random generated emitting point inside shape
		virtual Vec2F GetEmittingPoint();
	};

	// ---------------------------------
	// Circle with radius emitting shape
	// ---------------------------------
	class CircleParticlesEmitterShape : public ParticlesEmitterShape
	{
		SERIALIZABLE(CircleParticlesEmitterShape);

	public:
		float radius = 0; // Emitting shape radius @SERIALIZABLE

	public:
		// Returns next random generated emitting point inside circle shape
		Vec2F GetEmittingPoint() override;
	};

	// -------------------------------
	// Square with size emitting shape
	// -------------------------------
	class SquareParticlesEmitterShape : public ParticlesEmitterShape
	{
		SERIALIZABLE(SquareParticlesEmitterShape);

	public:
		Vec2F size; // Emitting shape size @SERIALIZABLE

	public:
		// Returns next random generated emitting point inside square shape
		Vec2F GetEmittingPoint() override;
	};
}

CLASS_BASES_META(o2::ParticlesEmitterShape)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitterShape)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEmitterShape)
{

	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetEmittingPoint);
}
END_META;

CLASS_BASES_META(o2::CircleParticlesEmitterShape)
{
	BASE_CLASS(o2::ParticlesEmitterShape);
}
END_META;
CLASS_FIELDS_META(o2::CircleParticlesEmitterShape)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(radius);
}
END_META;
CLASS_METHODS_META(o2::CircleParticlesEmitterShape)
{

	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetEmittingPoint);
}
END_META;

CLASS_BASES_META(o2::SquareParticlesEmitterShape)
{
	BASE_CLASS(o2::ParticlesEmitterShape);
}
END_META;
CLASS_FIELDS_META(o2::SquareParticlesEmitterShape)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(size);
}
END_META;
CLASS_METHODS_META(o2::SquareParticlesEmitterShape)
{

	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetEmittingPoint);
}
END_META;
