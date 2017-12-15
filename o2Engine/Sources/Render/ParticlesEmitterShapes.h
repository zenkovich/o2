#pragma once

#include "Utils/Serializable.h"

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
