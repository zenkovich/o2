#include "stdafx.h"
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

DECLARE_CLASS(o2::ParticlesEffect);
