#include "o2/stdafx.h"
#include "AnimatedFloat.h"

#include "o2/Animation/AnimationState.h"
#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
	AnimatedValue<float>::AnimatedValue()
	{
		curve.onKeysChanged.Add(this, &AnimatedValue<float>::OnCurveChanged);
	}

	AnimatedValue<float>::AnimatedValue(const AnimatedValue<float>& other):
		curve(other.curve), mValue(other.mValue), IAnimatedValue(other),
		value(this), target(this), targetDelegate(this), targetProxy(this), keys(this)
	{
		curve.onKeysChanged.Add(this, &AnimatedValue<float>::OnCurveChanged);
	}

	AnimatedValue<float>::~AnimatedValue()
	{
		if (mTargetProxy)
			delete mTargetProxy;
	}

	AnimatedValue<float>& AnimatedValue<float>::operator=(const AnimatedValue<float>& other)
	{
		IAnimation::operator =(other);

		curve = other.curve;
		mValue = other.mValue;

		onKeysChanged();

		return *this;
	}

	AnimatedValue<float>::operator float() const
	{
		return mValue;
	}

	void AnimatedValue<float>::SetTarget(float* value)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	void AnimatedValue<float>::SetTarget(float* value, const Function<void()>& changeEvent)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	void AnimatedValue<float>::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	void AnimatedValue<float>::SetTargetProxy(IValueProxy<float>* proxy)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProxy = proxy;
	}

	float AnimatedValue<float>::GetValue() const
	{
		return mValue;
	}

	float AnimatedValue<float>::GetValue(float time) const
	{
		return curve.Evaluate(time);
	}

	void AnimatedValue<float>::Evaluate()
	{
		mValue = curve.Evaluate(mInDurationTime);

		if (mTarget)
		{
			*mTarget = mValue;
			mTargetDelegate();
		}
		else if (mTargetProxy)
			mTargetProxy->SetValue(mValue);
	}

	void AnimatedValue<float>::BeginKeysBatchChange()
	{
		curve.BeginKeysBatchChange();
	}

	void AnimatedValue<float>::CompleteKeysBatchingChange()
	{
		curve.CompleteKeysBatchingChange();
	}

	void AnimatedValue<float>::AddKeys(Vector<Vec2F> values, float smooth /*= 1.0f*/)
	{
		curve.AppendKeys(values, smooth);
	}

	int AnimatedValue<float>::AddKey(const Key& key)
	{
		return curve.InsertKey(key);
	}

	int AnimatedValue<float>::AddKey(const Key& key, float position)
	{
		Key newKey = key;
		newKey.position = position;
		return curve.InsertKey(newKey);
	}

	int AnimatedValue<float>::AddKey(float position, float value, float leftCoef, float leftCoefPosition,
									  float rightCoef, float rightCoefPosition)
	{
		return curve.InsertKey(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition);
	}

	int AnimatedValue<float>::AddKey(float position, float value, float smooth /*= 1.0f*/)
	{
		return curve.InsertKey(position, value, smooth);
	}

	AnimatedValue<float>::Key AnimatedValue<float>::GetKey(float position) const
	{
		return curve.GetKey(position);
	}

	AnimatedValue<float>::Key AnimatedValue<float>::GetKeyAt(int idx) const
	{
		return curve.GetKeyAt(idx);
	}

	AnimatedValue<float>::Key AnimatedValue<float>::FindKey(UInt64 uid) const
	{
		return curve.FindKey(uid);
	}

	int AnimatedValue<float>::FindKeyIdx(UInt64 uid) const
	{
		return curve.FindKeyIdx(uid);
	}

	bool AnimatedValue<float>::RemoveKey(float position)
	{
		return curve.RemoveKey(position);
	}

	bool AnimatedValue<float>::RemoveKeyAt(int idx)
	{
		return curve.RemoveKeyAt(idx);
	}


	void AnimatedValue<float>::RemoveAllKeys()
	{
		curve.RemoveAllKeys();
	}

	bool AnimatedValue<float>::ContainsKey(float position) const
	{
		return curve.ContainsKey(position);
	}

	const AnimatedValue<float>::KeysVec& AnimatedValue<float>::GetKeys() const
	{
		return curve.GetKeys();
	}

	void AnimatedValue<float>::SetKeys(const KeysVec& keys)
	{
		curve.SetKeys(keys);
	}

	void AnimatedValue<float>::SmoothKey(float position, float smooth)
	{
		curve.SmoothKey(position, smooth);
	}

	AnimatedValue<float>::Key AnimatedValue<float>::operator[](float position) const
	{
		return curve.GetKey(position);
	}

	AnimatedValue<float>::KeysVec AnimatedValue<float>::GetKeysNonContant()
	{
		return curve.GetKeys();
	}

	void AnimatedValue<float>::OnCurveChanged()
	{
		float lastDuration = mDuration;
		mDuration = curve.Length();

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;

		onKeysChanged();
	}

	void AnimatedValue<float>::OnDeserialized(const DataNode& node)
	{
		mEndTime = mDuration;
	}

	void AnimatedValue<float>::SetTargetVoid(void* target)
	{
		SetTarget((float*)target);
	}

	void AnimatedValue<float>::SetTargetVoid(void* target, const Function<void()>& changeEvent)
	{
		SetTarget((float*)target, changeEvent);
	}

	void AnimatedValue<float>::SetTargetProxyVoid(void* target)
	{
		SetTargetProxy((IValueProxy<float>*)target);
	}

	void IAnimatedValue::ForceSetTime(float time, float duration)
	{
		float lastTime = mTime;
		mTime = time;

		float lastInDurationTime = mInDurationTime;

		if (mLoop == Loop::None)
		{
			mInDurationTime = Math::Clamp(mTime, mBeginTime, mEndTime);
			Evaluate();

			if (lastTime < mTime)
			{
				if (mEndTime > Math::Min(mTime, lastTime) && mEndTime <= Math::Max(mTime, lastTime))
				{
					onStopEvent();
					onPlayedEvent();
				}
			}
			else
			{
				if (mBeginTime > Math::Min(mTime, lastTime) && mBeginTime <= Math::Max(mTime, lastTime))
				{
					onStopEvent();
					onPlayedEvent();
				}
			}
		}
		else if (mLoop == Loop::Repeat)
		{
			float x;
			if (mTime > 0)
				mInDurationTime = modff(mTime/mDuration, &x)*mDuration;
			else
				mInDurationTime = (1.0f - modff(-mTime/mDuration, &x))*mDuration;

			mInDurationTime = Math::Clamp(mInDurationTime, mBeginTime, mEndTime);

			Evaluate();
		}
		else //if (mLoop == Loop::PingPong)
		{
			float x;
			if (mTime > 0)
			{
				mInDurationTime = modff(mTime/mDuration, &x)*mDuration;
				if ((int)x%2 == 1)
					mInDurationTime = mDuration - mInDurationTime;
			}
			else
			{
				mInDurationTime = (1.0f - modff(-mTime/mDuration, &x))*mDuration;
				if ((int)x%2 == 0)
					mInDurationTime = mDuration - mInDurationTime;
			}

			mInDurationTime = Math::Clamp(mInDurationTime, mBeginTime, mEndTime);

			Evaluate();
		}

		onUpdate(mTime);

		float eventCheckBeg = Math::Min(lastInDurationTime, mInDurationTime);
		float eventCheckEnd = Math::Max(lastInDurationTime, mInDurationTime);
		for (auto& kv : mTimeEvents)
		{
			if (kv.first > eventCheckBeg && kv.first <= eventCheckEnd)
				kv.second.Invoke();
		}
	}

	void AnimatedValue<float>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<float>(this, path, state);
	}

	AnimatedValue<float> AnimatedValue<float>::Parametric(float begin, float end, float duration,
														  float beginCoef, float beginCoefPosition,
														  float endCoef, float endCoefPosition)
	{
		AnimatedValue<float> res;
		res.curve.InsertKey(0.0f, begin, 0.0f, 0.0f, Math::Lerp(begin, end, beginCoef) - begin, beginCoefPosition*duration);
		res.curve.InsertKey(duration, end, Math::Lerp(begin, end, endCoef) - end, -endCoefPosition*duration, 0.0f, 0.0f);
		return res;
	}


	AnimatedValue<float> AnimatedValue<float>::EaseIn(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.5f, 1.0f, 1.0f);
	}

	AnimatedValue<float> AnimatedValue<float>::EaseOut(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 0.5f);
	}

	AnimatedValue<float> AnimatedValue<float>::EaseInOut(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 1.0f);
	}

	AnimatedValue<float> AnimatedValue<float>::Linear(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}
}

DECLARE_CLASS(o2::AnimatedValue<float>);
