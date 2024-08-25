#include "o2/stdafx.h"
#include "ParticlesEmitter.h"

#include "o2/Render/Mesh.h"
#include "o2/Render/Particles/ParticlesEffects.h"
#include "o2/Render/Particles/ParticlesEmitterShapes.h"

namespace o2
{
	ParticlesEmitter::ParticlesEmitter() :
		IRectDrawable(), mShape(mmake<CircleParticlesEmitterShape>())
	{
		mLastTransform = mTransform;
	}

	ParticlesEmitter::~ParticlesEmitter()
	{
	}

	ParticlesEmitter::ParticlesEmitter(const ParticlesEmitter& other) :
		IRectDrawable(other), mParticlesSource(other.mParticlesSource->CloneAsRef<ParticleSource>()),
		mShape(other.mShape->CloneAsRef<ParticlesEmitterShape>()),
		mParticlesNumLimit(other.mParticlesNumLimit), mPlaying(other.mPlaying),
		mEmittingCoefficient(other.mEmittingCoefficient), mIsLooped(other.mIsLooped),
		mIsParticlesRelative(other.mIsParticlesRelative), mDuration(other.mDuration),
		mParticlesLifetime(other.mParticlesLifetime), mEmitParticlesPerSecond(other.mEmitParticlesPerSecond),
		mInitialAngle(other.mInitialAngle), mInitialAngleRange(other.mInitialAngleRange),
		mInitialSize(other.mInitialSize), mInitialSizeRange(other.mInitialSizeRange),
		mInitialSpeed(other.mInitialSpeed), mInitialSpeedRangle(other.mInitialSpeedRangle),
		mInitialMoveDirection(other.mInitialMoveDirection), mInitialMoveDirectionRange(other.mInitialMoveDirectionRange),
		playing(this), emittingCoefficient(this), particlesRelative(this), looped(this), maxParticles(this),
		duration(this), particlesLifetime(this), particlesPerSecond(this), initialAngle(this), initialAngleRange(this),
		initialSize(this), initialSizeRange(this), initialSpeed(this), initialSpeedRange(this), initialAngleSpeed(this),
		initialAngleSpeedRange(this), moveDirection(this), moveDirectionRange(this),
		particlesSource(this), shape(this)
	{
		for (auto& effect : other.mEffects)
			AddEffect(effect->CloneAsRef<ParticlesEffect>());

		mLastTransform = mTransform;
	}

	ParticlesEmitter& ParticlesEmitter::operator=(const ParticlesEmitter& other)
	{
		RemoveAllEffects();
		mShape = nullptr;

		int idx = 0;
		for (auto& particle : mParticles)
		{
			if (particle.alive)
			{
				mDeadParticles.Add(idx);
				particle.alive = false;
			}

			idx++;
		}
		mNumAliveParticles = 0;

		IRectDrawable::operator=(other);

		mParticlesSource = other.mParticlesSource;
		CreateParticlesContainer();

		mShape = other.mShape->CloneAsRef<ParticlesEmitterShape>();

		for (auto& effect : other.mEffects)
			AddEffect(effect->CloneAsRef<ParticlesEffect>());

		mParticlesNumLimit = other.mParticlesNumLimit;

		mPlaying = other.mPlaying;
		mEmittingCoefficient = other.mEmittingCoefficient;
		mIsParticlesRelative = other.mIsParticlesRelative;
		mIsLooped = other.mIsLooped;

		mDuration = other.mDuration;

		mParticlesLifetime = other.mParticlesLifetime;
		mEmitParticlesPerSecond = other.mEmitParticlesPerSecond;

		mInitialAngle = other.mInitialAngle;
		mInitialAngleRange = other.mInitialAngleRange;

		mInitialSize = other.mInitialSize;
		mInitialSizeRange = other.mInitialSizeRange;

		mInitialSpeed = other.mInitialSpeed;
		mInitialSpeedRangle = other.mInitialSpeedRangle;

		mInitialMoveDirection = other.mInitialMoveDirection;
		mInitialMoveDirectionRange = other.mInitialMoveDirectionRange;

		mLastTransform = mTransform;

		return *this;
	}

	void ParticlesEmitter::Draw()
	{
		if (mParticlesContainer)
			mParticlesContainer->Draw();
	}

	void ParticlesEmitter::Update(float dt)
	{
		if (!mEnabled)
			return;

		if (mPlaying)
		{
			mCurrentTime += dt;

			if (!mIsLooped && mCurrentTime > mDuration)
			{
				mPlaying = false;
				mCurrentTime = mDuration;
			}
		}

		UpdateEmitting(dt);
		UpdateEffects(dt);
		UpdateParticles(dt);

		if (!mParticlesContainer)
			CreateParticlesContainer();

		mParticlesContainer->Update(mParticles, mParticlesNumLimit);
	}

	void ParticlesEmitter::OnDeserialized(const DataValue& node)
	{
		CreateParticlesContainer();
	}

	void ParticlesEmitter::CreateParticlesContainer()
	{
		mParticlesContainer = mParticlesSource->CreateContainer();
		mParticlesContainer->emitter = this;
	}

	void ParticlesEmitter::UpdateEmitting(float dt)
	{
		if (!mPlaying)
			return;

		mEmitTimeBuffer += dt;

		float currentParticlesPerSecond = mEmitParticlesPerSecond*mEmittingCoefficient;
		float particlesDelay = 1.0f/mEmitParticlesPerSecond;

		float halfAngleRange = Math::Deg2rad(mInitialAngleRange)*0.5f;
		Vec2F halfSizeRange = mInitialSizeRange*0.5f;
		float halfSpeedRange = mInitialSpeedRangle*0.5f;
		float halfDirRange = mInitialMoveDirectionRange*0.5f;
		float halfAngleSpeedRange = Math::Deg2rad(mInitialAngleSpeedRange)*0.5f;
		float halfLifetimeRange = mParticlesLifetimeRange * 0.5f;

		while (mEmitTimeBuffer > particlesDelay)
		{
			if (mNumAliveParticles < mParticlesNumLimit)
			{
				Particle* particle;
				int particleIndex;

				// Allocate particle
				if (mDeadParticles.IsEmpty())
				{
					mParticles.Add(Particle());
					particle = &mParticles.Last();
					particleIndex = mParticles.Count() - 1;
				}
				else
				{
					particleIndex = mDeadParticles.PopBack();
					particle = &mParticles[particleIndex];
				}

				// Initialize particle
				particle->index = particleIndex;

				particle->position = Local2WorldPoint(mShape->GetEmittinPoint());
				particle->angle = mInitialAngle + Math::Random(-halfAngleRange, halfAngleRange);

				particle->size.Set(mInitialSize.x + Math::Random(-halfSizeRange.x, halfSizeRange.x),
								   mInitialSize.y + Math::Random(-halfSizeRange.y, halfSizeRange.y));

				particle->velocity = Vec2F::Rotated(mInitialMoveDirection + Math::Random(-halfDirRange, halfDirRange))*
					(mInitialSpeed + Math::Random(-halfSpeedRange, halfSpeedRange));

				particle->angleSpeed = mInitialAngleSpeed + Math::Random(-halfAngleSpeedRange, halfAngleSpeedRange);

				particle->color = Color4::White();

				particle->timeLeft = mParticlesLifetime + Math::Random(-halfLifetimeRange, halfLifetimeRange);
				particle->lifetime = particle->timeLeft;

				particle->alive = true;

				// Notify container and effects
				mParticlesContainer->OnParticleEmitted(*particle);

				for (auto& effect : mEffects)
				{
					if (effect)
						effect->OnParticleEmitted(*particle);
				}

				mNumAliveParticles++;
			}

			mEmitTimeBuffer -= particlesDelay;
		}
	}

	void ParticlesEmitter::UpdateEffects(float dt)
	{
		for (auto& effect : mEffects)
		{
			if (effect)
				effect->Update(dt, this);
		}
	}

	void ParticlesEmitter::UpdateParticles(float dt)
	{
		int idx = 0;
		for (auto& particle : mParticles)
		{
			if (!particle.alive)
			{
				idx++;
				continue;
			}

			particle.position += particle.velocity*dt;
			particle.angle += particle.angleSpeed*dt;

			particle.timeLeft -= dt;

			if (particle.timeLeft < 0)
			{
				particle.alive = false;

				mParticlesContainer->OnParticleDied(particle);

				for (auto& effect : mEffects)
				{
					if (effect)
						effect->OnParticleDied(particle);
				}

				mDeadParticles.Add(idx);
				mNumAliveParticles--;
			}

			idx++;
		}
	}

	void ParticlesEmitter::BasisChanged()
	{
		if (!mIsParticlesRelative)
			return;

		Basis change = mLastTransform.Inverted()*mTransform;
		for (auto& particle : mParticles)
			particle.position = change.Transform(particle.position);

		mLastTransform = mTransform;
	}

	void ParticlesEmitter::SetPlaying(bool playing)
	{
		mPlaying = playing;
	}

	bool ParticlesEmitter::IsPlaying() const
	{
		return mPlaying;
	}

	void ParticlesEmitter::Play()
	{
		mPlaying = true;

		mEmitTimeBuffer = 0.0f;
	}

	void ParticlesEmitter::Stop()
	{
		mPlaying = false;
	}

	void ParticlesEmitter::SetParticlesSource(const Ref<ParticleSource>& source)
	{
		mParticlesSource = source;
		CreateParticlesContainer();
	}

	const Ref<ParticleSource>& ParticlesEmitter::GetParticlesSource() const
	{
		return mParticlesSource;
	}

	void ParticlesEmitter::SetEmittingCoef(float coef)
	{
		mEmittingCoefficient = coef;
	}

	float ParticlesEmitter::GetEmittingCoef() const
	{
		return mEmittingCoefficient;
	}

	void ParticlesEmitter::SetShape(const Ref<ParticlesEmitterShape>& shape)
	{
		mShape = shape;
	}

	const Ref<ParticlesEmitterShape>& ParticlesEmitter::GetShape() const
	{
		return mShape;
	}

	void ParticlesEmitter::AddEffect(const Ref<ParticlesEffect>& effect)
	{
		mEffects.Add(effect);
	}

	const Vector<Ref<ParticlesEffect>>& ParticlesEmitter::GetEffects() const
	{
		return mEffects;
	}

	void ParticlesEmitter::RemoveEffect(const Ref<ParticlesEffect>& effect)
	{
		mEffects.Remove(effect);
	}

	void ParticlesEmitter::RemoveAllEffects()
	{
		mEffects.Clear();
	}

	void ParticlesEmitter::SetMaxParticles(int count)
	{
		mParticlesNumLimit = count;

		int idx = 0;
		while (mNumAliveParticles > mParticlesNumLimit)
		{
			if (mParticles[idx].alive)
			{
				mParticles[idx].alive = false;
				mDeadParticles.Add(idx);
				mNumAliveParticles--;
			}

			idx++;
		}
	}

	int ParticlesEmitter::GetMaxParticles() const
	{
		return mParticlesNumLimit;
	}

	int ParticlesEmitter::GetParticlesCount() const
	{
		return mNumAliveParticles;
	}

	bool ParticlesEmitter::IsAliveParticles() const
	{
		return mNumAliveParticles > 0;
	}

	const Vector<Particle>& ParticlesEmitter::GetParticles() const
	{
		return mParticles;
	}

	void ParticlesEmitter::SetParticlesRelativity(bool relative)
	{
		mIsParticlesRelative = relative;
	}

	bool ParticlesEmitter::IsParticlesRelative() const
	{
		return mIsParticlesRelative;
	}

	void ParticlesEmitter::SetLoop(bool looped)
	{
		mIsLooped = looped;
	}

	bool ParticlesEmitter::IsLooped() const
	{
		return mIsLooped;
	}

	void ParticlesEmitter::SetDuration(float duration)
	{
		mDuration = duration;
	}

	float ParticlesEmitter::GetDuration() const
	{
		return mDuration;
	}

	void ParticlesEmitter::SetParticlesLifetime(float lifetime)
	{
		mParticlesLifetime = lifetime;
	}

	float ParticlesEmitter::GetParticlesLifetime() const
	{
		return mParticlesLifetime;
	}

	void ParticlesEmitter::SetParticlesPerSecond(float numParticles)
	{
		mEmitParticlesPerSecond = numParticles;
	}

	float ParticlesEmitter::GetParticlesPerSecond() const
	{
		return mEmitParticlesPerSecond;
	}

	void ParticlesEmitter::SetInitialAngle(float angle)
	{
		mInitialAngle = Math::Deg2rad(angle);
	}

	float ParticlesEmitter::GetInitialAngle() const
	{
		return Math::Rad2deg(mInitialAngle);
	}

	void ParticlesEmitter::SetInitialAngleRange(float range)
	{
		mInitialAngleRange = initialAngle;
	}

	float ParticlesEmitter::GetInitialAngleRange() const
	{
		return mInitialAngleRange;
	}

	void ParticlesEmitter::SetInitialSize(const Vec2F& size)
	{
		mInitialSize = size;
	}

	Vec2F ParticlesEmitter::GetInitialSize() const
	{
		return mInitialSize;
	}

	void ParticlesEmitter::SetInitialSizeRange(const Vec2F& range)
	{
		mInitialSizeRange = range;
	}

	Vec2F ParticlesEmitter::GetInitialSizeRange() const
	{
		return mInitialSizeRange;
	}

	void ParticlesEmitter::SetInitialAngleSpeed(float speed)
	{
		mInitialAngleSpeed = speed;
	}

	float ParticlesEmitter::GetInitialAngleSpeed() const
	{
		return mInitialAngleSpeed;
	}

	void ParticlesEmitter::SetInitialAngleSpeedRange(float speedRange)
	{
		mInitialAngleSpeedRange = Math::Deg2rad(speedRange);
	}

	float ParticlesEmitter::GetInitialAngleSpeedRange() const
	{
		return Math::Rad2deg(mInitialAngleSpeedRange);
	}

	void ParticlesEmitter::SetInitialSpeed(float speed)
	{
		mInitialSpeed = speed;
	}

	float ParticlesEmitter::GetInitialSpeed() const
	{
		return mInitialSpeed;
	}

	void ParticlesEmitter::SetInitialSpeedRange(float speedRange)
	{
		mInitialSpeedRangle = speedRange;
	}

	float ParticlesEmitter::GetInitialSpeedRange() const
	{
		return mInitialSpeed;
	}

	void ParticlesEmitter::SetEmitParticlesMoveDirection(float direction)
	{
		mInitialMoveDirection = Math::Deg2rad(direction);
	}

	float ParticlesEmitter::GetEmitParticlesMoveDirection() const
	{
		return Math::Rad2deg(mInitialMoveDirection);
	}

	void ParticlesEmitter::SetEmitParticlesMoveDirectionRange(float directionRange)
	{
		mInitialMoveDirectionRange = Math::Deg2rad(directionRange);
	}

	float ParticlesEmitter::GetEmitParticlesMoveDirectionRange() const
	{
		return Math::Rad2deg(mInitialMoveDirectionRange);
	}
}
// --- META ---

DECLARE_CLASS(o2::ParticlesEmitter, o2__ParticlesEmitter);
// --- END META ---
