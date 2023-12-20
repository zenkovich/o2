#pragma once

#include "o2/Render/Particle.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    class ParticlesEmitter;

    // -------------------------------
    // Particles effect base interface
    // -------------------------------
    class ParticlesEffect: public ISerializable, public RefCounterable
    {
        SERIALIZABLE(ParticlesEffect);

    public:
        virtual void Update(float dt, ParticlesEmitter* emitter);
        Vector<Particle>& GetParticlesDirect(ParticlesEmitter* emitter);
    };

    class ParticlesGravityEffect : public ParticlesEffect
    {
        SERIALIZABLE(ParticlesGravityEffect);

    public:
        Vec2F gravity; // Vector of gravity @SERIALIZABLE

    public:
        void Update(float dt, ParticlesEmitter* emitter) override;
    };
}
// --- META ---

CLASS_BASES_META(o2::ParticlesEffect)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEffect)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEffect)
{

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
// --- END META ---
