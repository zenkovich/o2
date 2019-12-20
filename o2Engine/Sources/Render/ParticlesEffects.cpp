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

	void ParticlesGravityEffect::Update(float dt, ParticlesEmitter* emitter)
	{
		Vec2F v = gravity*dt;
		for (auto& p : GetParticlesDirect(emitter))
			p.velocity += v;
	}
}

DECLARE_CLASS(o2::ParticlesEffect);

DECLARE_CLASS(o2::ParticlesGravityEffect);
