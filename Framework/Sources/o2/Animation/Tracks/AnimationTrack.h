#pragma once

#include "o2/Animation/AnimationState.h"
#include "o2/Animation/IAnimation.h"
#include "o2/Animation/Tracks/IAnimationTrack.h"
#include "o2/Utils/Math/Curve.h"
#include "o2/Utils/Math/Interpolation.h"
#include "o2/Utils/Memory/MemoryManager.h"
#include "o2/Utils/Tools/KeySearch.h"

namespace o2
{
	// ------------------------
	// Template Animation track
	// ------------------------
	template<typename _type>
	class AnimationTrack: public IAnimationTrack
	{
	public:
		typedef _type ValueType;
		class Key;

	public:
		PROPERTIES(AnimationTrack<_type>);
		PROPERTY(Vector<Key>, keys, SetKeys, GetKeysNonContant); // Keys property

	public:
		// Default constructor
		AnimationTrack();

		// Copy-constructor
		AnimationTrack(const AnimationTrack<_type>& other);

		// Assign operator
		AnimationTrack<_type>& operator=(const AnimationTrack<_type>& other);

		// It is called when beginning keys batch change. After this call all keys modifications will not be update pproximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange() override;

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange() override;

		// Returns track duration
		float GetDuration() const override;

		// Creates track-type specific player
		IPlayer* CreatePlayer() const override;

		// Adds key with smoothing
		void AddKeys(Vector<Key> keys, float smooth = 1.0f);

		// Adds single key
		int AddKey(const Key& key);

		// Adds key at position
		int AddKey(const Key& key, float position);

		// Adds and smooths key
		int AddSmoothKey(const Key& key, float smooth);

		// Adds key
		int AddKey(float position, const _type& value, float leftCoef, float leftCoefPosition,
				   float rightCoef, float rightCoefPosition);

		// Adds key at position with value and smoothing
		int AddKey(float position, const _type& value, float smooth = 1.0f);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes key by index
		bool RemoveKeyAt(int idx);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if animation contains key at position
		bool ContainsKey(float position) const;

		// Returns keys array
		const Vector<Key>& GetKeys() const;

		// Returns key at position
		Key GetKey(float position) const;

		// Returns key at index
		Key GetKeyAt(int idx) const;

		// Returns key by uid
		Key FindKey(UInt64 uid) const;

		// Returns key index by uid
		int FindKeyIdx(UInt64 uid) const;

		// Sets keys
		void SetKeys(const Vector<Key>& keys);

		// Smooths key at position
		void SmoothKey(float position, float smooth);

		// Returns key by position
		Key operator[](float position) const;

		// Returns parametric specified Animation track
		// Sample: Parametric(someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		static AnimationTrack<_type> Parametric(const _type& begin, const _type& end, float duration,
												float beginCoef, float beginCoefPosition,
												float endCoef, float endCoefPosition);

		// Returns tween animation from begin to end in duration with ease in
		static AnimationTrack<_type> EaseIn(const _type& begin, const _type& end, float duration = 1.0f);

		// Returns tween animation from begin to end in duration with ease out
		static AnimationTrack<_type> EaseOut(const _type& begin, const _type& end, float duration = 1.0f);

		// Returns tween animation from begin to end in duration with ease in-out
		static AnimationTrack<_type> EaseInOut(const _type& begin, const _type& end, float duration = 1.0f);

		// Returns tween animation from begin to end in duration with linear transition
		static AnimationTrack<_type> Linear(const _type& begin, const _type& end, float duration = 1.0f);

		SERIALIZABLE(AnimationTrack<_type>);

	public:
		// ----------------------
		// Animation track player
		// ----------------------
		class Player: public IPlayer
		{
		public:
			PROPERTIES(Player);
			GETTER(_type, value, GetValue);                               // Current value getter
			SETTER(_type*, target, SetTarget);                            // Bind target setter
			SETTER(Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter
			SETTER(IValueProxy<_type>*, targetProxy, SetTargetProxy);     // Bind proxy setter

		public:
			// Default constructor
			Player();

			// Destructor
			~Player();

			// Value type cast operator
			operator _type() const;

			// Sets target pointer
			void SetTarget(_type* value);

			// Sets target pointer and change event
			void SetTarget(_type* value, const Function<void()>& changeEvent);

			// Sets target change event
			void SetTargetDelegate(const Function<void()>& changeEvent) override;

			// Sets target property pointer
			void SetTargetProxy(IValueProxy<_type>* proxy);

			// Sets animation track
			void SetTrack(AnimationTrack<_type>* track);

			// Returns animation track
			AnimationTrack<_type>* GetTrackT() const;

			// Sets target by void pointer
			void SetTargetVoid(void* target) override;

			// Sets target by void pointer and change event
			void SetTargetVoid(void* target, const Function<void()>& changeEvent) override;

			// Sets target property by void pointer
			void SetTargetProxyVoid(void* target) override;

			// Sets animation track
			void SetTrack(IAnimationTrack* track) override;

			// Returns animation track
			IAnimationTrack* GetTrack() const override;

			// Returns current value
			_type GetValue() const;

			// Returns value at time
			_type GetValue(float time) const;

			// Returns value at time, with cached state
			_type GetValue(float time, bool direction, int& cacheKey, int& cacheKeyApprox) const;

			IOBJECT(Player);

		protected:
			AnimationTrack<_type>* mTrack = nullptr; // Animation track

			_type mCurrentValue; // Current animation track

			float mPrevInDurationTime = 0.0f; // Previous evaluation in duration time
			int   mPrevKey = 0;               // Previous evaluation key index
			int   mPrevKeyApproximation = 0;  // Previous evaluation key approximation index

			_type* mTarget = nullptr;      // Animation target value pointer
			Function<void()>    mTargetDelegate;        // Animation target value change event
			IValueProxy<_type>* mTargetProxy = nullptr; // Animation target proxy pointer

		protected:
			// Evaluates value
			void Evaluate() override;

			// Returns value for specified time
			_type Evaluate(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const;

			// Registering this in animatable value agent
			void RegMixer(AnimationState* state, const String& path) override;
		};

		// -------------
		// Animation key
		// -------------
		class Key: public ISerializable
		{
		public:
			UInt64 uid;                  // Random unique id @SERIALIZABLE
			float  position;             // Position on time line, in seconds @SERIALIZABLE
			_type  value;                // Value @SERIALIZABLE
			float  leftSupportValue;     // Transition curve coefficient for previous animation segment @SERIALIZABLE
			float  leftSupportPosition;  // Transition curve coefficient position for previous animation segment (must be in 0...1) @SERIALIZABLE
			float  rightSupportValue;    // Transition curve coefficient for next animation segment @SERIALIZABLE
			float  rightSupportPosition; // Transition curve coefficient position for next animation segment (must be in 0...1)@SERIALIZABLE

		public:
			// Default constructor
			Key();

			// Constructor from value
			Key(const _type& value);

			// Constructor from position and value
			Key(float position, const _type& value);

			// Constructor
			Key(float position, const _type& value, float leftSupportValue, float leftSupportPosition,
				float rightSupportValue, float rightSupportPosition);

			// Copy-constructor
			Key(const Key& other);

			// Assign operator from other key
			Key& operator=(const Key& other);

			// Assign operator from value
			Key& operator=(const _type& value);

			// Value type cast operator
			operator _type() const;

			// Equals operator
			bool operator==(const Key& other) const;

			SERIALIZABLE(Key);

		public:
			static const int   mApproxValuesCount = 10;                // Transition curve approximation values count
			ApproximationValue mCurveApproxValues[mApproxValuesCount]; // Transition curve approximation values
		};

	protected:
		bool mBatchChange = false; // It is true when began batch change
		bool mChangedKeys = false; // It is true when some keys changed during batch change

		Vector<Key> mKeys; // Animation keys @SERIALIZABLE

	protected:
		// Returns keys (for property)
		Vector<Key> GetKeysNonContant();

		// Updates keys approximation
		void UpdateApproximation();

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node) override;
	};
}

#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
	// ------------------------------
	// Animation track implementation 
	// ------------------------------

	template<typename _type>
	AnimationTrack<_type>::AnimationTrack()
	{}

	template<typename _type>
	AnimationTrack<_type>::AnimationTrack(const AnimationTrack<_type>& other):
		IAnimationTrack(other), mKeys(other.mKeys), keys(this)
	{}

	template<typename _type>
	AnimationTrack<_type>& AnimationTrack<_type>::operator=(const AnimationTrack<_type>& other)
	{
		IAnimationTrack::operator=(other);
		mKeys = other.mKeys;

		onKeysChanged();

		return *this;
	}

	template<typename _type>
	void AnimationTrack<_type>::BeginKeysBatchChange()
	{
		mBatchChange = true;
	}

	template<typename _type>
	void AnimationTrack<_type>::CompleteKeysBatchingChange()
	{
		UpdateApproximation();
		mBatchChange = false;
		mChangedKeys = false;
	}

	template<typename _type>
	float AnimationTrack<_type>::GetDuration() const
	{
		return mKeys.IsEmpty() ? 0.0f : mKeys.Last().position;
	}

	template<typename _type>
	IAnimationTrack::IPlayer* AnimationTrack<_type>::CreatePlayer() const
	{
		return mnew Player();
	}

	template<typename _type>
	void AnimationTrack<_type>::AddKeys(Vector<Key> keys, float smooth /*= 1.0f*/)
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

	template<typename _type>
	int AnimationTrack<_type>::AddKey(const Key& key)
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

	template<typename _type>
	int AnimationTrack<_type>::AddKey(const Key& key, float position)
	{
		Key newkey = key;
		newkey.position = position;
		return AddKey(newkey);
	}

	template<typename _type>
	int AnimationTrack<_type>::AddSmoothKey(const Key& key, float smooth)
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

	template<typename _type>
	int AnimationTrack<_type>::AddKey(float position, const _type& value,
									  float leftCoef, float leftCoefPosition,
									  float rightCoef, float rightCoefPosition)
	{
		return AddKey(Key(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition));
	}

	template<typename _type>
	int AnimationTrack<_type>::AddKey(float position, const _type& value, float smooth /*= 1.0f*/)
	{
		return AddSmoothKey(Key(position, value, 0.0f, 0.0f, 1.0f, 1.0f), smooth);
	}

	template<typename _type>
	typename AnimationTrack<_type>::Key AnimationTrack<_type>::GetKey(float position) const
	{
		for (auto& key : mKeys)
		{
			if (Math::Equals(key.position, position))
				return key;
		}

		return Key();
	}

	template<typename _type>
	typename AnimationTrack<_type>::Key AnimationTrack<_type>::GetKeyAt(int idx) const
	{
		return mKeys[idx];
	}

	template<typename _type>
	typename AnimationTrack<_type>::Key AnimationTrack<_type>::FindKey(UInt64 uid) const
	{
		for (auto& key : mKeys)
		{
			if (key.uid == uid)
				return key;
		}

		return Key();
	}

	template<typename _type>
	int AnimationTrack<_type>::FindKeyIdx(UInt64 uid) const
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

	template<typename _type>
	bool AnimationTrack<_type>::RemoveKey(float position)
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

	template<typename _type>
	bool AnimationTrack<_type>::RemoveKeyAt(int idx)
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


	template<typename _type>
	void AnimationTrack<_type>::RemoveAllKeys()
	{
		mKeys.Clear();
		onKeysChanged();
	}

	template<typename _type>
	bool AnimationTrack<_type>::ContainsKey(float position) const
	{
		for (auto& key : mKeys)
		{
			if (Math::Equals(key.position, position))
				return true;
		}

		return false;
	}

	template<typename _type>
	const Vector<typename AnimationTrack<_type>::Key>& AnimationTrack<_type>::GetKeys() const
	{
		return mKeys;
	}

	template<typename _type>
	void AnimationTrack<_type>::SetKeys(const Vector<Key>& keys)
	{
		mKeys = keys;

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	template<typename _type>
	void AnimationTrack<_type>::SmoothKey(float position, float smooth)
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

		float baseSmooth = 0.4f;
		float resSmooth = baseSmooth*smooth;

		mKeys[pos].leftSupportValue = 1.0f;
		mKeys[pos].leftSupportPosition = 1.0f - resSmooth;

		mKeys[pos].rightSupportValue = 0.0f;
		mKeys[pos].rightSupportPosition = resSmooth;

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	template<typename _type>
	typename AnimationTrack<_type>::Key AnimationTrack<_type>::operator[](float position) const
	{
		return GetKey(position);
	}

	template<typename _type>
	Vector<typename AnimationTrack<_type>::Key> AnimationTrack<_type>::GetKeysNonContant()
	{
		return mKeys;
	}

	template<typename _type>
	void AnimationTrack<_type>::UpdateApproximation()
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

			for (int j = 0; j < Key::mApproxValuesCount; j++)
			{
				float coef = (float)j/(float)(Key::mApproxValuesCount - 1);\
				endKey.mCurveApproxValues[j] = Bezier(curvea, curveb, curvec, curved, coef);
			}
		}

		onKeysChanged();
	}

	template<typename _type>
	void AnimationTrack<_type>::OnDeserialized(const DataNode& node)
	{
		UpdateApproximation();
	}

	template<typename _type>
	AnimationTrack<_type> AnimationTrack<_type>::Parametric(const _type& begin, const _type& end, float duration,
															float beginCoef, float beginCoefPosition,
															float endCoef, float endCoefPosition)
	{
		AnimationTrack<_type> res;
		res.AddKey(0.0f, begin, 0.0f, 0.0f, beginCoef, beginCoefPosition*duration);
		res.AddKey(duration*duration, end, endCoef, endCoefPosition, 0.0f, 0.0f);
		return res;
	}

	template<typename _type>
	AnimationTrack<_type> AnimationTrack<_type>::EaseIn(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.5f, 1.0f, 1.0f);
	}

	template<typename _type>
	AnimationTrack<_type> AnimationTrack<_type>::EaseOut(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 0.5f);
	}

	template<typename _type>
	AnimationTrack<_type> AnimationTrack<_type>::EaseInOut(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 1.0f);
	}

	template<typename _type>
	AnimationTrack<_type> AnimationTrack<_type>::Linear(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}

	// ----------------------------------
	// Animation track key implementation
	// ----------------------------------

	template<typename _type>
	AnimationTrack<_type>::Key::Key():
		uid(Math::Random()), position(0), leftSupportValue(1.0f), leftSupportPosition(1.0f), rightSupportValue(0.0f), rightSupportPosition(0.0f)
	{}

	template<typename _type>
	AnimationTrack<_type>::Key::Key(const _type& value) :
		uid(Math::Random()), position(0), leftSupportValue(1.0f), leftSupportPosition(1.0f), rightSupportValue(0.0f), rightSupportPosition(0.0f),
		value(value)
	{}

	template<typename _type>
	AnimationTrack<_type>::Key::Key(float position, const _type& value):
		uid(Math::Random()), position(position), leftSupportValue(1.0f), leftSupportPosition(1.0f), rightSupportValue(0.0f), rightSupportPosition(0.0f),
		value(value)
	{}

	template<typename _type>
	AnimationTrack<_type>::Key::Key(float position, const _type& value,
									float leftSupportValue, float leftSupportPosition,
									float rightSupportValue, float rightSupportPosition):
		uid(Math::Random()), position(position), leftSupportValue(leftSupportValue), leftSupportPosition(leftSupportPosition),
		rightSupportValue(rightSupportValue), rightSupportPosition(rightSupportPosition), value(value)
	{}

	template<typename _type>
	AnimationTrack<_type>::Key::Key(const Key& other):
		uid(other.uid), position(other.position), leftSupportValue(other.leftSupportValue), leftSupportPosition(other.leftSupportPosition),
		rightSupportValue(other.rightSupportValue), rightSupportPosition(other.rightSupportPosition), value(other.value)
	{
		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));
	}

	template<typename _type>
	typename AnimationTrack<_type>::Key& AnimationTrack<_type>::Key::operator=(const _type& value)
	{
		this->value = value;
		return *this;
	}

	template<typename _type>
	AnimationTrack<_type>::Key::operator _type() const
	{
		return value;
	}

	template<typename _type>
	typename AnimationTrack<_type>::Key& AnimationTrack<_type>::Key::operator=(const Key& other)
	{
		uid = other.uid;
		position = other.position;
		value = other.value;
		leftSupportValue = other.leftSupportValue;
		leftSupportPosition = other.leftSupportPosition;
		rightSupportValue = other.rightSupportValue;
		rightSupportPosition = other.rightSupportPosition;

		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));

		return *this;
	}

	template<typename _type>
	bool AnimationTrack<_type>::Key::operator==(const Key& other) const
	{
		return Math::Equals(position, other.position) && Math::Equals(value, other.value);
	}

	// ---------------------
	// Player implementation
	// ---------------------

	template<typename _type>
	AnimationTrack<_type>::Player::Player()
	{}

	template<typename _type>
	AnimationTrack<_type>::Player::~Player()
	{
		if (mTargetProxy)
			delete mTargetProxy;
	}

	template<typename _type>
	AnimationTrack<_type>::Player::operator _type() const
	{
		return mCurrentValue;
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTarget(_type* value)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTarget(_type* value, const Function<void()>& changeEvent)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTargetProxy(IValueProxy<_type>* proxy)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProxy = proxy;
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTargetVoid(void* target)
	{
		SetTarget((_type*)target);
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTargetVoid(void* target, const Function<void()>& changeEvent)
	{
		SetTarget((_type*)target, changeEvent);
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTargetProxyVoid(void* target)
	{
		SetTargetProxy((IValueProxy<_type>*)target);
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTrack(AnimationTrack<_type>* track)
	{
		mTrack = track;
		IPlayer::SetTrack(track);
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::SetTrack(IAnimationTrack* track)
	{
		SetTrack(dynamic_cast<AnimationTrack<_type>*>(track));
	}

	template<typename _type>
	IAnimationTrack* AnimationTrack<_type>::Player::GetTrack() const
	{
		return mTrack;
	}

	template<typename _type>
	AnimationTrack<_type>* AnimationTrack<_type>::Player::GetTrackT() const
	{
		return mTrack;
	}

	template<typename _type>
	_type AnimationTrack<_type>::Player::GetValue() const
	{
		return mCurrentValue;
	}

	template<typename _type>
	_type AnimationTrack<_type>::Player::GetValue(float time) const
	{
		int cacheKey = 0, cacheKeyApprox = 0;
		return Evaluate(time, true, cacheKey, cacheKeyApprox);
	}

	template<typename _type>
	_type AnimationTrack<_type>::Player::GetValue(float time, bool direction, int& cacheKey, int& cacheKeyApprox) const
	{
		return Evaluate(time, direction, cacheKey, cacheKeyApprox);
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::Evaluate() 
	{
		mCurrentValue = Evaluate(mInDurationTime, mInDurationTime > mPrevInDurationTime, mPrevKey, mPrevKeyApproximation);
		mPrevInDurationTime = mInDurationTime;

		if (mTarget)
		{
			*mTarget = mCurrentValue;
			mTargetDelegate();
		}
		else if (mTargetProxy)
			mTargetProxy->SetValue(mCurrentValue);
	}

	template<typename _type>
	_type AnimationTrack<_type>::Player::Evaluate(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const
	{
		if (!mTrack)
			return _type();

		int count = mTrack->mKeys.Count();

		if (count == 1)
			return mTrack->mKeys[0].value;
		else if (count == 0)
			return _type();

		int prevCacheKey = cacheKey;
		int keyLeftIdx = -1, keyRightIdx = -1;
		SearchKey(mTrack->mKeys, count, position, keyLeftIdx, keyRightIdx, direction, cacheKey);

		if (keyLeftIdx < 0)
			return _type();

		const Key& leftKey = mTrack->mKeys[keyLeftIdx];
		const Key& rightKey = mTrack->mKeys[keyRightIdx];

		int segLeftIdx = 0;
		int segRightIdx = 1;

		if (keyLeftIdx != prevCacheKey)
			cacheKeyApprox = 0;

		SearchKey(rightKey.mCurveApproxValues, Key::mApproxValuesCount, position, segLeftIdx, segRightIdx, direction, cacheKeyApprox);

		const ApproximationValue& segLeft = rightKey.mCurveApproxValues[segLeftIdx];
		const ApproximationValue& segRight = rightKey.mCurveApproxValues[segRightIdx];

		float dist = segRight.position - segLeft.position;
		float coef = (position - segLeft.position) / dist;
		float curveCoef = Math::Lerp(segLeft.value, segRight.value, coef);

		return Math::Lerp(leftKey.value, rightKey.value, curveCoef);
	}

	template<typename _type>
	void AnimationTrack<_type>::Player::RegMixer(AnimationState* state, const String& path)
	{
		state->mOwner->RegTrack<_type>(this, path, state);
	}
};

META_TEMPLATES(typename _type)
CLASS_BASES_META(o2::AnimationTrack<_type>)
{
	BASE_CLASS(o2::IAnimationTrack);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(o2::AnimationTrack<_type>)
{
	PUBLIC_FIELD(keys);
	PROTECTED_FIELD(mBatchChange);
	PROTECTED_FIELD(mChangedKeys);
	PROTECTED_FIELD(mKeys).SERIALIZABLE_ATTRIBUTE();
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(o2::AnimationTrack<_type>)
{

	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(IPlayer*, CreatePlayer);
	PUBLIC_FUNCTION(void, AddKeys, Vector<Key>, float);
	PUBLIC_FUNCTION(int, AddKey, const Key&);
	PUBLIC_FUNCTION(int, AddKey, const Key&, float);
	PUBLIC_FUNCTION(int, AddSmoothKey, const Key&, float);
	PUBLIC_FUNCTION(int, AddKey, float, const _type&, float, float, float, float);
	PUBLIC_FUNCTION(int, AddKey, float, const _type&, float);
	PUBLIC_FUNCTION(bool, RemoveKey, float);
	PUBLIC_FUNCTION(bool, RemoveKeyAt, int);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(bool, ContainsKey, float);
	PUBLIC_FUNCTION(const Vector<Key>&, GetKeys);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(Key, GetKeyAt, int);
	PUBLIC_FUNCTION(Key, FindKey, UInt64);
	PUBLIC_FUNCTION(int, FindKeyIdx, UInt64);
	PUBLIC_FUNCTION(void, SetKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<_type>, Parametric, const _type&, const _type&, float, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<_type>, EaseIn, const _type&, const _type&, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<_type>, EaseOut, const _type&, const _type&, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<_type>, EaseInOut, const _type&, const _type&, float);
	PUBLIC_STATIC_FUNCTION(AnimationTrack<_type>, Linear, const _type&, const _type&, float);
	PROTECTED_FUNCTION(Vector<Key>, GetKeysNonContant);
	PROTECTED_FUNCTION(void, UpdateApproximation);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;

META_TEMPLATES(typename _type)
CLASS_BASES_META(o2::AnimationTrack<_type>::Player)
{
	BASE_CLASS(o2::IAnimationTrack::IPlayer);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(o2::AnimationTrack<_type>::Player)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(target);
	PUBLIC_FIELD(targetDelegate);
	PUBLIC_FIELD(targetProxy);
	PROTECTED_FIELD(mTrack);
	PROTECTED_FIELD(mCurrentValue);
	PROTECTED_FIELD(mPrevInDurationTime);
	PROTECTED_FIELD(mPrevKey);
	PROTECTED_FIELD(mPrevKeyApproximation);
	PROTECTED_FIELD(mTarget);
	PROTECTED_FIELD(mTargetDelegate);
	PROTECTED_FIELD(mTargetProxy);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(o2::AnimationTrack<_type>::Player)
{

	PUBLIC_FUNCTION(void, SetTarget, _type*);
	PUBLIC_FUNCTION(void, SetTarget, _type*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<_type>*);
	PUBLIC_FUNCTION(void, SetTrack, AnimationTrack<_type>*);
	PUBLIC_FUNCTION(AnimationTrack<_type>*, GetTrackT);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxyVoid, void*);
	PUBLIC_FUNCTION(void, SetTrack, IAnimationTrack*);
	PUBLIC_FUNCTION(IAnimationTrack*, GetTrack);
	PUBLIC_FUNCTION(_type, GetValue);
	PUBLIC_FUNCTION(_type, GetValue, float);
	PUBLIC_FUNCTION(_type, GetValue, float, bool, int&, int&);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(_type, Evaluate, float, bool, int&, int&);
	PROTECTED_FUNCTION(void, RegMixer, AnimationState*, const String&);
}
END_META;

META_TEMPLATES(typename _type)
CLASS_BASES_META(o2::AnimationTrack<_type>::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(o2::AnimationTrack<_type>::Key)
{
	PUBLIC_FIELD(uid).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(position).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(value).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(leftSupportValue).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(leftSupportPosition).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(rightSupportValue).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(rightSupportPosition).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mCurveApproxValues);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(o2::AnimationTrack<_type>::Key)
{
}
END_META;
