#pragma once

#include "Assets/ImageAsset.h"
#include "Render/Particle.h"
#include "Render/ParticlesEmitterShapes.h"
#include "Render/RectDrawable.h"
#include "Utils/Math/Curve.h"

namespace o2
{
	class Mesh;
	class ParticlesEffect;

	// ------------------------------------------------------
	// Particles emitter. Emits, updates and manage particles
	// ------------------------------------------------------
	class ParticlesEmitter: public IRectDrawable
	{
		SERIALIZABLE(ParticlesEmitter);

	public:
		typedef Vector<Particle> ParticlesVec;
		typedef Vector<ParticlesEffect*> ParticleEffectsVec;

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
		PROPERTY(ParticlesEmitterShape*, shape, SetShape, GetShape); // Emitting shape property

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
		void Draw();

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
		const ParticleEffectsVec& GetEffects() const;

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
		const ParticlesVec& GetParticles() const;

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
		ParticlesEmitterShape* mShape = nullptr; // Particles emitting shape @SERIALIZABLE
		ParticleEffectsVec     mEffects;         // Particles effect @SERIALIZABLE @EDITOR_PROPERTY 
														                 
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

		float        mCurrentTime = 0;         // Current working time in seconds
		float        mEmitTimeBuffer = 0;      // Emitting next particle time buffer
		Mesh*        mParticlesMesh = nullptr; // Particles mesh
		ParticlesVec mParticles;               // Working particles
		Vector<int>  mDeadParticles;           // Dead particles indexes
		int          mNumAliveParticles = 0;   // Count of current alive particles
		Basis        mLastTransform;           // Last transformation

	protected:
		// Emits particles hen updating
		void UpdateEmitting(float dt);

		// Applies and updates particles effects
		void UpdateEffects(float dt);

		// Updates particles
		void UpdateParticles(float dt);

		// Updates mesh geometry
		void UpdateMesh(); 
		
		// It is called when basis was changed, updates particles positions from last transform
		void BasisChanged();

		friend class ParticlesEffect;
	};

	template<typename _type, typename ... _args>
	void ParticlesEmitter::AddEffect(_args ... args)
	{
		AddEffect(mnew _type(args ...));
	}
}

CLASS_BASES_META(o2::ParticlesEmitter)
{
	BASE_CLASS(o2::IRectDrawable);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitter)
{
	PUBLIC_FIELD(playing);
	PUBLIC_FIELD(emittingCoefficient);
	PUBLIC_FIELD(particlesRelative);
	PUBLIC_FIELD(looped);
	PUBLIC_FIELD(maxParticles);
	PUBLIC_FIELD(duration);
	PUBLIC_FIELD(particlesLifetime);
	PUBLIC_FIELD(emitParticlesPerSecond);
	PUBLIC_FIELD(emitParticlesAngle);
	PUBLIC_FIELD(emitParticlesAngleRange);
	PUBLIC_FIELD(emitParticlesSize);
	PUBLIC_FIELD(emitParticlesSizeRange);
	PUBLIC_FIELD(emitParticlesSpeed);
	PUBLIC_FIELD(emitParticlesAngleSpeedRange);
	PUBLIC_FIELD(emitParticlesAngleSpeed);
	PUBLIC_FIELD(emitParticlesSpeedRange);
	PUBLIC_FIELD(emitParticlesMoveDir);
	PUBLIC_FIELD(emitParticlesMoveDirRange);
	PUBLIC_FIELD(emitParticlesColorA);
	PUBLIC_FIELD(emitParticlesColorB);
	PUBLIC_FIELD(image);
	PUBLIC_FIELD(shape);
	PROTECTED_FIELD(mImageAsset).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mShape).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEffects).EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParticlesNumLimit).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPlaying).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmittingCoefficient).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsParticlesRelative).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsLooped).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDuration).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParticlesLifetime).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesPerSecond).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesAngle).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesAngleRange).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesSize).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesSizeRange).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesSpeed).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesSpeedRangle).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesMoveDirection).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesMoveDirectionRange).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesAngleSpeed);
	PROTECTED_FIELD(mEmitParticlesAngleSpeedRange);
	PROTECTED_FIELD(mEmitParticlesColorA).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEmitParticlesColorB).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurrentTime);
	PROTECTED_FIELD(mEmitTimeBuffer);
	PROTECTED_FIELD(mParticlesMesh);
	PROTECTED_FIELD(mParticles);
	PROTECTED_FIELD(mDeadParticles);
	PROTECTED_FIELD(mNumAliveParticles);
	PROTECTED_FIELD(mLastTransform);
}
END_META;
CLASS_METHODS_META(o2::ParticlesEmitter)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetPlaying, bool);
	PUBLIC_FUNCTION(bool, IsPlaying);
	PUBLIC_FUNCTION(void, Play);
	PUBLIC_FUNCTION(void, Stop);
	PUBLIC_FUNCTION(void, SetEmittingCoef, float);
	PUBLIC_FUNCTION(float, GetEmittingCoef);
	PUBLIC_FUNCTION(void, SetImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(ImageAssetRef, GetImage);
	PUBLIC_FUNCTION(void, SetShape, ParticlesEmitterShape*);
	PUBLIC_FUNCTION(ParticlesEmitterShape*, GetShape);
	PUBLIC_FUNCTION(void, AddEffect, ParticlesEffect*);
	PUBLIC_FUNCTION(const ParticleEffectsVec&, GetEffects);
	PUBLIC_FUNCTION(void, RemoveEffect, ParticlesEffect*);
	PUBLIC_FUNCTION(void, RemoveAllEffects);
	PUBLIC_FUNCTION(void, SetMaxParticles, int);
	PUBLIC_FUNCTION(int, GetMaxParticles);
	PUBLIC_FUNCTION(int, GetParticlesCount);
	PUBLIC_FUNCTION(bool, IsAliveParticles);
	PUBLIC_FUNCTION(const ParticlesVec&, GetParticles);
	PUBLIC_FUNCTION(void, SetParticlesRelativity, bool);
	PUBLIC_FUNCTION(bool, IsParticlesRelative);
	PUBLIC_FUNCTION(void, SetLoop, bool);
	PUBLIC_FUNCTION(bool, IsLooped);
	PUBLIC_FUNCTION(void, SetDuration, float);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(void, SetParticlesLifetime, float);
	PUBLIC_FUNCTION(float, GetParticlesLifetime);
	PUBLIC_FUNCTION(void, SetEmitParticlesPerSecond, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesPerSecond);
	PUBLIC_FUNCTION(void, SetEmitParticlesAngle, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesAngle);
	PUBLIC_FUNCTION(void, SetEmitParticlesAngleRange, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesAngleRange);
	PUBLIC_FUNCTION(void, SetEmitParticlesSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetEmitParticlesSize);
	PUBLIC_FUNCTION(void, SetEmitParticlesSizeRange, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetEmitParticlesSizeRange);
	PUBLIC_FUNCTION(void, SetEmitParticlesAngleSpeed, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesAngleSpeed);
	PUBLIC_FUNCTION(void, SetEmitParticlesAngleSpeedRange, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesAngleSpeedRange);
	PUBLIC_FUNCTION(void, SetEmitParticlesSpeed, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesSpeed);
	PUBLIC_FUNCTION(void, SetEmitParticlesSpeedRange, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesSpeedRange);
	PUBLIC_FUNCTION(void, SetEmitParticlesMoveDirection, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesMoveDirection);
	PUBLIC_FUNCTION(void, SetEmitParticlesMoveDirectionRange, float);
	PUBLIC_FUNCTION(float, GetEmitParticlesMoveDirectionRange);
	PUBLIC_FUNCTION(void, SetEmitParticlesColorA, const Color4&);
	PUBLIC_FUNCTION(Color4, GetEmitParticlesColorA);
	PUBLIC_FUNCTION(void, SetEmitParticlesColorB, const Color4&);
	PUBLIC_FUNCTION(Color4, GetEmitParticlesColorB);
	PUBLIC_FUNCTION(void, SetEmitParticlesColor, const Color4&);
	PUBLIC_FUNCTION(void, SetEmitParticlesColor, const Color4&, const Color4&);
	PROTECTED_FUNCTION(void, UpdateEmitting, float);
	PROTECTED_FUNCTION(void, UpdateEffects, float);
	PROTECTED_FUNCTION(void, UpdateParticles, float);
	PROTECTED_FUNCTION(void, UpdateMesh);
	PROTECTED_FUNCTION(void, BasisChanged);
}
END_META;
