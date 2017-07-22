#pragma once

#include "Assets/ImageAsset.h"
#include "Render/Particle.h"
#include "Render/RectDrawable.h"
#include "Utils/Math/Curve.h"

namespace o2
{
	class Mesh;
	class ParticlesEffect;
	class ParticlesEmitterShape;

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
		Property<bool>   playing;                      // Is particles playing property
		Property<float>  emittingCoefficient;          // Particles emitting coefficient property (0...1)
		Property<bool>   particlesRelative;            // Is particles relative to emitter
		Property<bool>   looped;                       // Is emitter looped property
		Property<int>    maxParticles;                 // Number of maximum particles in emitter property
		Property<float>  duration;                     // Working duration in seconds property
		Property<float>  particlesLifetime;            // Particles lifetime in seconds property
		Property<float>  emitParticlesPerSecond;       // Amount of particles emitting in one second property
		Property<float>  emitParticlesAngle;           // Emitting particle angle property in degrees
		Property<float>  emitParticlesAngleRange;      // Emitting particle angle range property in degrees
		Property<Vec2F>  emitParticlesSize;            // Emitting particle size property
		Property<Vec2F>  emitParticlesSizeRange;       // Emitting particle size range property
		Property<float>  emitParticlesSpeed;           // Emitting particle angle speed property in degrees/sec
		Property<float>  emitParticlesAngleSpeedRange; // Emitting particle angle speed range in degrees/sec
		Property<float>  emitParticlesAngleSpeed;      // Emitting particle speed property
		Property<float>  emitParticlesSpeedRange;      // Emitting particle speed range
		Property<float>  emitParticlesMoveDir;         // Emitting particle moving direction in degrees property
		Property<float>  emitParticlesMoveDirRange;    // Emitting particle moving direction range in degrees property
		Property<Color4> emitParticlesColorA;          // Emitting particle color A property
		Property<Color4> emitParticlesColorB;          // Emitting particle color B property
													   
		Property<ImageAssetRef>          image;        // Particle image property
		Property<ParticlesEmitterShape*> shape;        // Emitting shape property

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
		ImageAssetRef          mImageAsset;                    // Particle sprite image @SERIALIZABLE
		ParticlesEmitterShape* mShape = nullptr;               // Particles emitting shape @SERIALIZABLE
		ParticleEffectsVec     mEffects;                       // Particles effect @SERIALIZABLE
														                 
		int          mParticlesNumLimit = 100;                 // Max available visible particles @SERIALIZABLE
					           		
		bool         mPlaying = true;                          // Is playing @SERIALIZABLE
		float        mEmittingCoefficient = 1.0f;              // Emitting particles number coefficient (0...1) @SERIALIZABLE
		bool         mIsParticlesRelative = true;              // Is particles relative to emitter or global @SERIALIZABLE
		bool         mIsLooped = false;                        // Is emitter looped @SERIALIZABLE
					 							                 
		float        mDuration = 1;                            // Duration of working time @SERIALIZABLE
					 							                 
		float        mParticlesLifetime = 0.5f;                // Particles lifetime in seconds @SERIALIZABLE
		float        mEmitParticlesPerSecond = 10;             // Number of particles emitting in one second @SERIALIZABLE
					 							                 
		float        mEmitParticlesAngle = 0;                  // Emitting particles angle in radians @SERIALIZABLE
		float        mEmitParticlesAngleRange = 45.0f;         // Emitting particles angle in radians randomize range @SERIALIZABLE
					 							               
		Vec2F        mEmitParticlesSize = Vec2F(10, 10);       // Emitting particles size @SERIALIZABLE
		Vec2F        mEmitParticlesSizeRange;                  // Emitting particles size randomize range @SERIALIZABLE
					 						                   
		float        mEmitParticlesSpeed = 10;                 // Emitting particles speed @SERIALIZABLE
		float        mEmitParticlesSpeedRangle = 5;            // Emitting particles speed range @SERIALIZABLE
					 								           
		float        mEmitParticlesMoveDirection = 0;          // Emitting particles direction in radians @SERIALIZABLE
		float        mEmitParticlesMoveDirectionRange = 45.0f; // Emitting particles direction range in radians @SERIALIZABLE

		float        mEmitParticlesAngleSpeed = 0;             // Emitting particles angle speed in radians/sec
		float        mEmitParticlesAngleSpeedRange = 0;        // Emitting particles angle speed range in radians/sec
					 
		Color4       mEmitParticlesColorA;                     // Emitting particles color A (particle emitting with color in range from this and ColorB)  @SERIALIZABLE
		Color4       mEmitParticlesColorB;                     // Emitting particles color B (particle emitting with color in range from this and ColorA) @SERIALIZABLE

		float        mCurrentTime = 0;                         // Current working time in seconds
		float        mEmitTimeBuffer = 0;                      // Emitting next particle time buffer
		Mesh*        mParticlesMesh = nullptr;                 // Particles mesh
		ParticlesVec mParticles;                               // Working particles
		Vector<int>  mDeadParticles;                           // Dead particles indexes
		int          mNumAliveParticles = 0;                   // Count of current alive particles
		Basis        mLastTransform;                           // Last transformation

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

		// Initialize properties
		void InitializeProperties();

		friend class ParticlesEffect;
	};

	template<typename _type, typename ... _args>
	void ParticlesEmitter::AddEffect(_args ... args)
	{
		AddEffect(mnew _type(args ...));
	}
}
