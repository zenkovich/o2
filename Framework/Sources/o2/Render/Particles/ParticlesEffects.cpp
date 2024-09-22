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

	void ParticlesEffect::OnChanged()
	{
		if (mEmitter)
			mEmitter.Lock() ->InvalidateBakedFrames();
	}

	void ParticlesGravityEffect::Update(float dt, ParticlesEmitter* emitter)
    {
        Vec2F v = mGravity*dt;
        for (auto& p : GetParticlesDirect(emitter))
            p.velocity += v;
    }

	ParticlesColorEffect::ParticlesColorEffect()
	{
		colorGradient = mmake<ColorGradient>();
		colorGradient->onKeysChanged += [this]() { OnChanged(); };
	}

	void ParticlesColorEffect::OnParticleEmitted(Particle& particle)
	{
		int particleIndex = particle.index;

		CheckDataBufferSize(particleIndex);

		mColorData[particleIndex].cacheKey = 0;
	}

	void ParticlesColorEffect::Update(float dt, ParticlesEmitter* emitter)
	{
		auto& particles = GetParticlesDirect(emitter);

		CheckDataBufferSize(particles.Count());

		for (auto& p : particles)
		{
			int particleIndex = p.index;

			auto& colorData = mColorData[particleIndex];
			p.color = colorGradient->Evaluate(1.0f - p.timeLeft/p.lifetime, true, colorData.cacheKey);
		}
	}

	void ParticlesColorEffect::CheckDataBufferSize(int particlesCount)
	{
		for (int i = mColorData.size(); i <= particlesCount; i++)
			mColorData.Add(ParticleColorData());
	}

	void ParticlesColorEffect::OnDeserialized(const DataValue& node)
	{
		colorGradient->onKeysChanged += [this]() { OnChanged(); };
	}

	ParticlesRandomColorEffect::ParticlesRandomColorEffect()
	{
		colorGradientA = mmake<ColorGradient>();
		colorGradientB = mmake<ColorGradient>();

		colorGradientA->onKeysChanged += [this]() { OnChanged(); };
		colorGradientB->onKeysChanged += [this]() { OnChanged(); };
	}

	void ParticlesRandomColorEffect::CheckDataBufferSize(int particlesCount)
	{
		for (int i = mColorData.size(); i <= particlesCount; i++)
			mColorData.Add(ParticleColorData());
	}

	void ParticlesRandomColorEffect::OnDeserialized(const DataValue& node)
	{
		colorGradientA->onKeysChanged += [this]() { OnChanged(); };
		colorGradientB->onKeysChanged += [this]() { OnChanged(); };
	}

	void ParticlesRandomColorEffect::OnParticleEmitted(Particle& particle)
	{
		int particleIndex = particle.index;

		CheckDataBufferSize(particleIndex);

		auto& colorData = mColorData[particleIndex];
		colorData.cacheKeyA = 0;
		colorData.cacheKeyB = 0;
		colorData.coef = Math::Random(0.0f, 1.0f);
	}

	void ParticlesRandomColorEffect::Update(float dt, ParticlesEmitter* emitter)
	{
		auto& particles = GetParticlesDirect(emitter);

		CheckDataBufferSize(particles.Count());

		for (auto& p : particles)
		{
			int particleIndex = p.index;

			auto& colorData = mColorData[particleIndex];
			auto lifeTimeCoef = 1.0f - p.timeLeft / p.lifetime;
			auto colorA = colorGradientA->Evaluate(lifeTimeCoef, true, colorData.cacheKeyA);
			auto colorB = colorGradientB->Evaluate(lifeTimeCoef, true, colorData.cacheKeyB);
			p.color = Math::Lerp(colorA, colorB, colorData.coef);
		}
	}

	ParticlesSizeEffect::ParticlesSizeEffect()
	{
		sizeCurve = mmake<Curve>(Curve::Linear(1.0f, 1.0f));
		sizeCurve->onKeysChanged += [this]() { OnChanged(); };
	}

	void ParticlesSizeEffect::OnParticleEmitted(Particle& particle)
	{
		int particleIndex = particle.index;

		CheckDataBufferSize(particleIndex);

		mSizeData[particleIndex].initialSize = particle.size;
		mSizeData[particleIndex].cacheKey = 0;
		mSizeData[particleIndex].cacheKeyApprox = 0;
	}

	void ParticlesSizeEffect::Update(float dt, ParticlesEmitter* emitter)
	{
		auto& particles = GetParticlesDirect(emitter);

		CheckDataBufferSize(particles.Count());

		for (auto& p : particles)
		{
			int particleIndex = p.index;

			auto& sizeData = mSizeData[particleIndex];
			p.size = sizeData.initialSize*sizeCurve->Evaluate(1.0f - p.timeLeft/p.lifetime, true, sizeData.cacheKey, sizeData.cacheKeyApprox);
		}
	}

	void ParticlesSizeEffect::CheckDataBufferSize(int particlesCount)
	{
		for (int i = mSizeData.size(); i <= particlesCount; i++)
			mSizeData.Add(ParticleSizeData());
	}

	void ParticlesSizeEffect::OnDeserialized(const DataValue& node)
	{
		sizeCurve->onKeysChanged += [this]() { OnChanged(); };
	}

}
// --- META ---

DECLARE_CLASS(o2::ParticlesEffect, o2__ParticlesEffect);

DECLARE_CLASS(o2::ParticlesGravityEffect, o2__ParticlesGravityEffect);

DECLARE_CLASS(o2::ParticlesColorEffect, o2__ParticlesColorEffect);

DECLARE_CLASS(o2::ParticlesRandomColorEffect, o2__ParticlesRandomColorEffect);

DECLARE_CLASS(o2::ParticlesSizeEffect, o2__ParticlesSizeEffect);
// --- END META ---
