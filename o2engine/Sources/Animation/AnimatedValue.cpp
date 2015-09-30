#include "AnimatedValue.h"

namespace o2
{

	AnimatedValue<float>::AnimatedValue():
		mTarget(nullptr), mTargetProperty(nullptr)
	{
		curve.onKeysChanged.Add(this, &AnimatedValue<float>::OnCurveChanged);
		InitializeProperties();
	}

	AnimatedValue<float>::AnimatedValue(const AnimatedValue<float>& other):
		mTarget(other.mTarget), mTargetProperty(other.mTargetProperty),
		mTargetDelegate(other.mTargetDelegate), curve(other.curve),
		mValue(other.mValue), IAnimatedValue(other)
	{
		curve.onKeysChanged.Add(this, &AnimatedValue<float>::OnCurveChanged);
		InitializeProperties();
	}

	AnimatedValue<float>& AnimatedValue<float>::operator=(const AnimatedValue<float>& other)
	{
		IAnimation::operator =(other);

		mTarget = other.mTarget;
		mTargetProperty = other.mTargetProperty;
		mTargetDelegate = other.mTargetDelegate;
		curve = other.curve;
		mValue = other.mValue;

		return *this;
	}

	void AnimatedValue<float>::SetTarget(float* value)
	{
		mTargetProperty = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	void AnimatedValue<float>::SetTarget(float* value, const Function<void()>& changeEvent)
	{
		mTargetProperty = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	void AnimatedValue<float>::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	void AnimatedValue<float>::SetTargetProperty(Setter<float>* setter)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProperty = setter;
	}

	float AnimatedValue<float>::GetValue()
	{
		return mValue;
	}

	void AnimatedValue<float>::Evaluate()
	{
		if (curve.IsEmpty())
			return;

		mValue = curve.Evaluate(mInDurationTime);

		if (mTarget)
		{
			*mTarget = mValue;
			mTargetDelegate();
		}
		else if (mTargetProperty)
			*mTargetProperty = mValue;
	}

	void AnimatedValue<float>::AddKeys(Vector<Vec2F> values, float smooth /*= 1.0f*/)
	{
		curve.AddKeys(values, smooth);
	}

	void AnimatedValue<float>::AddKey(const Curve::Key& key)
	{
		curve.AddKey(key);
	}

	void AnimatedValue<float>::AddKey(float position, float value, float leftCoef, float leftCoefPosition,
									  float rightCoef, float rightCoefPosition)
	{
		curve.AddKey(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition);
	}

	void AnimatedValue<float>::AddKey(float position, float value, float smooth /*= 1.0f*/)
	{
		curve.AddKey(position, value, smooth);
	}

	Curve::Key AnimatedValue<float>::GetKey(float position)
	{
		return curve.GetKey(position);
	}

	bool AnimatedValue<float>::RemoveKey(float position)
	{
		return curve.RemoveKey(position);
	}

	void AnimatedValue<float>::RemoveAllKeys()
	{
		curve.RemoveAllKeys();
	}

	bool AnimatedValue<float>::ContainsKey(float position)
	{
		return curve.ContainsKey(position);
	}

	const Curve::KeysVec& AnimatedValue<float>::GetKeys() const
	{
		return curve.GetKeys();
	}

	void AnimatedValue<float>::SetKeys(const Curve::KeysVec& keys)
	{
		curve.SetKeys(keys);
	}

	void AnimatedValue<float>::SmoothKey(float position, float smooth)
	{
		curve.SmoothKey(position, smooth);
	}

	Curve::Key AnimatedValue<float>::operator[](float position)
	{
		return curve.GetKey(position);
	}

	AnimatedValue<float> AnimatedValue<float>::Parametric(float begin, float end, float duration, 
														  float beginCoef, float beginCoefPosition, 
														  float endCoef, float endCoefPosition)
	{
		AnimatedValue<float> res;
		res.curve.AddKey(0.0f, begin, 0.0f, 0.0f, beginCoef*begin, beginCoefPosition);
		res.curve.AddKey(duration, end, endCoef*end, endCoefPosition, 0.0f, 0.0f);
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
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 0.6f);
	}

	AnimatedValue<float> AnimatedValue<float>::Linear(float begin /*= 0.0f*/, float end /*= 1.0f*/, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}

	Curve::KeysVec AnimatedValue<float>::GetKeysNonContant()
	{
		return curve.GetKeys();
	}

	void AnimatedValue<float>::OnCurveChanged()
	{
		float lastDuration = mDuration;
		mDuration = curve.Length();

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;
	}

	void AnimatedValue<float>::InitializeProperties()
	{
		INITIALIZE_GETTER(AnimatedValue<float>, value, GetValue);
		INITIALIZE_SETTER(AnimatedValue<float>, target, SetTarget);
		INITIALIZE_SETTER(AnimatedValue<float>, targetDelegate, SetTargetDelegate);
		INITIALIZE_SETTER(AnimatedValue<float>, targetProperty, SetTargetProperty);
		INITIALIZE_ACCESSOR(AnimatedValue<float>, key, GetKey);
		INITIALIZE_PROPERTY(AnimatedValue<float>, keys, SetKeys, GetKeysNonContant);
	}


}