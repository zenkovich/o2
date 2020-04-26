#include "o2/stdafx.h"
#include "AnimationFloatTrack.h"

#include "o2/Animation/AnimationState.h"
#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
	AnimationTrack<float>::AnimationTrack()
	{
		curve.onKeysChanged.Add(this, &AnimationTrack<float>::OnCurveChanged);
	}

	AnimationTrack<float>::AnimationTrack(const AnimationTrack<float>& other):
		IAnimationTrack(other), curve(other.curve)
	{
		curve.onKeysChanged.Add(this, &AnimationTrack<float>::OnCurveChanged);
	}

	AnimationTrack<float>& AnimationTrack<float>::operator=(const AnimationTrack<float>& other)
	{
		IAnimationTrack::operator=(other);
		curve = other.curve;

		onKeysChanged();

		return *this;
	}

	float AnimationTrack<float>::GetValue(float position) const
	{
		int cacheKey = 0;
		int cacheKeyApprox = 0;
		return GetValue(position, true, cacheKey, cacheKeyApprox);
	}

	float AnimationTrack<float>::GetValue(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const
	{
		return curve.Evaluate(position, direction, cacheKey, cacheKeyApprox);
	}

	void AnimationTrack<float>::BeginKeysBatchChange()
	{
		curve.BeginKeysBatchChange();
	}

	void AnimationTrack<float>::CompleteKeysBatchingChange()
	{
		curve.CompleteKeysBatchingChange();
	}

	float AnimationTrack<float>::GetDuration() const
	{
		return curve.Length();
	}

	IAnimationTrack::IPlayer* AnimationTrack<float>::CreatePlayer() const
	{
		return mnew Player();
	}

	void AnimationTrack<float>::AddKeys(Vector<Vec2F> values, float smooth /*= 1.0f*/)
	{
		curve.AppendKeys(values, smooth);
	}

	int AnimationTrack<float>::AddKey(const Key& key)
	{
		return curve.InsertKey(key);
	}

	int AnimationTrack<float>::AddKey(const Key& key, float position)
	{
		Key newKey = key;
		newKey.position = position;
		return curve.InsertKey(newKey);
	}

	int AnimationTrack<float>::AddSmoothKey(const Key& key, float smooth)
	{
		return curve.InsertKey(key.position, key.value, smooth);
	}

	int AnimationTrack<float>::AddKey(float position, float value, float leftCoef, float leftCoefPosition,
									  float rightCoef, float rightCoefPosition)
	{
		return curve.InsertKey(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition);
	}

	int AnimationTrack<float>::AddKey(float position, float value, float smooth /*= 1.0f*/)
	{
		return curve.InsertKey(position, value, smooth);
	}

	bool AnimationTrack<float>::RemoveKey(float position)
	{
		return curve.RemoveKey(position);
	}

	bool AnimationTrack<float>::RemoveKeyAt(int idx)
	{
		return curve.RemoveKeyAt(idx);
	}

	void AnimationTrack<float>::RemoveAllKeys()
	{
		curve.RemoveAllKeys();
	}

	bool AnimationTrack<float>::ContainsKey(float position) const
	{
		return curve.ContainsKey(position);
	}

	const Vector<AnimationTrack<float>::Key>& AnimationTrack<float>::GetKeys() const
	{
		return curve.GetKeys();
	}

	AnimationTrack<float>::Key AnimationTrack<float>::GetKey(float position) const
	{
		return curve.GetKey(position);
	}

	AnimationTrack<float>::Key AnimationTrack<float>::GetKeyAt(int idx) const
	{
		return curve.GetKeyAt(idx);
	}

	AnimationTrack<float>::Key AnimationTrack<float>::FindKey(UInt64 uid) const
	{
		return curve.FindKey(uid);
	}

	int AnimationTrack<float>::FindKeyIdx(UInt64 uid) const
	{
		return curve.FindKeyIdx(uid);
	}

	void AnimationTrack<float>::SetKeys(const Vector<Key>& keys)
	{
		curve.SetKeys(keys);
	}

	void AnimationTrack<float>::SmoothKey(float position, float smooth)
	{
		curve.SmoothKey(position, smooth);
	}

	AnimationTrack<float>::Key AnimationTrack<float>::operator[](float position) const
	{
		return curve.GetKey(position);
	}

	Vector<AnimationTrack<float>::Key> AnimationTrack<float>::GetKeysNonContant()
	{
		return curve.GetKeys();
	}

	void AnimationTrack<float>::OnCurveChanged()
	{
		onKeysChanged();
	}

	void AnimationTrack<float>::OnDeserialized(const DataNode& node)
	{}

	AnimationTrack<float> AnimationTrack<float>::Parametric(float begin, float end, float duration,
														  float beginCoef, float beginCoefPosition,
														  float endCoef, float endCoefPosition)
	{
		AnimationTrack<float> res;
		res.curve.InsertKey(0.0f, begin, 0.0f, 0.0f, Math::Lerp(begin, end, beginCoef) - begin, beginCoefPosition*duration);
		res.curve.InsertKey(duration, end, Math::Lerp(begin, end, endCoef) - end, -endCoefPosition*duration, 0.0f, 0.0f);
		return res;
	}


	AnimationTrack<float> AnimationTrack<float>::EaseIn(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.5f, 1.0f, 1.0f);
	}

	AnimationTrack<float> AnimationTrack<float>::EaseOut(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 0.5f);
	}

	AnimationTrack<float> AnimationTrack<float>::EaseInOut(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 1.0f);
	}

	AnimationTrack<float> AnimationTrack<float>::Linear(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}

	AnimationTrack<float>::Player::Player():
		IPlayer()
	{}

	AnimationTrack<float>::Player::~Player()
	{
		if (mTargetProxy)
			delete mTargetProxy;
	}

	AnimationTrack<float>::Player::operator float() const
	{
		return mCurrentValue;
	}

	void AnimationTrack<float>::Player::SetTarget(float* value)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	void AnimationTrack<float>::Player::SetTarget(float* value, const Function<void()>& changeEvent)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	void AnimationTrack<float>::Player::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	void AnimationTrack<float>::Player::SetTargetProxy(IValueProxy<float>* proxy)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProxy = proxy;
	}

	void AnimationTrack<float>::Player::SetTrack(AnimationTrack<float>* track)
	{
		mTrack = track;
		IPlayer::SetTrack(track);
	}

	AnimationTrack<float>* AnimationTrack<float>::Player::GetTrackT() const
	{
		return mTrack;
	}

	void AnimationTrack<float>::Player::SetTargetVoid(void* target)
	{
		SetTarget((float*)target);
	}

	void AnimationTrack<float>::Player::SetTargetVoid(void* target, const Function<void()>& changeEvent)
	{
		SetTarget((float*)target, changeEvent);
	}

	void AnimationTrack<float>::Player::SetTargetProxyVoid(void* target)
	{
		SetTargetProxy((IValueProxy<float>*)target);
	}

	void AnimationTrack<float>::Player::SetTrack(IAnimationTrack* track)
	{
		SetTrack(dynamic_cast<AnimationTrack<float>*>(track));
	}

	IAnimationTrack* AnimationTrack<float>::Player::GetTrack() const
	{
		return mTrack;
	}

	float AnimationTrack<float>::Player::GetValue() const
	{
		return mCurrentValue;
	}

	void AnimationTrack<float>::Player::Evaluate()
	{
		if (!mTrack)
			return;

		mCurrentValue = mTrack->curve.Evaluate(mInDurationTime, mInDurationTime > mPrevInDurationTime, mPrevKey, mPrevKeyApproximation);
		mPrevInDurationTime = mInDurationTime;

		if (mTarget)
		{
			*mTarget = mCurrentValue;
			mTargetDelegate();
		}
		else if (mTargetProxy)
			mTargetProxy->SetValue(mCurrentValue);
	}

	void AnimationTrack<float>::Player::RegMixer(AnimationState* state, const String& path)
	{
		state->mOwner->RegTrack<float>(this, path, state);
	}
}

DECLARE_CLASS(o2::AnimationTrack<float>);

DECLARE_CLASS(o2::AnimationTrack<float>::Player);
