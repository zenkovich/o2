#include "AnimatedVec2.h"

#include "Utils/Math/Interpolation.h"

namespace o2
{
	AnimatedValue<Vec2F>::AnimatedValue():
		mTarget(nullptr), mTargetProperty(nullptr)
	{
		InitializeProperties();
	}

	AnimatedValue<Vec2F>::AnimatedValue(const AnimatedValue<Vec2F>& other):
		mKeys(other.mKeys), mValue(other.mValue), mTarget(other.mTarget), mTargetDelegate(other.mTargetDelegate),
		mTargetProperty(other.mTargetProperty), IAnimatedValue(other)
	{
		InitializeProperties();
	}

	AnimatedValue<Vec2F>& AnimatedValue<Vec2F>::operator=(const AnimatedValue<Vec2F>& other)
	{
		IAnimation::operator =(other);

		mTarget = other.mTarget;
		mTargetProperty = other.mTargetProperty;
		mTargetDelegate = other.mTargetDelegate;
		mKeys = other.mKeys;
		mValue = other.mValue;

		return *this;
	}

	void AnimatedValue<Vec2F>::SetTarget(Vec2F* value)
	{
		mTargetProperty = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	void AnimatedValue<Vec2F>::SetTarget(Vec2F* value, const Function<void()>& changeEvent)
	{
		mTargetProperty = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	void AnimatedValue<Vec2F>::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	void AnimatedValue<Vec2F>::SetTargetProperty(Setter<Vec2F>* setter)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProperty = setter;
	}

	Vec2F AnimatedValue<Vec2F>::GetValue()
	{
		return mValue;
	}

	void AnimatedValue<Vec2F>::AddKeys(Vector<Key> keys, float smooth /*= 1.0f*/)
	{
		for (auto key : keys)
			AddKey(key, smooth);

		for (auto key : keys)
			SmoothKey(key.position, smooth);

		UpdateApproximation();
	}

	void AnimatedValue<Vec2F>::AddKey(const Key& key)
	{
		int pos = mKeys.Count();
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (mKeys[i].position > key.position)
			{
				pos = i;
				break;
			}
		}

		pos = Math::Clamp(pos, 0, mKeys.Count());
		mKeys.Insert(key, pos);

		UpdateApproximation();
	}

	void AnimatedValue<Vec2F>::AddKey(const Key& key, float smooth)
	{
		int pos = mKeys.Count();
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (mKeys[i].position > key.position)
			{
				pos = i;
				break;
			}
		}

		pos = Math::Clamp(pos, 0, mKeys.Count());
		mKeys.Insert(key, pos);

		SmoothKey(key.position, smooth);
	}

	void AnimatedValue<Vec2F>::AddKey(float position, const Vec2F& value,
									  const Vec2F& prevSupport, const Vec2F& nextSupport,
									  float leftCoef, float leftCoefPosition, float rightCoef, float rightCoefPosition)
	{
		AddKey(Key(position, value, prevSupport, nextSupport, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition));
	}

	void AnimatedValue<Vec2F>::AddKey(float position, const Vec2F& value, float smooth /*= 1.0f*/)
	{
		AddKey(Key(position, value, value, value, 0.0f, 0.0f, 1.0f, 1.0f), smooth);
	}

	AnimatedValue<Vec2F>::Key AnimatedValue<Vec2F>::GetKey(float position)
	{
		for (auto& key : mKeys)
			if (Math::Equals(key.position, position))
				return key;

		return Key();
	}

	bool AnimatedValue<Vec2F>::RemoveKey(float position)
	{
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (Math::Equals(mKeys[i].position, position))
			{
				mKeys.RemoveAt(i);
				UpdateApproximation();
				return true;
			}
		}

		return false;
	}

	void AnimatedValue<Vec2F>::RemoveAllKeys()
	{
		mKeys.Clear();
	}

	bool AnimatedValue<Vec2F>::ContainsKey(float position)
	{
		for (auto& key : mKeys)
			if (Math::Equals(key.position, position))
				return true;

		return false;
	}

	const AnimatedValue<Vec2F>::KeysVec& AnimatedValue<Vec2F>::GetKeys() const
	{
		return mKeys;
	}

	void AnimatedValue<Vec2F>::SetKeys(const KeysVec& keys)
	{
		mKeys = keys;
		UpdateApproximation();
	}

	void AnimatedValue<Vec2F>::SmoothKey(float position, float smooth)
	{
		int pos = 0;

		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (Math::Equals(mKeys[i].position, position))
			{
				pos = i;
				break;
			}
		}

		if (pos > 0 && pos < mKeys.Count() - 1)
		{
			float baseSmooth = 0.3333f;
			float resSmooth = baseSmooth*smooth;

			Key& prevKey = mKeys[pos - 1];
			Key& curKey  = mKeys[pos];
			Key& nextKey = mKeys[pos + 1];

			Vec2F prev = prevKey.value;
			Vec2F cur = curKey.value;
			Vec2F next = nextKey.value;

			Vec2F prevCur = prev - cur;
			Vec2F nextCur = next - cur;

			Vec2F nd = (prevCur.Normalized() + nextCur.Normalized()).Perpendicular().Normalized();
			if (nd == Vec2F()) nd = prevCur.Normalized();
			nd *= Math::Sign(nd.x);

			curKey.prevSupportValue = cur + nd*resSmooth;
			curKey.nextSupportValue = cur + nd*resSmooth;
		}

		UpdateApproximation();
	}

	AnimatedValue<Vec2F>::Key AnimatedValue<Vec2F>::operator[](float position)
	{
		return GetKey(position);
	}

	void AnimatedValue<Vec2F>::Evaluate()
	{
		if (mKeys.Count() == 0)
			return;

		mValue = Evaluate(mInDurationTime);

		if (mTarget)
		{
			*mTarget = mValue;
			mTargetDelegate();
		}
		else if (mTargetProperty)
			*mTargetProperty = mValue;
	}

	Vec2F AnimatedValue<Vec2F>::Evaluate(float position)
	{
		int count = mKeys.Count();

		if (count == 1)
			return mKeys[0].value;
		else if (count == 0)
			return Vec2F();

		int begi = -1, endi = -1;

		for (int i = 1; i < count; i++)
		{
			begi = i - 1;
			endi = i;

			if (mKeys[i].position > position)
				break;
		}

		if (begi < 0)
			return Vec2F();

		Key& beginKey = mKeys[begi];
		Key& endKey = mKeys[endi];

		float curveCoef = 0.0f;
		{
			int segBeg = 0;
			int segEnd = 1;

			for (int i = 0; i < Key::mApproxValuesCount - 1; i++)
			{
				segBeg = i;
				segEnd = i + 1;

				if (endKey.mCurveApproxValues[i].x > position)
					break;
			}

			Vec2F begs = endKey.mCurveApproxValues[segBeg];
			Vec2F ends = endKey.mCurveApproxValues[segEnd];

			float dist = ends.x - begs.x;
			float coef = (position - begs.x)/dist;

			curveCoef = Math::Lerp(begs.y, ends.y, coef);
		}

		float keyCoef = (position*curveCoef - beginKey.position)/(endKey.position - beginKey.position);

		float lengthSum = 0.0f;
		float posLength = endKey.mApproxTotalLength*keyCoef;

		int segBeg = 0;
		int segEnd = 1;
		for (int i = 0; i < Key::mApproxValuesCount - 1; i++)
		{
			segBeg = i;
			segEnd = i + 1;

			lengthSum += endKey.mApproxLengths[i];

			if (lengthSum > posLength)
				break;
		}

		float segCoef = (posLength - endKey.mApproxLengths[segBeg])/endKey.mApproxLengths[segBeg];

		Vec2F begs = endKey.mApproxValues[segBeg];
		Vec2F ends = endKey.mApproxValues[segEnd];

		return Math::Lerp(begs, ends, segCoef);
	}

	AnimatedValue<Vec2F>::KeysVec AnimatedValue<Vec2F>::GetKeysNonContant()
	{
		return mKeys;
	}

	void AnimatedValue<Vec2F>::UpdateApproximation()
	{
		for (int i = 1; i < mKeys.Count(); i++)
		{
			Key& beginKey = mKeys[i - 1];
			Key& endKey = mKeys[i];

			beginKey.curvePrevCoefPos = Math::Clamp01(beginKey.curvePrevCoefPos);
			endKey.curveNextCoefPos = Math::Clamp01(endKey.curveNextCoefPos);

			float dist = endKey.position - beginKey.position;

			Vec2F curvea(beginKey.position, 0.0f);
			Vec2F curveb(Math::Lerp(beginKey.position, endKey.position, beginKey.curveNextCoefPos), beginKey.curveNextCoef);
			Vec2F curvec(Math::Lerp(beginKey.position, endKey.position, endKey.curvePrevCoefPos), endKey.curvePrevCoef);
			Vec2F curved(endKey.position, 1.0f);

			Vec2F lastApproxPoint = beginKey.value;
			float totalLength = 0.0f;
			for (int j = 0; j < Key::mApproxValuesCount; j++)
			{
				float coef = (float)j/(float)(Key::mApproxValuesCount - 1);

				endKey.mCurveApproxValues[j] = Bezier(curvea, curveb, curvec, curved, coef);

				Vec2F approxPoint = Bezier(beginKey.value, beginKey.nextSupportValue, 
										   endKey.prevSupportValue, endKey.value, coef);
				float length = (lastApproxPoint - approxPoint).Length();

				endKey.mApproxValues[j] = approxPoint;
				endKey.mApproxLengths[j] = length;

				totalLength += length;

				lastApproxPoint = approxPoint;
			}

			endKey.mApproxTotalLength = totalLength;
		}

		mDuration = mKeys.Last().position;
	}


	AnimatedValue<Vec2F> AnimatedValue<Vec2F>::Parametric(const Vec2F& begin, const Vec2F& end, float duration,
														  float beginCoef, float beginCoefPosition,
														  float endCoef, float endCoefPosition)
	{
		AnimatedValue<Vec2F> res;
		res.AddKey(0.0f, begin, begin, begin, 0.0f, 0.0f, beginCoef, beginCoefPosition);
		res.AddKey(duration, end, end, end, endCoef, endCoefPosition, 0.0f, 0.0f);
		return res;
	}

	AnimatedValue<Vec2F> AnimatedValue<Vec2F>::EaseIn(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.5f, 1.0f, 1.0f);
	}

	AnimatedValue<Vec2F> AnimatedValue<Vec2F>::EaseOut(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 0.5f);
	}

	AnimatedValue<Vec2F> AnimatedValue<Vec2F>::EaseInOut(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 0.6f);
	}

	AnimatedValue<Vec2F> AnimatedValue<Vec2F>::Linear(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}

	void AnimatedValue<Vec2F>::InitializeProperties()
	{
		INITIALIZE_GETTER(AnimatedValue<Vec2F>, value, GetValue);
		INITIALIZE_SETTER(AnimatedValue<Vec2F>, target, SetTarget);
		INITIALIZE_SETTER(AnimatedValue<Vec2F>, targetDelegate, SetTargetDelegate);
		INITIALIZE_SETTER(AnimatedValue<Vec2F>, targetProperty, SetTargetProperty);
		INITIALIZE_ACCESSOR(AnimatedValue<Vec2F>, key, GetKey);
		INITIALIZE_PROPERTY(AnimatedValue<Vec2F>, keys, SetKeys, GetKeysNonContant);
	}

	AnimatedValue<Vec2F>::Key::Key():
		position(0), curvePrevCoef(1.0f), curvePrevCoefPos(1.0f), curveNextCoef(0.0f), curveNextCoefPos(0.0f),
		mApproxTotalLength(0)
	{}

	AnimatedValue<Vec2F>::Key::Key(float position, const Vec2F& value):
		position(position), curvePrevCoef(1.0f), curvePrevCoefPos(1.0f), curveNextCoef(0.0f), curveNextCoefPos(0.0f),
		value(value), prevSupportValue(value), nextSupportValue(value), mApproxTotalLength(0)
	{}

	AnimatedValue<Vec2F>::Key::Key(float position, const Vec2F& value, 
								   const Vec2F& prevSupportValue, const Vec2F& nextSupportValue,
								   float curvePrevCoef, float curvePrevCoefPos, 
								   float curveNextCoef, float curveNextCoefPos):
		position(position), curvePrevCoef(curvePrevCoef), curvePrevCoefPos(curvePrevCoefPos), 
		curveNextCoef(curveNextCoef), curveNextCoefPos(curveNextCoefPos), value(value),
		prevSupportValue(prevSupportValue), nextSupportValue(nextSupportValue), mApproxTotalLength(0)
	{}

	AnimatedValue<Vec2F>::Key::Key(const Key& other):
		position(other.position), curvePrevCoef(other.curvePrevCoef), curvePrevCoefPos(other.curvePrevCoefPos),
		curveNextCoef(other.curveNextCoef), curveNextCoefPos(other.curveNextCoefPos), value(other.value),
		prevSupportValue(other.prevSupportValue), nextSupportValue(other.nextSupportValue),
		mApproxTotalLength(other.mApproxTotalLength)
	{
		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mApproxLengths, other.mApproxLengths, mApproxValuesCount*sizeof(float));
	}

	AnimatedValue<Vec2F>::Key& AnimatedValue<Vec2F>::Key::operator=(const Key& other)
	{
		position = other.position;
		value = other.value;
		prevSupportValue = other.prevSupportValue;
		nextSupportValue = other.nextSupportValue;
		curvePrevCoef = other.curvePrevCoef;
		curvePrevCoefPos = other.curvePrevCoefPos;
		curveNextCoef = other.curveNextCoef;
		curveNextCoefPos = other.curveNextCoefPos;
		mApproxTotalLength = other.mApproxTotalLength;

		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mApproxLengths, other.mApproxLengths, mApproxValuesCount*sizeof(float));

		return *this;
	}

	bool AnimatedValue<Vec2F>::Key::operator==(const Key& other) const
	{
		return position == other.position && value == other.value;
	}
}