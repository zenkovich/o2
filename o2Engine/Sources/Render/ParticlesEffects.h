#pragma once

#include "Utils/Serialization/Serializable.h"
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

CLASS_BASES_META(o2::ParticlesEffect)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEffect)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEffect)
{

	PUBLIC_FUNCTION(void, Update, float, ParticlesEmitter*);
	PUBLIC_FUNCTION(Vector<Particle>&, GetParticlesDirect, ParticlesEmitter*);
}
END_META;
