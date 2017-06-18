#include "ParticlesEmitter.h"

#include "Render/Mesh.h"
#include "Render/ParticlesEffects.h"
#include "Render/ParticlesEmitterShapes.h"

namespace o2
{
	ParticlesEmitter::ParticlesEmitter():
		IRectDrawable()
	{
		mShape = mnew CircleParticlesEmitterShape();
		mParticlesMesh = mnew Mesh(NoTexture(), mParticlesNumLimit*4, mParticlesNumLimit*2);
		mLastTransform = mTransform;

		InitializeProperties();
	}

	ParticlesEmitter::~ParticlesEmitter()
	{
		delete mParticlesMesh;

		for (auto effect : mEffects)
			delete effect;
	}

	ParticlesEmitter::ParticlesEmitter(const ParticlesEmitter& other):
		IRectDrawable(other), mImageAsset(other.mImageAsset), mShape(other.mShape->Clone()), mParticlesNumLimit(other.mParticlesNumLimit),
		mPlaying(other.mPlaying), mEmittingCoefficient(other.mEmittingCoefficient), mIsLooped(other.mIsLooped),
		mIsParticlesRelative(other.mIsParticlesRelative), mDuration(other.mDuration),
		mParticlesLifetime(other.mParticlesLifetime), mEmitParticlesPerSecond(other.mEmitParticlesPerSecond),
		mEmitParticlesAngle(other.mEmitParticlesAngle), mEmitParticlesAngleRange(other.mEmitParticlesAngleRange),
		mEmitParticlesSize(other.mEmitParticlesSize), mEmitParticlesSizeRange(other.mEmitParticlesSizeRange),
		mEmitParticlesSpeed(other.mEmitParticlesSpeed), mEmitParticlesSpeedRangle(other.mEmitParticlesSpeedRangle),
		mEmitParticlesMoveDirection(other.mEmitParticlesMoveDirection), mEmitParticlesMoveDirectionRange(other.mEmitParticlesMoveDirectionRange),
		mEmitParticlesColorA(other.mEmitParticlesColorA), mEmitParticlesColorB(other.mEmitParticlesColorB)
	{
		mParticlesMesh = mnew Mesh(NoTexture(), mParticlesNumLimit*4, mParticlesNumLimit*2);

		for (auto effect : other.mEffects)
			AddEffect(effect->Clone());

		mLastTransform = mTransform;

		InitializeProperties();
	}

	ParticlesEmitter& ParticlesEmitter::operator=(const ParticlesEmitter& other)
	{
		RemoveAllEffects();
		delete mShape;

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

		for (auto effect : other.mEffects)
			AddEffect(effect->Clone());

		mImageAsset = other.mImageAsset;
		mShape = other.mShape->Clone();

		for (auto effect : other.mEffects)
			AddEffect(effect->Clone());

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

		mParticlesMesh->vertexCount = 0;
		mParticlesMesh->polyCount = 0;
		mParticlesMesh->Resize(mParticlesNumLimit*4, mParticlesNumLimit*2);

		mLastTransform = mTransform;

		return *this;
	}

	void ParticlesEmitter::Draw()
	{
		mParticlesMesh->Draw();
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
		UpdateMesh();
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
				p->time = mParticlesLifetime;
				p->alive = true;

				mNumAliveParticles++;
			}

			mEmitTimeBuffer -= particlesDelay;
		}
	}

	void ParticlesEmitter::UpdateEffects(float dt)
	{
		for (auto effect : mEffects)
			effect->Update(dt, this);
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
			particle.time -= dt;

			if (particle.time < 0)
			{
				particle.alive = false;
				mDeadParticles.Add(idx);
				mNumAliveParticles--;
			}

			idx++;
		}
	}

	void ParticlesEmitter::UpdateMesh()
	{
		if (mParticlesMesh->GetMaxVertexCount() < mParticlesNumLimit*4)
			mParticlesMesh->Resize(mParticlesNumLimit*4, mParticlesNumLimit*2);

		mParticlesMesh->vertexCount = 0;
		mParticlesMesh->polyCount = 0;
		int polyIndex = 0;

		Vec2F invTexSize(1.0f, 1.0f);
		if (mParticlesMesh->GetTexture())
		{
			invTexSize.Set(1.0f/mParticlesMesh->GetTexture()->GetSize().x,
						   1.0f/mParticlesMesh->GetTexture()->GetSize().y);
		}

		RectF textureSrcRect;
		if (mImageAsset)
			textureSrcRect = mImageAsset->GetAtlasRect();

		float uvLeft = textureSrcRect.left*invTexSize.x;
		float uvRight = textureSrcRect.right*invTexSize.x;
		float uvUp = 1.0f - textureSrcRect.bottom*invTexSize.y;
		float uvDown = 1.0f - textureSrcRect.top*invTexSize.y;

		for (auto& particle : mParticles)
		{
			if (!particle.alive)
				continue;

			float sn = Math::Sin(particle.angle), cs = Math::Cos(particle.angle);
			Vec2F hs = particle.size*0.5f;
			Vec2F xv(cs*hs.x, sn*hs.x);
			Vec2F yv(-sn*hs.y, cs*hs.y);
			Vec2F o(particle.position);
			ULong colr = particle.color.ARGB();

			mParticlesMesh->vertices[mParticlesMesh->vertexCount++].Set(o - xv + yv, colr, uvLeft, uvUp);
			mParticlesMesh->vertices[mParticlesMesh->vertexCount++].Set(o + xv + yv, colr, uvRight, uvUp);
			mParticlesMesh->vertices[mParticlesMesh->vertexCount++].Set(o + xv - yv, colr, uvRight, uvDown);
			mParticlesMesh->vertices[mParticlesMesh->vertexCount++].Set(o - xv - yv, colr, uvLeft, uvDown);

			mParticlesMesh->indexes[polyIndex++] = mParticlesMesh->vertexCount - 4;
			mParticlesMesh->indexes[polyIndex++] = mParticlesMesh->vertexCount - 3;
			mParticlesMesh->indexes[polyIndex++] = mParticlesMesh->vertexCount - 2;

			mParticlesMesh->indexes[polyIndex++] = mParticlesMesh->vertexCount - 4;
			mParticlesMesh->indexes[polyIndex++] = mParticlesMesh->vertexCount - 2;
			mParticlesMesh->indexes[polyIndex++] = mParticlesMesh->vertexCount - 1;
			mParticlesMesh->polyCount += 2;
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

	void ParticlesEmitter::SetEmittingCoef(float coef)
	{
		mEmittingCoefficient = coef;
	}

	float ParticlesEmitter::GetEmittingCoef() const
	{
		return mEmittingCoefficient;
	}

	void ParticlesEmitter::SetImage(const ImageAssetRef& image)
	{
		mImageAsset = image;

		if (mImageAsset)
			mParticlesMesh->SetTexture(TextureRef(mImageAsset->GetAtlasId(), mImageAsset->GetAtlasPage()));
		else
			mParticlesMesh->SetTexture(NoTexture());
	}

	ImageAssetRef ParticlesEmitter::GetImage() const
	{
		return mImageAsset;
	}

	void ParticlesEmitter::SetShape(ParticlesEmitterShape* shape)
	{
		if (mShape)
			delete mShape;

		mShape = shape;
	}

	ParticlesEmitterShape* ParticlesEmitter::GetShape() const
	{
		return mShape;
	}

	void ParticlesEmitter::AddEffect(ParticlesEffect* effect)
	{
		mEffects.Add(effect);
	}

	const ParticlesEmitter::ParticleEffectsVec& ParticlesEmitter::GetEffects() const
	{
		return mEffects;
	}

	void ParticlesEmitter::RemoveEffect(ParticlesEffect* effect)
	{
		mEffects.Remove(effect);
		delete effect;
	}

	void ParticlesEmitter::RemoveAllEffects()
	{
		for (auto effect : mEffects)
			delete effect;

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

	const ParticlesEmitter::ParticlesVec& ParticlesEmitter::GetParticles() const
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

	void ParticlesEmitter::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ParticlesEmitter, playing, SetPlaying, IsPlaying);
		INITIALIZE_PROPERTY(ParticlesEmitter, emittingCoefficient, SetEmittingCoef, GetEmittingCoef);
		INITIALIZE_PROPERTY(ParticlesEmitter, particlesRelative, SetParticlesRelativity, IsParticlesRelative);
		INITIALIZE_PROPERTY(ParticlesEmitter, looped, SetLoop, IsLooped);
		INITIALIZE_PROPERTY(ParticlesEmitter, maxParticles, SetMaxParticles, GetMaxParticles);
		INITIALIZE_PROPERTY(ParticlesEmitter, duration, SetDuration, GetDuration);
		INITIALIZE_PROPERTY(ParticlesEmitter, particlesLifetime, SetParticlesLifetime, GetParticlesLifetime);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesPerSecond, SetEmitParticlesPerSecond, GetEmitParticlesPerSecond);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesAngle, SetEmitParticlesAngle, GetEmitParticlesAngle);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesAngleRange, SetEmitParticlesAngleRange, GetEmitParticlesAngleRange);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesSize, SetEmitParticlesSize, GetEmitParticlesSize);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesSizeRange, SetEmitParticlesSizeRange, GetEmitParticlesSizeRange);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesAngleSpeed, SetEmitParticlesAngleSpeed, GetEmitParticlesAngleSpeed);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesAngleSpeedRange, SetEmitParticlesAngleSpeedRange, GetEmitParticlesAngleSpeedRange);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesSpeed, SetEmitParticlesSpeed, GetEmitParticlesSpeed);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesSpeedRange, SetEmitParticlesSpeedRange, GetEmitParticlesSpeedRange);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesMoveDir, SetEmitParticlesMoveDirection, GetEmitParticlesMoveDirection);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesMoveDirRange, SetEmitParticlesMoveDirectionRange, GetEmitParticlesMoveDirectionRange);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesColorA, SetEmitParticlesColorA, GetEmitParticlesColorA);
		INITIALIZE_PROPERTY(ParticlesEmitter, emitParticlesColorB, SetEmitParticlesColorB, GetEmitParticlesColorB);

		INITIALIZE_PROPERTY(ParticlesEmitter, image, SetImage, GetImage);
		INITIALIZE_PROPERTY(ParticlesEmitter, shape, SetShape, GetShape);
	}

}

CLASS_META(o2::ParticlesEmitter)
{
	BASE_CLASS(o2::IRectDrawable);

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
	PROTECTED_FIELD(mEffects).SERIALIZABLE_ATTRIBUTE();
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
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
