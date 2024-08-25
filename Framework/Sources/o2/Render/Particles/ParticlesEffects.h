#pragma once

#include "o2/Render/Particles/Particle.h"
#include "o2/Utils/Math/ColorGradient.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    class ParticlesEmitter;

    // -------------------------------
    // Particles effect base interface
    // -------------------------------
    class ParticlesEffect: public ISerializable, public RefCounterable, public ICloneableRef
    {
	public:
		virtual void OnParticleEmitted(Particle& particle) {}
		virtual void OnParticleDied(Particle& particle) {}
        virtual void Update(float dt, ParticlesEmitter* emitter);
        Vector<Particle>& GetParticlesDirect(ParticlesEmitter* emitter);

		SERIALIZABLE(ParticlesEffect);
		CLONEABLE_REF(ParticlesEffect);
	};

	class ParticlesGravityEffect : public ParticlesEffect
	{
	public:
		Vec2F gravity; // Vector of gravity @SERIALIZABLE

	public:
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesGravityEffect);
		CLONEABLE_REF(ParticlesGravityEffect);
	};

	class ParticlesColorEffect : public ParticlesEffect
	{
	public:
		Ref<ColorGradient> colorGradient; // Color gradient @SERIALIZABLE

	public:
		ParticlesColorEffect();

		void OnParticleEmitted(Particle& particle) override;
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesColorEffect);
		CLONEABLE_REF(ParticlesColorEffect);

	private:
		struct ParticleColorData
		{
			int cacheKey = -1;

			bool operator==(const ParticleColorData& other) const
			{
				return cacheKey == other.cacheKey;
			}
		};

		Vector<ParticleColorData> mColorData;
	};

	class ParticlesRandomColorEffect : public ParticlesEffect
	{
	public:
		Ref<ColorGradient> colorGradientA; // Color gradient A @SERIALIZABLE
		Ref<ColorGradient> colorGradientB; // Color gradient B @SERIALIZABLE

	public:
		ParticlesRandomColorEffect();

		void OnParticleEmitted(Particle& particle) override;
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesRandomColorEffect);
		CLONEABLE_REF(ParticlesRandomColorEffect);

	private:
		struct ParticleColorData
		{
			int cacheKeyA = -1;
			int cacheKeyB = -1;

			float coef = 0.0f;

			bool operator==(const ParticleColorData& other) const
			{
				return cacheKeyA == other.cacheKeyA && cacheKeyB == other.cacheKeyB && coef == other.coef;
			}
		};

		Vector<ParticleColorData> mColorData;
	};
}
// --- META ---

CLASS_BASES_META(o2::ParticlesEffect)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
    BASE_CLASS(o2::ICloneableRef);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEffect)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEffect)
{

    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleEmitted, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleDied, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
    FUNCTION().PUBLIC().SIGNATURE(Vector<Particle>&, GetParticlesDirect, ParticlesEmitter*);
}
END_META;

CLASS_BASES_META(o2::ParticlesGravityEffect)
{
    BASE_CLASS(o2::ParticlesEffect);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesGravityEffect)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(gravity);
}
END_META;
CLASS_METHODS_META(o2::ParticlesGravityEffect)
{

    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
}
END_META;

CLASS_BASES_META(o2::ParticlesColorEffect)
{
    BASE_CLASS(o2::ParticlesEffect);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesColorEffect)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(colorGradient);
    FIELD().PRIVATE().NAME(mColorData);
}
END_META;
CLASS_METHODS_META(o2::ParticlesColorEffect)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleEmitted, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
}
END_META;

CLASS_BASES_META(o2::ParticlesRandomColorEffect)
{
    BASE_CLASS(o2::ParticlesEffect);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesRandomColorEffect)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(colorGradientA);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(colorGradientB);
    FIELD().PRIVATE().NAME(mColorData);
}
END_META;
CLASS_METHODS_META(o2::ParticlesRandomColorEffect)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleEmitted, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
}
END_META;
// --- END META ---
