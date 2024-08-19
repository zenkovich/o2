#include "o2/stdafx.h"
#include "ParticlesEmitter.h"

#include "o2/Render/Mesh.h"
#include "o2/Render/Particles/ParticlesEffects.h"
#include "o2/Render/Particles/ParticlesEmitterShapes.h"

namespace o2
{
    ParticlesEmitter::ParticlesEmitter():
        IRectDrawable(), mShape(mmake<CircleParticlesEmitterShape>())
    {
        mLastTransform = mTransform;
    }

    ParticlesEmitter::~ParticlesEmitter()
    {
    }

    ParticlesEmitter::ParticlesEmitter(const ParticlesEmitter& other):
        IRectDrawable(other), mParticlesSource(other.mParticlesSource->CloneAsRef<ParticleSource>()), 
        mShape(other.mShape->CloneAsRef<ParticlesEmitterShape>()),
        mParticlesNumLimit(other.mParticlesNumLimit), mPlaying(other.mPlaying), 
        mEmittingCoefficient(other.mEmittingCoefficient), mIsLooped(other.mIsLooped),
        mIsParticlesRelative(other.mIsParticlesRelative), mDuration(other.mDuration),
        mParticlesLifetime(other.mParticlesLifetime), mEmitParticlesPerSecond(other.mEmitParticlesPerSecond),
        mEmitParticlesAngle(other.mEmitParticlesAngle), mEmitParticlesAngleRange(other.mEmitParticlesAngleRange),
        mEmitParticlesSize(other.mEmitParticlesSize), mEmitParticlesSizeRange(other.mEmitParticlesSizeRange),
        mEmitParticlesSpeed(other.mEmitParticlesSpeed), mEmitParticlesSpeedRangle(other.mEmitParticlesSpeedRangle),
        mEmitParticlesMoveDirection(other.mEmitParticlesMoveDirection), mEmitParticlesMoveDirectionRange(other.mEmitParticlesMoveDirectionRange),
        mEmitParticlesColorA(other.mEmitParticlesColorA), mEmitParticlesColorB(other.mEmitParticlesColorB),
        playing(this), emittingCoefficient(this), particlesRelative(this), looped(this), maxParticles(this),
        duration(this), particlesLifetime(this), emitParticlesPerSecond(this), emitParticlesAngle(this), emitParticlesAngleRange(this),
        emitParticlesSize(this), emitParticlesSizeRange(this), emitParticlesSpeed(this), emitParticlesAngleSpeedRange(this), emitParticlesAngleSpeed(this),
        emitParticlesSpeedRange(this), emitParticlesMoveDir(this), emitParticlesMoveDirRange(this), emitParticlesColorA(this), emitParticlesColorB(this),
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

        mEmitParticlesAngle = other.mEmitParticlesAngle;
        mEmitParticlesAngleRange = other.mEmitParticlesAngleRange;

        mEmitParticlesSize = other.mEmitParticlesSize;
        mEmitParticlesSizeRange = other.mEmitParticlesSizeRange;

        mEmitParticlesSpeed = other.mEmitParticlesSpeed;
        mEmitParticlesSpeedRangle = other.mEmitParticlesSpeedRangle;

        mEmitParticlesMoveDirection = other.mEmitParticlesMoveDirection;
        mEmitParticlesMoveDirectionRange = other.mEmitParticlesMoveDirectionRange;

        mEmitParticlesColorA = other.mEmitParticlesColorA;
        mEmitParticlesColorB = other.mEmitParticlesColorB;

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
        mEmitTimeBuffer += dt;

        float currentParticlesPerSecond = mEmitParticlesPerSecond*mEmittingCoefficient;
        if (!mPlaying)
            currentParticlesPerSecond = 0;

        if (currentParticlesPerSecond < FLT_EPSILON)
            currentParticlesPerSecond = FLT_EPSILON;

        float particlesDelay = 1.0f/mEmitParticlesPerSecond;

        float halfAngleRange = mEmitParticlesAngleRange*0.5f;
        Vec2F halfSizeRange = mEmitParticlesSizeRange*0.5f;
        float halfSpeedRange = mEmitParticlesSpeedRangle*0.5f;
        float halfDirRange = mEmitParticlesMoveDirectionRange*0.5f;
        float halfAngleSpeedRange = mEmitParticlesAngleSpeedRange*0.5f;
		float halfLifetimeRange = mParticlesLifetimeRange * 0.5f;

        while (mEmitTimeBuffer > particlesDelay)
        {
            if (mNumAliveParticles < mParticlesNumLimit)
            {
                Particle* p;

                if (mDeadParticles.IsEmpty())
                {
                    mParticles.Add(Particle());
                    p = &mParticles.Last();
                }
                else p = &mParticles[mDeadParticles.PopBack()];

                p->position = Local2WorldPoint(mShape->GetEmittinPoint());
                p->angle = mEmitParticlesAngle + Math::Random(-halfAngleRange, halfAngleRange);

                p->size.Set(mEmitParticlesSize.x + Math::Random(-halfSizeRange.x, halfSizeRange.x),
                            mEmitParticlesSize.y + Math::Random(-halfSizeRange.y, halfSizeRange.y));

                p->velocity = Vec2F::Rotated(mEmitParticlesMoveDirection + Math::Random(-halfDirRange, halfDirRange))*
                    (mEmitParticlesSpeed + Math::Random(-halfSpeedRange, halfSpeedRange));

                p->angleSpeed = mEmitParticlesAngleSpeed + Math::Random(-halfAngleSpeedRange, halfAngleSpeedRange);

                p->color.r = Math::Random(mEmitParticlesColorA.r, mEmitParticlesColorB.r);
                p->color.g = Math::Random(mEmitParticlesColorA.g, mEmitParticlesColorB.g);
                p->color.b = Math::Random(mEmitParticlesColorA.b, mEmitParticlesColorB.b);
                p->color.a = Math::Random(mEmitParticlesColorA.a, mEmitParticlesColorB.a);
                p->timeLeft = mParticlesLifetime + Math::Random(-halfLifetimeRange, halfLifetimeRange);
                p->lifetime = p->timeLeft;
                p->alive = true;

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

    void ParticlesEmitter::SetEmitParticlesPerSecond(float numParticles)
    {
        mEmitParticlesPerSecond = numParticles;
    }

    float ParticlesEmitter::GetEmitParticlesPerSecond() const
    {
        return mEmitParticlesPerSecond;
    }

    void ParticlesEmitter::SetEmitParticlesAngle(float angle)
    {
        mEmitParticlesAngle = Math::Deg2rad(angle);
    }

    float ParticlesEmitter::GetEmitParticlesAngle() const
    {
        return Math::Rad2deg(mEmitParticlesAngle);
    }

    void ParticlesEmitter::SetEmitParticlesAngleRange(float range)
    {
        mEmitParticlesAngleRange = Math::Deg2rad(angle);
    }

    float ParticlesEmitter::GetEmitParticlesAngleRange() const
    {
        return Math::Rad2deg(mEmitParticlesAngleRange);
    }

    void ParticlesEmitter::SetEmitParticlesSize(const Vec2F& size)
    {
        mEmitParticlesSize = size;
    }

    Vec2F ParticlesEmitter::GetEmitParticlesSize() const
    {
        return mEmitParticlesSize;
    }

    void ParticlesEmitter::SetEmitParticlesSizeRange(const Vec2F& range)
    {
        mEmitParticlesSizeRange = range;
    }

    Vec2F ParticlesEmitter::GetEmitParticlesSizeRange() const
    {
        return mEmitParticlesSizeRange;
    }

    void ParticlesEmitter::SetEmitParticlesAngleSpeed(float speed)
    {
        mEmitParticlesAngleSpeed = Math::Deg2rad(speed);
    }

    float ParticlesEmitter::GetEmitParticlesAngleSpeed() const
    {
        return Math::Rad2deg(mEmitParticlesAngleSpeed);
    }

    void ParticlesEmitter::SetEmitParticlesAngleSpeedRange(float speedRange)
    {
        mEmitParticlesAngleSpeedRange = Math::Deg2rad(speedRange);
    }

    float ParticlesEmitter::GetEmitParticlesAngleSpeedRange() const
    {
        return Math::Rad2deg(mEmitParticlesAngleSpeedRange);
    }

    void ParticlesEmitter::SetEmitParticlesSpeed(float speed)
    {
        mEmitParticlesSpeed = speed;
    }

    float ParticlesEmitter::GetEmitParticlesSpeed() const
    {
        return mEmitParticlesSpeed;
    }

    void ParticlesEmitter::SetEmitParticlesSpeedRange(float speedRange)
    {
        mEmitParticlesSpeedRangle = speedRange;
    }

    float ParticlesEmitter::GetEmitParticlesSpeedRange() const
    {
        return mEmitParticlesSpeed;
    }

    void ParticlesEmitter::SetEmitParticlesMoveDirection(float direction)
    {
        mEmitParticlesMoveDirection = Math::Deg2rad(direction);
    }

    float ParticlesEmitter::GetEmitParticlesMoveDirection() const
    {
        return Math::Rad2deg(mEmitParticlesMoveDirection);
    }

    void ParticlesEmitter::SetEmitParticlesMoveDirectionRange(float directionRange)
    {
        mEmitParticlesMoveDirectionRange = Math::Deg2rad(directionRange);
    }

    float ParticlesEmitter::GetEmitParticlesMoveDirectionRange() const
    {
        return Math::Rad2deg(mEmitParticlesMoveDirectionRange);
    }

    void ParticlesEmitter::SetEmitParticlesColorA(const Color4& color)
    {
        mEmitParticlesColorA = color;
    }

    Color4 ParticlesEmitter::GetEmitParticlesColorA() const
    {
        return mEmitParticlesColorA;
    }

    void ParticlesEmitter::SetEmitParticlesColorB(const Color4& color)
    {
        mEmitParticlesColorB = color;
    }

    Color4 ParticlesEmitter::GetEmitParticlesColorB() const
    {
        return mEmitParticlesColorB;
    }

    void ParticlesEmitter::SetEmitParticlesColor(const Color4& color)
    {
        mEmitParticlesColorA = color;
        mEmitParticlesColorB = color;
    }

    void ParticlesEmitter::SetEmitParticlesColor(const Color4& colorA, const Color4& colorB)
    {
        mEmitParticlesColorA = colorA;
        mEmitParticlesColorB = colorB;
    }
}
// --- META ---

DECLARE_CLASS(o2::ParticlesEmitter, o2__ParticlesEmitter);
// --- END META ---
