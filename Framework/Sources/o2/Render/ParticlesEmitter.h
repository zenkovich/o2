#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Particle.h"
#include "o2/Render/ParticlesEffects.h"
#include "o2/Render/ParticlesEmitterShapes.h"
#include "o2/Render/RectDrawable.h"
#include "o2/Utils/Math/Curve.h"

namespace o2
{
	class Mesh;

	// ------------------------------------------------------
	// Particles emitter. Emits, updates and manage particles
	// ------------------------------------------------------
	class ParticlesEmitter: public IRectDrawable
	{
		SERIALIZABLE(ParticlesEmitter);

	public:
		PROPERTIES(ParticlesEmitter);
		PROPERTY(bool, playing, SetPlaying, IsPlaying);                                 // Is particles playing property
		PROPERTY(float, emittingCoefficient, SetEmittingCoef, GetEmittingCoef);         // Particles emitting coefficient property (0...1)
		PROPERTY(bool, particlesRelative, SetParticlesRelativity, IsParticlesRelative); // Is particles relative to emitter
		PROPERTY(bool, looped, SetLoop, IsLooped);                                      // Is emitter looped property
		PROPERTY(int, maxParticles, SetMaxParticles, GetMaxParticles);                  // Number of maximum particles in emitter property
		PROPERTY(float, duration, SetDuration, GetDuration);                            // Working duration in seconds property

		PROPERTY(float, particlesLifetime, SetParticlesLifetime, GetParticlesLifetime);                   // Particles lifetime in seconds property
		PROPERTY(float, emitParticlesPerSecond, SetEmitParticlesPerSecond, GetEmitParticlesPerSecond);    // Amount of particles emitting in one second property
		PROPERTY(float, emitParticlesAngle, SetEmitParticlesAngle, GetEmitParticlesAngle);                // Emitting particle angle property in degrees
		PROPERTY(float, emitParticlesAngleRange, SetEmitParticlesAngleRange, GetEmitParticlesAngleRange); // Emitting particle angle range property in degrees
		PROPERTY(Vec2F, emitParticlesSize, SetEmitParticlesSize, GetEmitParticlesSize);                   // Emitting particle size property
		PROPERTY(Vec2F, emitParticlesSizeRange, SetEmitParticlesSizeRange, GetEmitParticlesSizeRange);    // Emitting particle size range property
		PROPERTY(float, emitParticlesSpeed, SetEmitParticlesSpeed, GetEmitParticlesSpeed);                // Emitting particle angle speed property in degrees/sec

		PROPERTY(float, emitParticlesAngleSpeedRange, SetEmitParticlesSpeedRange, GetEmitParticlesSpeedRange);              // Emitting particle angle speed range in degrees/sec
		PROPERTY(float, emitParticlesAngleSpeed, SetEmitParticlesAngleSpeed, GetEmitParticlesAngleSpeed);                   // Emitting particle speed property
		PROPERTY(float, emitParticlesSpeedRange, SetEmitParticlesAngleSpeedRange, GetEmitParticlesAngleSpeedRange);         // Emitting particle speed range
		PROPERTY(float, emitParticlesMoveDir, SetEmitParticlesMoveDirection, GetEmitParticlesMoveDirection);                // Emitting particle moving direction in degrees property
		PROPERTY(float, emitParticlesMoveDirRange, SetEmitParticlesMoveDirectionRange, GetEmitParticlesMoveDirectionRange); // Emitting particle moving direction range in degrees property

		PROPERTY(Color4, emitParticlesColorA, SetEmitParticlesColorA, GetEmitParticlesColorA); // Emitting particle color A property
		PROPERTY(Color4, emitParticlesColorB, SetEmitParticlesColorB, GetEmitParticlesColorB); // Emitting particle color B property													   

		PROPERTY(ImageAssetRef, image, SetImage, GetImage);          // Particle image property
		PROPERTY(ParticlesEmitterShape*, shape, SetShape, GetShape); // Emitting shape property @EDITOR_IGNORE

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

		// Set playing
		void SetPlaying(bool playing);

		// Returns is playing
		bool IsPlaying() const;

		// Starts playing
		void Play();

		// Stops playing
		void Stop();

		// Sets particles emitting coefficient (0...1)
		void SetEmittingCoef(float coef);

		// Returns particles emitting coefficient
		float GetEmittingCoef() const;

		// Sets particle sprite image
		void SetImage(const ImageAssetRef& image);

		// Returns particle sprite image
		ImageAssetRef GetImage() const;

		// Sets emitting shape
		void SetShape(ParticlesEmitterShape* shape);

		// Return emitting shape
		ParticlesEmitterShape* GetShape() const;

		// Adds effect
		void AddEffect(ParticlesEffect* effect);

		// Adds effect
		template<typename _type, typename ... _args>
		void AddEffect(_args ... args);

		// Returns list of effects
		const Vector<ParticlesEffect*>& GetEffects() const;

		// Removes effect
		void RemoveEffect(ParticlesEffect* effect);

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
		void SetEmitParticlesPerSecond(float numParticles);

		// Returns number of particles emitting per second
		float GetEmitParticlesPerSecond() const;

		// Sets emitting particles rotation angle in degrees
		void SetEmitParticlesAngle(float angle);

		// Returns emitting particles rotation angle in degrees
		float GetEmitParticlesAngle() const;

		// Sets emitting particles rotation angle range in degrees
		void SetEmitParticlesAngleRange(float range);

		// Returns emitting particles rotation angle range in degrees
		float GetEmitParticlesAngleRange() const;

		// Sets emitting particles size
		void SetEmitParticlesSize(const Vec2F& size);

		// Returns emitting particles size
		Vec2F GetEmitParticlesSize() const;

		// Sets emitting particles size range
		void SetEmitParticlesSizeRange(const Vec2F& range);

		// Returns emitting particles size range
		Vec2F GetEmitParticlesSizeRange() const;

		// Sets emitting particles angle speed in degrees/sec
		void SetEmitParticlesAngleSpeed(float speed);

		// Returns emitting particles angle speed in degrees/sec
		float GetEmitParticlesAngleSpeed() const;

		// Sets emitting particles angle speed range in degrees/sec
		void SetEmitParticlesAngleSpeedRange(float speedRange);

		// Returns emitting particles angle speed range in degrees/sec
		float GetEmitParticlesAngleSpeedRange() const;

		// Sets emitting particles speed
		void SetEmitParticlesSpeed(float speed);

		// Returns emitting particles speed
		float GetEmitParticlesSpeed() const;

		// Sets emitting particles speed range
		void SetEmitParticlesSpeedRange(float speedRange);

		// Returns emitting particles speed range
		float GetEmitParticlesSpeedRange() const;

		// Sets emitting particles moving direction angle in degrees
		void SetEmitParticlesMoveDirection(float direction);

		// Returns emitting particles moving direction angle in degrees
		float GetEmitParticlesMoveDirection() const;

		// Sets emitting particles moving direction angle range in degrees
		void SetEmitParticlesMoveDirectionRange(float directionRange);

		// Returns emitting particles moving direction angle range in degrees
		float GetEmitParticlesMoveDirectionRange() const;

		// Sets emitting particles color A
		void SetEmitParticlesColorA(const Color4& color);

		// Returns emitting particles color A
		Color4 GetEmitParticlesColorA() const;

		// Sets emitting particles color B
		void SetEmitParticlesColorB(const Color4& color);

		// Returns emitting particles color B
		Color4 GetEmitParticlesColorB() const;

		// Sets emitting color A and B to color
		void SetEmitParticlesColor(const Color4& color);

		// Sets emitting color A and B
		void SetEmitParticlesColor(const Color4& colorA, const Color4& colorB);

	protected:
		ImageAssetRef          mImageAsset;      // Particle sprite image @SERIALIZABLE
		ParticlesEmitterShape* mShape = nullptr; // Particles emitting shape @SERIALIZABLE @EDITOR_PROPERTY 
		Vector<ParticlesEffect*>     mEffects;         // Particles effect @SERIALIZABLE @EDITOR_PROPERTY 
														                 
		int mParticlesNumLimit = 100; // Max available visible particles @SERIALIZABLE
					           		
		bool  mPlaying = true;             // Is playing @SERIALIZABLE
		float mEmittingCoefficient = 1.0f; // Emitting particles number coefficient (0...1) @SERIALIZABLE
		bool  mIsParticlesRelative = true; // Is particles relative to emitter or global @SERIALIZABLE
		bool  mIsLooped = false;           // Is emitter looped @SERIALIZABLE
					 							                 
		float mDuration = 1;                            // Duration of working time @SERIALIZABLE
					 							                 
		float mParticlesLifetime = 0.5f;    // Particles lifetime in seconds @SERIALIZABLE
		float mEmitParticlesPerSecond = 10; // Number of particles emitting in one second @SERIALIZABLE
			 							                 
		float mEmitParticlesAngle = 0;          // Emitting particles angle in radians @SERIALIZABLE
		float mEmitParticlesAngleRange = 45.0f; // Emitting particles angle in radians randomize range @SERIALIZABLE
			 							               
		Vec2F mEmitParticlesSize = Vec2F(10, 10); // Emitting particles size @SERIALIZABLE
		Vec2F mEmitParticlesSizeRange;            // Emitting particles size randomize range @SERIALIZABLE
			 						                   
		float mEmitParticlesSpeed = 10;      // Emitting particles speed @SERIALIZABLE
		float mEmitParticlesSpeedRangle = 5; // Emitting particles speed range @SERIALIZABLE
			 								           
		float mEmitParticlesMoveDirection = 0;          // Emitting particles direction in radians @SERIALIZABLE
		float mEmitParticlesMoveDirectionRange = 45.0f; // Emitting particles direction range in radians @SERIALIZABLE
			  
		float mEmitParticlesAngleSpeed = 0;      // Emitting particles angle speed in radians/sec
		float mEmitParticlesAngleSpeedRange = 0; // Emitting particles angle speed range in radians/sec
					 
		Color4 mEmitParticlesColorA; // Emitting particles color A (particle emitting with color in range from this and ColorB)  @SERIALIZABLE
		Color4 mEmitParticlesColorB; // Emitting particles color B (particle emitting with color in range from this and ColorA) @SERIALIZABLE

		float            mCurrentTime = 0;         // Current working time in seconds
		float            mEmitTimeBuffer = 0;      // Emitting next particle time buffer
		Mesh*            mParticlesMesh = nullptr; // Particles mesh
		Vector<Particle> mParticles;               // Working particles
		Vector<int>      mDeadParticles;           // Dead particles indexes
		int              mNumAliveParticles = 0;   // Count of current alive particles
		Basis            mLastTransform;           // Last transformation

	protected:
		// Emits particles hen updating
		void UpdateEmitting(float dt);

		// Applies and updates particles effects
		void UpdateEffects(float dt);

		// Updates particles
		void UpdateParticles(float dt);

		// Updates mesh geometry
		void UpdateMesh(); 
		
		// Called when basis was changed, updates particles positions from last transform
		void BasisChanged() override;

		friend class ParticlesEffect;
	};

	template<typename _type, typename ... _args>
	void ParticlesEmitter::AddEffect(_args ... args)
	{
		AddEffect(mnew _type(args ...));
	}
}
// --- META ---

CLASS_BASES_META(o2::ParticlesEmitter)
{
	BASE_CLASS(o2::IRectDrawable);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitter)
{
	FIELD().PUBLIC().NAME(playing);
	FIELD().PUBLIC().NAME(emittingCoefficient);
	FIELD().PUBLIC().NAME(particlesRelative);
	FIELD().PUBLIC().NAME(looped);
	FIELD().PUBLIC().NAME(maxParticles);
	FIELD().PUBLIC().NAME(duration);
	FIELD().PUBLIC().NAME(particlesLifetime);
	FIELD().PUBLIC().NAME(emitParticlesPerSecond);
	FIELD().PUBLIC().NAME(emitParticlesAngle);
	FIELD().PUBLIC().NAME(emitParticlesAngleRange);
	FIELD().PUBLIC().NAME(emitParticlesSize);
	FIELD().PUBLIC().NAME(emitParticlesSizeRange);
	FIELD().PUBLIC().NAME(emitParticlesSpeed);
	FIELD().PUBLIC().NAME(emitParticlesAngleSpeedRange);
	FIELD().PUBLIC().NAME(emitParticlesAngleSpeed);
	FIELD().PUBLIC().NAME(emitParticlesSpeedRange);
	FIELD().PUBLIC().NAME(emitParticlesMoveDir);
	FIELD().PUBLIC().NAME(emitParticlesMoveDirRange);
	FIELD().PUBLIC().NAME(emitParticlesColorA);
	FIELD().PUBLIC().NAME(emitParticlesColorB);
	FIELD().PUBLIC().NAME(image);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(shape);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mImageAsset);
	FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mShape);
	FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mEffects);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(100).NAME(mParticlesNumLimit);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mPlaying);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mEmittingCoefficient);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mIsParticlesRelative);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mIsLooped);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1).NAME(mDuration);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.5f).NAME(mParticlesLifetime);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(10).NAME(mEmitParticlesPerSecond);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(mEmitParticlesAngle);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(45.0f).NAME(mEmitParticlesAngleRange);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2F(10, 10)).NAME(mEmitParticlesSize);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mEmitParticlesSizeRange);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(10).NAME(mEmitParticlesSpeed);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(5).NAME(mEmitParticlesSpeedRangle);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(mEmitParticlesMoveDirection);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(45.0f).NAME(mEmitParticlesMoveDirectionRange);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mEmitParticlesAngleSpeed);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mEmitParticlesAngleSpeedRange);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mEmitParticlesColorA);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mEmitParticlesColorB);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mCurrentTime);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mEmitTimeBuffer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mParticlesMesh);
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
	FUNCTION().PUBLIC().SIGNATURE(void, SetPlaying, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsPlaying);
	FUNCTION().PUBLIC().SIGNATURE(void, Play);
	FUNCTION().PUBLIC().SIGNATURE(void, Stop);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmittingCoef, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmittingCoef);
	FUNCTION().PUBLIC().SIGNATURE(void, SetImage, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(ImageAssetRef, GetImage);
	FUNCTION().PUBLIC().SIGNATURE(void, SetShape, ParticlesEmitterShape*);
	FUNCTION().PUBLIC().SIGNATURE(ParticlesEmitterShape*, GetShape);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEffect, ParticlesEffect*);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<ParticlesEffect*>&, GetEffects);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveEffect, ParticlesEffect*);
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
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesPerSecond, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesPerSecond);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesAngle, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesAngle);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesAngleRange, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesAngleRange);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesSize, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetEmitParticlesSize);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesSizeRange, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetEmitParticlesSizeRange);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesAngleSpeed, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesAngleSpeed);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesAngleSpeedRange, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesAngleSpeedRange);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesSpeed, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesSpeed);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesSpeedRange, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesSpeedRange);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesMoveDirection, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesMoveDirection);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesMoveDirectionRange, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetEmitParticlesMoveDirectionRange);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesColorA, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(Color4, GetEmitParticlesColorA);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesColorB, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(Color4, GetEmitParticlesColorB);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesColor, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEmitParticlesColor, const Color4&, const Color4&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateEmitting, float);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateEffects, float);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateParticles, float);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateMesh);
	FUNCTION().PROTECTED().SIGNATURE(void, BasisChanged);
}
END_META;
// --- END META ---
