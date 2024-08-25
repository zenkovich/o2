#pragma once

#include "o2/Animation/IAnimation.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Particles/Particle.h"
#include "o2/Render/Particles/ParticlesContainer.h"
#include "o2/Render/Particles/ParticlesEffects.h"
#include "o2/Render/Particles/ParticlesEmitterShapes.h"
#include "o2/Render/RectDrawable.h"
#include "o2/Utils/Editor/Attributes/RangeAttribute.h"
#include "o2/Utils/Math/ColorGradient.h"
#include "o2/Utils/Math/Curve.h"

namespace o2
{
    class Mesh;

    // ------------------------------------------------------
    // Particles emitter. Emits, updates and manage particles
    // ------------------------------------------------------
    class ParticlesEmitter: public IRectDrawable, public IAnimation
    {
    public:
		PROPERTIES(ParticlesEmitter);

		PROPERTY(bool, playing, SetPlaying, IsPlaying);                                 // Is particles playing property
		PROPERTY(bool, looped, SetLoop, IsLooped);                                      // Is emitter looped property
		PROPERTY(float, duration, SetDuration, GetDuration);                            // Working duration in seconds property @RANGE(0, 10)
		PROPERTY(bool, particlesRelative, SetParticlesRelativity, IsParticlesRelative); // Is particles relative to emitter

		PROPERTY(Ref<ParticlesEmitterShape>, shape, SetShape, GetShape);                        // Emitting shape property @EXPANDED_BY_DEFAULT @DONT_DELETE @DEFAULT_TYPE(o2::CircleParticlesEmitterShape)
		PROPERTY(Ref<ParticleSource>, particlesSource, SetParticlesSource, GetParticlesSource); // Particles source property @EXPANDED_BY_DEFAULT @DONT_DELETE @DEFAULT_TYPE(o2::SingleSpriteParticleSource)

        PROPERTY(int, maxParticles, SetMaxParticles, GetMaxParticles); // Number of maximum particles in emitter property

        PROPERTY(float, particlesLifetime, SetParticlesLifetime, GetParticlesLifetime);    // Particles lifetime in seconds property @RANGE(0, 10)
		PROPERTY(float, particlesPerSecond, SetParticlesPerSecond, GetParticlesPerSecond); // Amount of particles emitting in one second property 
		
		PROPERTY(float, initialAngle, SetInitialAngle, GetInitialAngle);                // Emitting particle angle property in degrees @RANGE(0, 360)
		PROPERTY(float, initialAngleRange, SetInitialAngleRange, GetInitialAngleRange); // Emitting particle angle range property in degrees @RANGE(0, 360)
		
		PROPERTY(Vec2F, initialSize, SetInitialSize, GetInitialSize);                // Emitting particle size property
		PROPERTY(Vec2F, initialSizeRange, SetInitialSizeRange, GetInitialSizeRange); // Emitting particle size range property
		
		PROPERTY(float, initialSpeed, SetInitialSpeed, GetInitialSpeed);                // Emitting particle angle speed property in degrees/sec
		PROPERTY(float, initialSpeedRange, SetInitialSpeedRange, GetInitialSpeedRange); // Emitting particle angle speed range in degrees/sec
        
		PROPERTY(float, initialAngleSpeed, SetInitialAngleSpeed, GetInitialAngleSpeed);                // Emitting particle speed property
		PROPERTY(float, initialAngleSpeedRange, SetInitialAngleSpeedRange, GetInitialAngleSpeedRange); // Emitting particle speed range
       
        PROPERTY(float, moveDirection, SetEmitParticlesMoveDirection, GetEmitParticlesMoveDirection);                // Emitting particle moving direction in degrees property
        PROPERTY(float, moveDirectionRange, SetEmitParticlesMoveDirectionRange, GetEmitParticlesMoveDirectionRange); // Emitting particle moving direction range in degrees property 

		PROPERTY(float, emittingCoefficient, SetEmittingCoef, GetEmittingCoef); // Particles emitting coefficient property (0...1) @RANGE(0, 1)

    public:
        // Default constructor
        ParticlesEmitter();

        // Copy-constructor
        ParticlesEmitter(const ParticlesEmitter& other);

        // Destructor
        ~ParticlesEmitter();

        // Copy-operator
        ParticlesEmitter& operator=(const ParticlesEmitter& other);

        // DRaws particles
        void Draw() override;

        // Updates particles
        void Update(float dt);

        // Starts playing
        void Play() override;

        // Stops playing
        void Stop() override;

        // Sets particles source
		void SetParticlesSource(const Ref<ParticleSource>& source);

		// Returns particles source
		const Ref<ParticleSource>& GetParticlesSource() const;

        // Sets particles emitting coefficient (0...1)
        void SetEmittingCoef(float coef);

        // Returns particles emitting coefficient
        float GetEmittingCoef() const;

        // Sets emitting shape
        void SetShape(const Ref<ParticlesEmitterShape>& shape);

        // Return emitting shape
        const Ref<ParticlesEmitterShape>& GetShape() const;

        // Adds effect
        void AddEffect(const Ref<ParticlesEffect>& effect);

        // Adds effect
        template<typename _type, typename ... _args>
        void AddEffect(_args ... args);

        // Returns list of effects
        const Vector<Ref<ParticlesEffect>>& GetEffects() const;

        // Removes effect
        void RemoveEffect(const Ref<ParticlesEffect>& effect);

        // Removes all effects
        void RemoveAllEffects();

        // Set particles limit number
        void SetMaxParticles(int count);

        // Returns particles limit number
        int GetMaxParticles() const;

        // Returns current particles count
        int GetParticlesCount() const;

        // Returns has alive particles
        bool IsAliveParticles() const;

        // Returns particles list
        const Vector<Particle>& GetParticles() const;

        // Sets particles relativity
        void SetParticlesRelativity(bool relative);

        // Is particles relative to emitter
        bool IsParticlesRelative() const;

        // Sets emitter looped
        void SetLoop(bool looped);

        // Returns is emitter looped
        bool IsLooped() const;

        // Sets working duration
        void SetDuration(float duration);

        // Returns working duration
        float GetDuration() const;

        // Sets particles lifetime in seconds
        void SetParticlesLifetime(float lifetime);

        // Returns particles lifetime in seconds
        float GetParticlesLifetime() const;

        // Sets number of particles emitting per second
        void SetParticlesPerSecond(float numParticles);

        // Returns number of particles emitting per second
        float GetParticlesPerSecond() const;

        // Sets emitting particles rotation angle in degrees
        void SetInitialAngle(float angle);

        // Returns emitting particles rotation angle in degrees
        float GetInitialAngle() const;

        // Sets emitting particles rotation angle range in degrees
        void SetInitialAngleRange(float range);

        // Returns emitting particles rotation angle range in degrees
        float GetInitialAngleRange() const;

        // Sets emitting particles size
        void SetInitialSize(const Vec2F& size);

        // Returns emitting particles size
        Vec2F GetInitialSize() const;

        // Sets emitting particles size range
        void SetInitialSizeRange(const Vec2F& range);

        // Returns emitting particles size range
        Vec2F GetInitialSizeRange() const;

        // Sets emitting particles angle speed in degrees/sec
        void SetInitialAngleSpeed(float speed);

        // Returns emitting particles angle speed in degrees/sec
        float GetInitialAngleSpeed() const;

        // Sets emitting particles angle speed range in degrees/sec
        void SetInitialAngleSpeedRange(float speedRange);

        // Returns emitting particles angle speed range in degrees/sec
        float GetInitialAngleSpeedRange() const;

        // Sets emitting particles speed
        void SetInitialSpeed(float speed);

        // Returns emitting particles speed
        float GetInitialSpeed() const;

        // Sets emitting particles speed range
        void SetInitialSpeedRange(float speedRange);

        // Returns emitting particles speed range
        float GetInitialSpeedRange() const;

        // Sets emitting particles moving direction angle in degrees
        void SetEmitParticlesMoveDirection(float direction);

        // Returns emitting particles moving direction angle in degrees
        float GetEmitParticlesMoveDirection() const;

        // Sets emitting particles moving direction angle range in degrees
        void SetEmitParticlesMoveDirectionRange(float directionRange);

        // Returns emitting particles moving direction angle range in degrees
        float GetEmitParticlesMoveDirectionRange() const;

        SERIALIZABLE(ParticlesEmitter);
        CLONEABLE_REF(ParticlesEmitter);

	protected:
		Ref<ParticleSource> mParticlesSource = mmake<SingleSpriteParticleSource>(); // Source of particles @SERIALIZABLE 

		Ref<ParticlesContainer> mParticlesContainer; // Particles container, stores and updates particles, created from particles source

        Ref<ParticlesEmitterShape> mShape = nullptr; // Particles emitting shape @SERIALIZABLE 

        Vector<Ref<ParticlesEffect>> mEffects; // Particles effect @SERIALIZABLE @EDITOR_PROPERTY @EXPANDED_BY_DEFAULT @DONT_DELETE
                                                                         
        int mParticlesNumLimit = 100; // Max available visible particles @SERIALIZABLE
                                       
        float mEmittingCoefficient = 1.0f; // Emitting particles number coefficient (0...1) @SERIALIZABLE
        bool  mIsParticlesRelative = true; // Is particles relative to emitter or global @SERIALIZABLE
        bool  mIsLooped = false;           // Is emitter looped @SERIALIZABLE
                                                                  
        float mDuration = 1; // Duration of working time @SERIALIZABLE
                                                                  
		float mParticlesLifetime = 0.5f;      // Particles lifetime in seconds @SERIALIZABLE
		float mParticlesLifetimeRange = 0.0f; // Particles lifetime range in seconds @SERIALIZABLE
        float mEmitParticlesPerSecond = 10;   // Number of particles emitting in one second @SERIALIZABLE
                                                          
        float mInitialAngle = 0;          // Emitting particles angle in radians @SERIALIZABLE
        float mInitialAngleRange = 45.0f; // Emitting particles angle in radians randomize range @SERIALIZABLE
                                                        
        Vec2F mInitialSize = Vec2F(10, 10); // Emitting particles size @SERIALIZABLE
        Vec2F mInitialSizeRange;            // Emitting particles size randomize range @SERIALIZABLE
                                                        
        float mInitialSpeed = 10;      // Emitting particles speed @SERIALIZABLE
        float mInitialSpeedRangle = 5; // Emitting particles speed range @SERIALIZABLE
                                                        
        float mInitialMoveDirection = 0;          // Emitting particles direction in radians @SERIALIZABLE
        float mInitialMoveDirectionRange = 45.0f; // Emitting particles direction range in radians @SERIALIZABLE
              
        float mInitialAngleSpeed = 0;      // Emitting particles angle speed in radians/sec
        float mInitialAngleSpeedRange = 0; // Emitting particles angle speed range in radians/sec

		float            mEmitTimeBuffer = 0;    // Emitting next particle time buffer
		Vector<Particle> mParticles;             // Working particles
		Vector<int>      mDeadParticles;         // Dead particles indexes
		int              mNumAliveParticles = 0; // Count of current alive particles
		Basis            mLastTransform;         // Last transformation

	protected:
        // Beginning serialization callback
        void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback, initializes particles container
        void OnDeserialized(const DataValue& node) override;

		// Checks is particles container initialized
		void CreateParticlesContainer();

        // Emits particles hen updating
        void UpdateEmitting(float dt);

        // Applies and updates particles effects
        void UpdateEffects(float dt);

        // Updates particles
        void UpdateParticles(float dt);
        
        // Called when basis was changed, updates particles positions from last transform
        void BasisChanged() override;

        friend class ParticlesEffect;
    };

    template<typename _type, typename ... _args>
    void ParticlesEmitter::AddEffect(_args ... args)
    {
        AddEffect(mmake<_type>(args ...));
    }
}
// --- META ---

CLASS_BASES_META(o2::ParticlesEmitter)
{
    BASE_CLASS(o2::IRectDrawable);
    BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitter)
{
    FIELD().PUBLIC().NAME(playing);
    FIELD().PUBLIC().NAME(looped);
    FIELD().PUBLIC().RANGE_ATTRIBUTE(0, 10).NAME(duration);
    FIELD().PUBLIC().NAME(particlesRelative);
    FIELD().PUBLIC().DEFAULT_TYPE_ATTRIBUTE(o2::CircleParticlesEmitterShape).DONT_DELETE_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().NAME(shape);
    FIELD().PUBLIC().DEFAULT_TYPE_ATTRIBUTE(o2::SingleSpriteParticleSource).DONT_DELETE_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().NAME(particlesSource);
    FIELD().PUBLIC().NAME(maxParticles);
    FIELD().PUBLIC().RANGE_ATTRIBUTE(0, 10).NAME(particlesLifetime);
    FIELD().PUBLIC().NAME(particlesPerSecond);
    FIELD().PUBLIC().RANGE_ATTRIBUTE(0, 360).NAME(initialAngle);
    FIELD().PUBLIC().RANGE_ATTRIBUTE(0, 360).NAME(initialAngleRange);
    FIELD().PUBLIC().NAME(initialSize);
    FIELD().PUBLIC().NAME(initialSizeRange);
    FIELD().PUBLIC().NAME(initialSpeed);
    FIELD().PUBLIC().NAME(initialSpeedRange);
    FIELD().PUBLIC().NAME(initialAngleSpeed);
    FIELD().PUBLIC().NAME(initialAngleSpeedRange);
    FIELD().PUBLIC().NAME(moveDirection);
    FIELD().PUBLIC().NAME(moveDirectionRange);
    FIELD().PUBLIC().RANGE_ATTRIBUTE(0, 1).NAME(emittingCoefficient);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(mmake<SingleSpriteParticleSource>()).NAME(mParticlesSource);
    FIELD().PROTECTED().NAME(mParticlesContainer);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mShape);
    FIELD().PROTECTED().DONT_DELETE_ATTRIBUTE().EDITOR_PROPERTY_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mEffects);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(100).NAME(mParticlesNumLimit);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mEmittingCoefficient);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mIsParticlesRelative);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mIsLooped);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1).NAME(mDuration);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.5f).NAME(mParticlesLifetime);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mParticlesLifetimeRange);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(10).NAME(mEmitParticlesPerSecond);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(mInitialAngle);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(45.0f).NAME(mInitialAngleRange);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(10, 10)).NAME(mInitialSize);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mInitialSizeRange);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(10).NAME(mInitialSpeed);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(5).NAME(mInitialSpeedRangle);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(mInitialMoveDirection);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(45.0f).NAME(mInitialMoveDirectionRange);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mInitialAngleSpeed);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mInitialAngleSpeedRange);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mEmitTimeBuffer);
    FIELD().PROTECTED().NAME(mParticles);
    FIELD().PROTECTED().NAME(mDeadParticles);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mNumAliveParticles);
    FIELD().PROTECTED().NAME(mLastTransform);
}
END_META;
CLASS_METHODS_META(o2::ParticlesEmitter)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const ParticlesEmitter&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, Play);
    FUNCTION().PUBLIC().SIGNATURE(void, Stop);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParticlesSource, const Ref<ParticleSource>&);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<ParticleSource>&, GetParticlesSource);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEmittingCoef, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetEmittingCoef);
    FUNCTION().PUBLIC().SIGNATURE(void, SetShape, const Ref<ParticlesEmitterShape>&);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<ParticlesEmitterShape>&, GetShape);
    FUNCTION().PUBLIC().SIGNATURE(void, AddEffect, const Ref<ParticlesEffect>&);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<ParticlesEffect>>&, GetEffects);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveEffect, const Ref<ParticlesEffect>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllEffects);
    FUNCTION().PUBLIC().SIGNATURE(void, SetMaxParticles, int);
    FUNCTION().PUBLIC().SIGNATURE(int, GetMaxParticles);
    FUNCTION().PUBLIC().SIGNATURE(int, GetParticlesCount);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsAliveParticles);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Particle>&, GetParticles);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParticlesRelativity, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsParticlesRelative);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLoop, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsLooped);
    FUNCTION().PUBLIC().SIGNATURE(void, SetDuration, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetDuration);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParticlesLifetime, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetParticlesLifetime);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParticlesPerSecond, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetParticlesPerSecond);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialAngle, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetInitialAngle);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialAngleRange, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetInitialAngleRange);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialSize, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetInitialSize);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialSizeRange, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetInitialSizeRange);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialAngleSpeed, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetInitialAngleSpeed);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialAngleSpeedRange, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetInitialAngleSpeedRange);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialSpeed, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetInitialSpeed);
    FUNCTION().PUBLIC().SIGNATURE(void, SetInitialSpeedRange, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetInitialSpeedRange);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesMoveDirection, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesMoveDirection);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesMoveDirectionRange, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesMoveDirectionRange);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, CreateParticlesContainer);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateEmitting, float);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateEffects, float);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateParticles, float);
    FUNCTION().PROTECTED().SIGNATURE(void, BasisChanged);
}
END_META;
// --- END META ---
