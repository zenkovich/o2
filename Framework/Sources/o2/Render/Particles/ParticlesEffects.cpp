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

	ParticlesColorEffect::ParticlesColorEffect()
	{
		colorGradient = mmake<ColorGradient>();
	}

	void ParticlesColorEffect::OnParticleEmitted(Particle& particle)
	{
		int particleIndex = particle.index;

		for (int i = mColorData.size(); i <= particleIndex; i++)
			mColorData.Add(ParticleColorData());

		mColorData[particleIndex].cacheKey = 0;
	}

	void ParticlesColorEffect::Update(float dt, ParticlesEmitter* emitter)
	{
		for (auto& p : GetParticlesDirect(emitter))
		{
			int particleIndex = p.index;

			auto& colorData = mColorData[particleIndex];
			p.color = colorGradient->Evaluate(1.0f - p.timeLeft/p.lifetime, true, colorData.cacheKey);
		}
	}

	ParticlesRandomColorEffect::ParticlesRandomColorEffect()
	{
		colorGradientA = mmake<ColorGradient>();
		colorGradientB = mmake<ColorGradient>();
	}

	void ParticlesRandomColorEffect::OnParticleEmitted(Particle& particle)
	{
		int particleIndex = particle.index;

		for (int i = mColorData.size(); i <= particleIndex; i++)
			mColorData.Add(ParticleColorData());

		auto& colorData = mColorData[particleIndex];
		colorData.cacheKeyA = 0;
		colorData.cacheKeyB = 0;
		colorData.coef = Math::Random(0.0f, 1.0f);
	}

	void ParticlesRandomColorEffect::Update(float dt, ParticlesEmitter* emitter)
	{
		for (auto& p : GetParticlesDirect(emitter))
		{
			int particleIndex = p.index;

			auto& colorData = mColorData[particleIndex];
			auto lifeTimeCoef = 1.0f - p.timeLeft / p.lifetime;
			auto colorA = colorGradientA->Evaluate(lifeTimeCoef, true, colorData.cacheKeyA);
			auto colorB = colorGradientB->Evaluate(lifeTimeCoef, true, colorData.cacheKeyB);
			p.color = Math::Lerp(colorA, colorB, colorData.coef);
		}
	}

}
// --- META ---

DECLARE_CLASS(o2::ParticlesEffect, o2__ParticlesEffect);

DECLARE_CLASS(o2::ParticlesGravityEffect, o2__ParticlesGravityEffect);

DECLARE_CLASS(o2::ParticlesColorEffect, o2__ParticlesColorEffect);

DECLARE_CLASS(o2::ParticlesRandomColorEffect, o2__ParticlesRandomColorEffect);
// --- END META ---
