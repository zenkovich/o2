#include "ParticlesEffects.h"

#include "Render/ParticlesEmitter.h"

namespace o2
{
	void ParticlesEffect::Update(float dt, ParticlesEmitter* emitter)
	{}

	Vector<Particle>& ParticlesEffect::GetParticlesDirect(ParticlesEmitter* emitter)
	{
		return emitter->mParticles;
	}
}

CLASS_META(o2::ParticlesEffect)
{
	BASE_CLASS(o2::ISerializable);


	PUBLIC_FUNCTION(void, Update, float, ParticlesEmitter*);
	PUBLIC_FUNCTION(Vector<Particle>&, GetParticlesDirect, ParticlesEmitter*);
}
END_META;
