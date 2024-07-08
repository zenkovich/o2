#include "o2/stdafx.h"
#include "ParticlesEffects.h"

#include "o2/Render/Particles/ParticlesEmitter.h"

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
// --- META ---

DECLARE_CLASS(o2::ParticlesEffect, o2__ParticlesEffect);

DECLARE_CLASS(o2::ParticlesGravityEffect, o2__ParticlesGravityEffect);
// --- END META ---
