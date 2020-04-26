#include "o2/stdafx.h"
#include "AnimationVec2FTrack.h"

#include "o2/Animation/AnimationState.h"
#include "o2/Scene/Components/AnimationComponent.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Math/Interpolation.h"
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
	AnimationTrack<Vec2F>::AnimationTrack()
	{}

	AnimationTrack<Vec2F>::AnimationTrack(const AnimationTrack<Vec2F>& other) :
		IAnimationTrack(other), mKeys(other.mKeys)
	{}

	AnimationTrack<Vec2F>& AnimationTrack<Vec2F>::operator=(const AnimationTrack<Vec2F>& other)
	{
		IAnimationTrack::operator =(other);
		mKeys = other.mKeys;

		onKeysChanged();

		return *this;
	}

	Vec2F AnimationTrack<Vec2F>::GetValue(float position) const
	{
		int cacheKey = 0, cacheKeyApporx = 0;
		return GetValue(position, true, cacheKey, cacheKeyApporx);
	}

	Vec2F AnimationTrack<Vec2F>::GetValue(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const
	{
		int count = mKeys.Count();

		if (count == 1)
			return mKeys[0].value;
		else if (count == 0)
			return Vec2F();

		int prevCacheKey = cacheKey;
		int keyLeftIdx = -1, rightKeyIdx = -1;
		SearchKey(mKeys, count, position, keyLeftIdx, rightKeyIdx, direction, cacheKey);

		if (keyLeftIdx < 0)
			return Vec2F();

		const Key& leftKey = mKeys[keyLeftIdx];
		const Key& rightKey = mKeys[rightKeyIdx];

		if (Math::Equals(rightKey.mApproxTotalLength, 0.0f, 0.001f))
			return rightKey.value;

		float curveCoef = 0.0f;
		{
			int segLeftIdx = 0;
			int segRightIdx = 1;

			if (keyLeftIdx != prevCacheKey)
				cacheKeyApprox = 0;

			SearchKey(rightKey.mCurveApproxValues, Key::mApproxValuesCount, position, segLeftIdx, segRightIdx, direction, cacheKeyApprox);

			const ApproximationValue& segLeft = rightKey.mCurveApproxValues[segLeftIdx];
			const ApproximationValue& segRight = rightKey.mCurveApproxValues[segRightIdx];

			float dist = segRight.position - segLeft.position;
			float coef = (position - segLeft.position)/dist;

			cacheKey = keyLeftIdx;
			cacheKeyApprox = segLeftIdx;

			curveCoef = Math::Lerp(segLeft.value, segRight.value, coef);
		}

		float keyCoef = Math::Lerp(leftKey.position, rightKey.position, curveCoef);

		float lengthSum = 0.0f;
		float posLength = rightKey.mApproxTotalLength*curveCoef;

		int segBeg = 0;
		int segEnd = 1;
		for (int i = 1; i < Key::mApproxValuesCount; i++)
		{
			segBeg = i - 1;
			segEnd = i;

			lengthSum += rightKey.mApproxLengths[i];

			if (lengthSum > posLength)
				break;
		}

		float segCoef = (posLength - (lengthSum - rightKey.mApproxLengths[segEnd]))/rightKey.mApproxLengths[segEnd];

		Vec2F begs = rightKey.mApproxValues[segBeg];
		Vec2F ends = rightKey.mApproxValues[segEnd];

		return Math::Lerp(begs, ends, segCoef);
	}

	void AnimationTrack<Vec2F>::BeginKeysBatchChange()
	{
		mBatchChange = true;
	}

	void AnimationTrack<Vec2F>::CompleteKeysBatchingChange()
	{
		UpdateApproximation();
		mBatchChange = false;
		mChangedKeys = false;
	}

	float AnimationTrack<Vec2F>::GetDuration() const
	{
		return mKeys.IsEmpty() ? 0.0f : mKeys.Last().position;
	}

	IAnimationTrack::IPlayer* AnimationTrack<Vec2F>::CreatePlayer() const
	{
		return mnew Player();
	}

	void AnimationTrack<Vec2F>::AddKeys(Vector<Key> keys, float smooth /*= 1.0f*/)
	{
		for (auto key : keys)
			AddKey(key, smooth);

		for (auto key : keys)
			SmoothKey(key.position, smooth);

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	int AnimationTrack<Vec2F>::AddKey(const Key& key)
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

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();

		return pos;
	}

	int AnimationTrack<Vec2F>::AddKey(const Key& key, float position)
	{
		Key newkey = key;
		newkey.position = position;
		return AddKey(newkey);
	}

	int AnimationTrack<Vec2F>::AddSmoothKey(const Key& key, float smooth)
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

		return pos;
	}

	int AnimationTrack<Vec2F>::AddKey(float position, const Vec2F& value,
									 const Vec2F& prevSupport, const Vec2F& nextSupport,
									 float leftCoef, float leftCoefPosition, float rightCoef, float rightCoefPosition)
	{
		return AddKey(Key(position, value, prevSupport, nextSupport, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition));
	}

	int AnimationTrack<Vec2F>::AddKey(float position, const Vec2F& value, float smooth /*= 1.0f*/)
	{
		return AddSmoothKey(Key(position, value, value, value, 0.0f, 0.0f, 1.0f, 1.0f), smooth);
	}

	bool AnimationTrack<Vec2F>::RemoveKey(float position)
	{
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (Math::Equals(mKeys[i].position, position))
			{
				mKeys.RemoveAt(i);

				if (mBatchChange)
					mChangedKeys = true;
				else
					UpdateApproximation();

				return true;
			}
		}

		return false;
	}

	bool AnimationTrack<Vec2F>::RemoveKeyAt(int idx)
	{
		if (idx < 0 || idx > mKeys.Count() - 1)
			return false;

		mKeys.RemoveAt(idx);

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();

		return true;
	}

	void AnimationTrack<Vec2F>::RemoveAllKeys()
	{
		mKeys.Clear();
		onKeysChanged();
	}

	bool AnimationTrack<Vec2F>::ContainsKey(float position) const
	{
		for (auto& key : mKeys)
		{
			if (Math::Equals(key.position, position))
				return true;
		}

		return false;
	}

	const Vector<AnimationTrack<Vec2F>::Key>& AnimationTrack<Vec2F>::GetKeys() const
	{
		return mKeys;
	}

	AnimationTrack<Vec2F>::Key AnimationTrack<Vec2F>::GetKey(float position) const
	{
		for (auto& key : mKeys)
		{
			if (Math::Equals(key.position, position))
				return key;
		}

		return Key();
	}

	AnimationTrack<Vec2F>::Key AnimationTrack<Vec2F>::GetKeyAt(int idx) const
	{
		return mKeys[idx];
	}

	AnimationTrack<Vec2F>::Key AnimationTrack<Vec2F>::FindKey(UInt64 uid) const
	{
		for (auto& key : mKeys)
		{
			if (key.uid == uid)
				return key;
		}

		return Key();
	}

	int AnimationTrack<o2::Vec2F>::FindKeyIdx(UInt64 uid) const
	{
		int idx = 0;
		for (auto& key : mKeys)
		{
			if (key.uid == uid)
				return idx;

			idx++;
		}

		return -1;
	}

	void AnimationTrack<Vec2F>::SetKeys(const Vector<Key>& keys)
	{
		mKeys = keys;

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	void AnimationTrack<Vec2F>::SmoothKey(float position, float smooth)
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
			Key& curKey = mKeys[pos];
			Key& nextKey = mKeys[pos + 1];

			Vec2F prev = prevKey.value;
			Vec2F cur = curKey.value;
			Vec2F next = nextKey.value;

			Vec2F prevCur = prev - cur;
			Vec2F nextCur = next - cur;

			Vec2F nd = (prevCur.Normalized() + nextCur.Normalized()).Perpendicular().Normalized();
			if (nd == Vec2F()) nd = prevCur.Normalized();
			nd *= Math::Sign(nd.x);

			curKey.prevSupportValue = cur - nd*resSmooth*prevCur.Length();
			curKey.nextSupportValue = cur + nd*resSmooth*nextCur.Length();
		}

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	AnimationTrack<Vec2F>::Key AnimationTrack<Vec2F>::operator[](float position) const
	{
		return GetKey(position);
	}

	Vector<AnimationTrack<Vec2F>::Key> AnimationTrack<Vec2F>::GetKeysNonContant()
	{
		return mKeys;
	}

	void AnimationTrack<Vec2F>::UpdateApproximation()
	{
		for (int i = 1; i < mKeys.Count(); i++)
		{
			Key& beginKey = mKeys[i - 1];
			Key& endKey = mKeys[i];

			beginKey.leftSupportPosition = Math::Clamp01(beginKey.leftSupportPosition);
			endKey.rightSupportPosition = Math::Clamp01(endKey.rightSupportPosition);

			float dist = endKey.position - beginKey.position;

			Vec2F curvea(beginKey.position, 0.0f);
			Vec2F curveb(Math::Lerp(beginKey.position, endKey.position, beginKey.rightSupportPosition), beginKey.rightSupportValue);
			Vec2F curvec(Math::Lerp(beginKey.position, endKey.position, endKey.leftSupportPosition), endKey.leftSupportValue);
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

		onKeysChanged();
	}

	void AnimationTrack<Vec2F>::OnDeserialized(const DataNode& node)
	{
		UpdateApproximation();
	}

	AnimationTrack<Vec2F> AnimationTrack<Vec2F>::Parametric(const Vec2F& begin, const Vec2F& end, float duration,
														  float beginCoef, float beginCoefPosition,
														  float endCoef, float endCoefPosition)
	{
		AnimationTrack<Vec2F> res;
		res.AddKey(0.0f, begin, begin, begin, 0.0f, 0.0f, beginCoef, beginCoefPosition*duration);
		res.AddKey(duration, end, end, end, 1.0f - endCoef, -endCoefPosition*duration, 0.0f, 0.0f);
		return res;
	}

	AnimationTrack<Vec2F> AnimationTrack<Vec2F>::EaseIn(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.5f, 1.0f, 1.0f);
	}

	AnimationTrack<Vec2F> AnimationTrack<Vec2F>::EaseOut(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 0.5f);
	}

	AnimationTrack<Vec2F> AnimationTrack<Vec2F>::EaseInOut(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 1.0f);
	}

	AnimationTrack<Vec2F> AnimationTrack<Vec2F>::Linear(const Vec2F& begin, const Vec2F& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}

	AnimationTrack<Vec2F>::Key::Key() :
		uid(Math::Random()), position(0), leftSupportValue(1.0f), leftSupportPosition(1.0f), rightSupportValue(0.0f), rightSupportPosition(0.0f),
		mApproxTotalLength(0)
	{}

	AnimationTrack<Vec2F>::Key::Key(const Vec2F& value) :
		uid(Math::Random()), position(0), leftSupportValue(1.0f), leftSupportPosition(1.0f), rightSupportValue(0.0f), rightSupportPosition(0.0f),
		mApproxTotalLength(0), value(value)
	{}

	AnimationTrack<Vec2F>::Key& AnimationTrack<Vec2F>::Key::operator=(const Vec2F& value)
	{
		this->value = value;
		return *this;
	}

	AnimationTrack<Vec2F>::Key::operator Vec2F() const
	{
		return value;
	}

	AnimationTrack<Vec2F>::Key::Key(float position, const Vec2F& value) :
		uid(Math::Random()), position(position), leftSupportValue(1.0f), leftSupportPosition(1.0f), rightSupportValue(0.0f), rightSupportPosition(0.0f),
		value(value), prevSupportValue(value), nextSupportValue(value), mApproxTotalLength(0)
	{}

	AnimationTrack<Vec2F>::Key::Key(float position, const Vec2F& value,
								   const Vec2F& prevSupportValue, const Vec2F& nextSupportValue,
								   float leftSupportValue, float leftSupportPosition,
								   float rightSupportValue, float rightSupportPosition) :
		uid(Math::Random()), position(position), leftSupportValue(leftSupportValue), leftSupportPosition(leftSupportPosition),
		rightSupportValue(rightSupportValue), rightSupportPosition(rightSupportPosition), value(value),
		prevSupportValue(prevSupportValue), nextSupportValue(nextSupportValue), mApproxTotalLength(0)
	{}

	AnimationTrack<Vec2F>::Key::Key(const Key& other) :
		uid(other.uid), position(other.position), leftSupportValue(other.leftSupportValue), leftSupportPosition(other.leftSupportPosition),
		rightSupportValue(other.rightSupportValue), rightSupportPosition(other.rightSupportPosition), value(other.value),
		prevSupportValue(other.prevSupportValue), nextSupportValue(other.nextSupportValue),
		mApproxTotalLength(other.mApproxTotalLength)
	{
		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mApproxLengths, other.mApproxLengths, mApproxValuesCount*sizeof(float));
	}

	AnimationTrack<Vec2F>::Key& AnimationTrack<Vec2F>::Key::operator=(const Key& other)
	{
		uid = other.uid;
		position = other.position;
		value = other.value;
		prevSupportValue = other.prevSupportValue;
		nextSupportValue = other.nextSupportValue;
		leftSupportValue = other.leftSupportValue;
		leftSupportPosition = other.leftSupportPosition;
		rightSupportValue = other.rightSupportValue;
		rightSupportPosition = other.rightSupportPosition;
		mApproxTotalLength = other.mApproxTotalLength;

		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));
		memcpy(mApproxLengths, other.mApproxLengths, mApproxValuesCount*sizeof(float));

		return *this;
	}

	bool AnimationTrack<Vec2F>::Key::operator==(const Key& other) const
	{
		return position == other.position && value == other.value;
	}

	AnimationTrack<Vec2F>::Player::Player():
		IPlayer()
	{}

	AnimationTrack<Vec2F>::Player::~Player()
	{
		if (mTargetProxy)
			delete mTargetProxy;
	}

	AnimationTrack<Vec2F>::Player::operator Vec2F() const
	{
		return mCurrentValue;
	}

	void AnimationTrack<Vec2F>::Player::SetTarget(Vec2F* value)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	void AnimationTrack<Vec2F>::Player::SetTarget(Vec2F* value, const Function<void()>& changeEvent)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	void AnimationTrack<Vec2F>::Player::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	void AnimationTrack<Vec2F>::Player::SetTargetProxy(IValueProxy<Vec2F>* proxy)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProxy = proxy;
	}

	void AnimationTrack<Vec2F>::Player::SetTrack(AnimationTrack<Vec2F>* track)
	{
		mTrack = track;
		IPlayer::SetTrack(track);
	}

	void AnimationTrack<Vec2F>::Player::SetTargetVoid(void* target)
	{
		SetTarget((Vec2F*)target);
	}

	void AnimationTrack<Vec2F>::Player::SetTargetVoid(void* target, const Function<void()>& changeEvent)
	{
		SetTarget((Vec2F*)target, changeEvent);
	}

	void AnimationTrack<Vec2F>::Player::SetTargetProxyVoid(void* target)
	{
		SetTargetProxy((IValueProxy<Vec2F>*)target);
	}

	void AnimationTrack<Vec2F>::Player::SetTrack(IAnimationTrack* track)
	{
		SetTrack(dynamic_cast<AnimationTrack<Vec2F>*>(track));
	}

	Vec2F AnimationTrack<Vec2F>::Player::GetValue() const
	{
		return mCurrentValue;
	}

	AnimationTrack<Vec2F>* AnimationTrack<Vec2F>::Player::GetTrackT() const
	{
		return mTrack;
	}

	IAnimationTrack* AnimationTrack<Vec2F>::Player::GetTrack() const
	{
		return mTrack;
	}

	void AnimationTrack<Vec2F>::Player::Evaluate()
	{
		mCurrentValue = mTrack->GetValue(mInDurationTime, mInDurationTime > mPrevInDurationTime, 
										 mPrevKey, mPrevKeyApproximation);

		mPrevInDurationTime = mInDurationTime;

		if (mTarget)
		{
			*mTarget = mCurrentValue;
			mTargetDelegate();
		}
		else if (mTargetProxy)
			mTargetProxy->SetValue(mCurrentValue);
	}

	void AnimationTrack<Vec2F>::Player::RegMixer(AnimationState* state, const String& path)
	{
		state->mOwner->RegTrack<Vec2F>(this, path, state);
	}
}

DECLARE_CLASS(o2::AnimationTrack<o2::Vec2F>);

DECLARE_CLASS(o2::AnimationTrack<o2::Vec2F>::Player);

DECLARE_CLASS(o2::AnimationTrack<o2::Vec2F>::Key);
