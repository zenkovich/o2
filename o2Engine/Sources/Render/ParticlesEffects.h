#pragma once

#include "Utils/Serializable.h"
#include "Render/Particle.h"

namespace o2
{
	class ParticlesEmitter;

	// -------------------------------
	// Particles effect base interface
	// -------------------------------
	class ParticlesEffect: public ISerializable
	{
		SERIALIZABLE(ParticlesEffect);

	public:
		virtual void Update(float dt, ParticlesEmitter* emitter);
		Vector<Particle>& GetParticlesDirect(ParticlesEmitter* emitter);
	};
}
