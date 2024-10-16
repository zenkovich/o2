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
		// Called when particle is emitted, used to initialize effect data
		virtual void OnParticleEmitted(Particle& particle) {}

		// Called when particle is died, used to cleanup effect data
		virtual void OnParticleDied(Particle& particle) {}

		// Called each frame to update effect data
        virtual void Update(float dt, ParticlesEmitter* emitter);

		// Get particles directly from emitter
        Vector<Particle>& GetParticlesDirect(ParticlesEmitter* emitter);

		SERIALIZABLE(ParticlesEffect);
		CLONEABLE_REF(ParticlesEffect);

	protected:
		WeakRef<ParticlesEmitter> mEmitter; // Owning emitter

	protected:
		// Called  when particle effect parameters are changed, used to invalidate baked frames
		void OnChanged();

		friend class ParticlesEmitter;
	};

	// ------------------------
	// Particles gravity effect
	// ------------------------
	class ParticlesGravityEffect : public ParticlesEffect
	{
	public:
		PROPERTIES(ParticlesGravityEffect);
		PROPERTY(Vec2F, gravity, SetGravity, GetGravity);

	public:
		// Set gravity vector
		void SetGravity(const Vec2F& gravity) { mGravity = gravity; OnChanged(); }

		// Get gravity vector
		const Vec2F& GetGravity() const { return mGravity; }

		// Update particles velocity with gravity vector
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesGravityEffect);
		CLONEABLE_REF(ParticlesGravityEffect);

	protected:
		Vec2F mGravity; // Vector of gravity @SERIALIZABLE
	};

	// --------------------------------
	// Particles color over time effect
	// --------------------------------
	class ParticlesColorEffect : public ParticlesEffect
	{
	public:
		Ref<ColorGradient> colorGradient; // Color gradient @SERIALIZABLE

	public:
		ParticlesColorEffect();

		// Called when particle is emitted, used to initialize effect data
		void OnParticleEmitted(Particle& particle) override;

		// Update particles color over time
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesColorEffect);
		CLONEABLE_REF(ParticlesColorEffect);

	private:
		struct ParticleColorData
		{
			int cacheKey = 0;

			bool operator==(const ParticleColorData& other) const
			{
				return cacheKey == other.cacheKey;
			}
		};

		Vector<ParticleColorData> mColorData; // Color data buffer

	private:
		// Check if color data buffer size is enough
		void CheckDataBufferSize(int particlesCount);

		// Called when deserialization is done, used to subscribe to color gradient changes
		void OnDeserialized(const DataValue& node) override;
	};

	// ---------------------------------------------------
	// Particles random color between two over time effect
	// ---------------------------------------------------
	class ParticlesRandomColorEffect : public ParticlesEffect
	{
	public:
		Ref<ColorGradient> colorGradientA; // Color gradient A @SERIALIZABLE
		Ref<ColorGradient> colorGradientB; // Color gradient B @SERIALIZABLE

	public:
		// Default constructor
		ParticlesRandomColorEffect();

		// Called when particle is emitted, used to initialize effect data
		void OnParticleEmitted(Particle& particle) override;

		// Update particles color between two gradients over time
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesRandomColorEffect);
		CLONEABLE_REF(ParticlesRandomColorEffect);

	private:
		struct ParticleColorData
		{
			int cacheKeyA = 0;
			int cacheKeyB = 0;

			float coef = 0.0f;

			bool operator==(const ParticleColorData& other) const
			{
				return cacheKeyA == other.cacheKeyA && cacheKeyB == other.cacheKeyB && coef == other.coef;
			}
		};

		Vector<ParticleColorData> mColorData; // Color data buffer

	private:
		void CheckDataBufferSize(int particlesCount);

		// Called when deserialization is done, used to subscribe to color gradient changes
		void OnDeserialized(const DataValue& node) override;
	};

	// -------------------------------
	// Particles size over time effect
	// -------------------------------
	class ParticlesSizeEffect : public ParticlesEffect
	{
	public:
		Ref<Curve> sizeCurve; // Size curve @SERIALIZABLE

	public:
		// Default constructor
		ParticlesSizeEffect();

		// Called when particle is emitted, used to initialize effect data
		void OnParticleEmitted(Particle& particle) override;

		// Update particles size over time
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesSizeEffect);
		CLONEABLE_REF(ParticlesSizeEffect);

	private:
		struct ParticleSizeData
		{
			Vec2F initialSize;

			int cacheKey = 0;
			int cacheKeyApprox = 0;

			bool operator==(const ParticleSizeData& other) const
			{
				return cacheKey == other.cacheKey;
			}
		};

		Vector<ParticleSizeData> mSizeData; // Size data buffer

	private:
		void CheckDataBufferSize(int particlesCount);

		// Called when deserialization is done, used to subscribe to size curve changes
		void OnDeserialized(const DataValue& node) override;
	};

	// -------------------------------
	// Particles size over time effect
	// -------------------------------
	class ParticlesRandomSizeEffect : public ParticlesEffect
	{
	public:
		Ref<Curve> sizeCurveA; // Size curve A @SERIALIZABLE
		Ref<Curve> sizeCurveB; // Size curve A @SERIALIZABLE

	public:
		// Default constructor
		ParticlesRandomSizeEffect();

		// Called when particle is emitted, used to initialize effect data
		void OnParticleEmitted(Particle& particle) override;

		// Update particles size over time
		void Update(float dt, ParticlesEmitter* emitter) override;

		SERIALIZABLE(ParticlesRandomSizeEffect);
		CLONEABLE_REF(ParticlesRandomSizeEffect);

	private:
		struct ParticleSizeData
		{
			Vec2F initialSize;
			float coef = 0.0f;

			int cacheKeyA = 0;
			int cacheKeyApproxA = 0;

			int cacheKeyB = 0;
			int cacheKeyApproxB = 0;

			bool operator==(const ParticleSizeData& other) const
			{
				return cacheKeyA == other.cacheKeyA && cacheKeyB == other.cacheKeyB;
			}
		};

		Vector<ParticleSizeData> mSizeData; // Size data buffer

	private:
		void CheckDataBufferSize(int particlesCount);

		// Called when deserialization is done, used to subscribe to size curve changes
		void OnDeserialized(const DataValue& node) override;
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
    FIELD().PROTECTED().NAME(mEmitter);
}
END_META;
CLASS_METHODS_META(o2::ParticlesEffect)
{

    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleEmitted, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleDied, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
    FUNCTION().PUBLIC().SIGNATURE(Vector<Particle>&, GetParticlesDirect, ParticlesEmitter*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnChanged);
}
END_META;

CLASS_BASES_META(o2::ParticlesGravityEffect)
{
    BASE_CLASS(o2::ParticlesEffect);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesGravityEffect)
{
    FIELD().PUBLIC().NAME(gravity);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mGravity);
}
END_META;
CLASS_METHODS_META(o2::ParticlesGravityEffect)
{

    FUNCTION().PUBLIC().SIGNATURE(void, SetGravity, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(const Vec2F&, GetGravity);
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
    FUNCTION().PRIVATE().SIGNATURE(void, CheckDataBufferSize, int);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&);
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
    FUNCTION().PRIVATE().SIGNATURE(void, CheckDataBufferSize, int);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;

CLASS_BASES_META(o2::ParticlesSizeEffect)
{
    BASE_CLASS(o2::ParticlesEffect);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesSizeEffect)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(sizeCurve);
    FIELD().PRIVATE().NAME(mSizeData);
}
END_META;
CLASS_METHODS_META(o2::ParticlesSizeEffect)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleEmitted, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
    FUNCTION().PRIVATE().SIGNATURE(void, CheckDataBufferSize, int);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;

CLASS_BASES_META(o2::ParticlesRandomSizeEffect)
{
    BASE_CLASS(o2::ParticlesEffect);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesRandomSizeEffect)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(sizeCurveA);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(sizeCurveB);
    FIELD().PRIVATE().NAME(mSizeData);
}
END_META;
CLASS_METHODS_META(o2::ParticlesRandomSizeEffect)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, OnParticleEmitted, Particle&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float, ParticlesEmitter*);
    FUNCTION().PRIVATE().SIGNATURE(void, CheckDataBufferSize, int);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
