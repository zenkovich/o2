#pragma once

#include "Animation/IAnimation.h"
#include "Utils/Math/Curve.h"
#include "Utils/Math/Interpolation.h"
#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	class AnimationState;

	// ------------------------
	// Animated value interface
	// ------------------------
	class IAnimatedValue: public IAnimation
	{
	public:
		Function<void()> onKeysChanged; // It is called when keys was changed

		// Sets target changing delegate
		virtual void SetTargetDelegate(const Function<void()>& changeEvent) {}

		// It is called when beginning keys batch change. After this call all keys modifications will not be update approximation
		// Used for optimizing many keys change
		virtual void BeginKeysBatchChange() {}

		// It is called when keys batch change completed. Updates approximation
		virtual void CompleteKeysBatchingChange() {}

		SERIALIZABLE(IAnimatedValue);

	protected:
		// Sets target by void pointer
		virtual void SetTargetVoid(void* target) {}

		// Sets target by void pointer and change event
		virtual void SetTargetVoid(void* target, const Function<void()>& changeEvent) {}

		// Sets target property by void pointer
		virtual void SetTargetProxyVoid(void* target) {}

		// Registering this in animatable value agent
		virtual void RegInAnimatable(AnimationState* state, const String& path) {}

		// Force setting time (using in Animation): works same as update, but by hard setting time
		void ForceSetTime(float time, float duration);

		friend class Animation;
		friend class Animatable;
	};

	// -----------------------
	// Template animated value
	// -----------------------
	// Specializations: AnimatedValue<RectF>, AnimatedValue<Color4>, AnimatedValue<bool>
	template<typename _type>
	class AnimatedValue: public IAnimatedValue
	{
	public:
		typedef _type ValueType;

		class Key;
		typedef Vector<Key> KeysVec;

	public:
		PROPERTIES(AnimatedValue<_type>);
		GETTER(_type, value, GetValue);                               // Current value getter
		SETTER(_type*, target, SetTarget);                            // Bind target setter
		SETTER(Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter
		SETTER(IValueProxy<_type>*, targetProxy, SetTargetProxy);     // Bind proxy setter
		PROPERTY(KeysVec, keys, SetKeys, GetKeysNonContant);          // Keys property

	public:
		// Default constructor
		AnimatedValue();

		// Copy-constructor
		AnimatedValue(const AnimatedValue<_type>& other);

		// Destructor
		~AnimatedValue();

		// Assign operator
		AnimatedValue<_type>& operator=(const AnimatedValue<_type>& other);

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

		// Returns current value
		_type GetValue() const;

		// Returns value at time
		_type GetValue(float time) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update pproximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange() override;

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange() override;

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
		const KeysVec& GetKeys() const;

		// Returns key at position
		Key GetKey(float position) const;

		// Returns key by uid
		Key FindKey(UInt64 uid) const;

		// Returns key index by uid
		int FindKeyIdx(UInt64 uid) const;

		// Sets keys
		void SetKeys(const KeysVec& keys);

		// Smooths key at position
		void SmoothKey(float position, float smooth);

		// Returns key by position
		Key operator[](float position) const;

		// Returns parametric specified animated value
		// Sample: Parametric(someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		// Works like css-bezier curves
		static AnimatedValue<_type> Parametric(const _type& begin, const _type& end, float duration,
											   float beginCoef, float beginCoefPosition,
											   float endCoef, float endCoefPosition);

	   // Returns tweening animation from begin to end in duration with ease in
		static AnimatedValue<_type> EaseIn(const _type& begin, const _type& end, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with ease out
		static AnimatedValue<_type> EaseOut(const _type& begin, const _type& end, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with ease in-out
		static AnimatedValue<_type> EaseInOut(const _type& begin, const _type& end, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with linear transition
		static AnimatedValue<_type> Linear(const _type& begin, const _type& end, float duration = 1.0f);

		SERIALIZABLE(AnimatedValue<_type>);

	public:
		// -------------
		// Animation key
		// -------------
		class Key: public ISerializable
		{
		public:
			UInt64 uid;              // Random unique id @SERIALIZABLE
			float  position;         // Position on time line, in seconds @SERIALIZABLE
			_type  value;            // Value @SERIALIZABLE
			float  curvePrevCoef;    // Transition curve coefficient for previous animation segment @SERIALIZABLE
			float  curvePrevCoefPos; // Transition curve coefficient position for previous animation segment (must be in 0...1) @SERIALIZABLE
			float  curveNextCoef;    // Transition curve coefficient for next animation segment @SERIALIZABLE
			float  curveNextCoefPos; // Transition curve coefficient position for next animation segment (must be in 0...1)@SERIALIZABLE

		public:
			// Default constructor
			Key();

			// Constructor from value
			Key(const _type& value);

			// Constructor from position and value
			Key(float position, const _type& value);

			// Constructor
			Key(float position, const _type& value, float curvePrevCoef, float curvePrevCoefPos,
				float curveNextCoef, float curveNextCoefPos);

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
			static const int mApproxValuesCount = 10;     // Transition curve approximation values count
			Vec2F mCurveApproxValues[mApproxValuesCount]; // Transition curve approximation values
		};

	protected:
		bool mBatchChange = false; // It is true when began batch change
		bool mChangedKeys = false; // It is true when some keys changed during batch change

		KeysVec mKeys; // Animation keys @SERIALIZABLE

		_type mValue; // Current animation value

		_type*              mTarget = nullptr;      // Animation target value pointer
		Function<void()>    mTargetDelegate;        // Animation target value change event
		IValueProxy<_type>* mTargetProxy = nullptr; // Animation target proxy pointer

	protected:
		// Evaluates value
		void Evaluate() override;

		// Returns value for specified time
		_type Evaluate(float position) const;

		// Returns keys (for property)
		KeysVec GetKeysNonContant();

		// Updates keys approximation
		void UpdateApproximation();

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node) override;

		// Sets target value pointer
		void SetTargetVoid(void* target) override;

		// Sets target value pointer and change event
		void SetTargetVoid(void* target, const Function<void()>& changeEvent) override;

		// Sets target property pointer
		void SetTargetProxyVoid(void* target) override;

		// Registering this in animatable value agent
		void RegInAnimatable(AnimationState* state, const String& path) override;
	};

	template<typename _type>
	AnimatedValue<_type>::AnimatedValue()
	{}

	template<typename _type>
	AnimatedValue<_type>::AnimatedValue(const AnimatedValue<_type>& other):
		mKeys(other.mKeys), mValue(other.mValue), mTargetDelegate(), IAnimatedValue(other),
		value(this), target(this), targetDelegate(this), targetProxy(this), keys(this)		
	{}

	template<typename _type>
	AnimatedValue<_type>::~AnimatedValue()
	{
		if (mTargetProxy)
			delete mTargetProxy;
	}

	template<typename _type>
	AnimatedValue<_type>& AnimatedValue<_type>::operator=(const AnimatedValue<_type>& other)
	{
		IAnimation::operator=(other);

		mKeys = other.mKeys;
		mValue = other.mValue;

		onKeysChanged();

		return *this;
	}

	template<typename _type>
	AnimatedValue<_type>::operator _type() const
	{
		return mValue;
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTarget(_type* value)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate.Clear();
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTarget(_type* value, const Function<void()>& changeEvent)
	{
		mTargetProxy = nullptr;
		mTarget = value;
		mTargetDelegate = changeEvent;
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTargetDelegate(const Function<void()>& changeEvent)
	{
		mTargetDelegate = changeEvent;
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTargetProxy(IValueProxy<_type>* proxy)
	{
		mTarget = nullptr;
		mTargetDelegate.Clear();
		mTargetProxy = proxy;
	}

	template<typename _type>
	_type AnimatedValue<_type>::GetValue() const
	{
		return mValue;
	}

	template<typename _type>
	_type AnimatedValue<_type>::GetValue(float time) const
	{
		return Evaluate(time);
	}

	template<typename _type>
	void AnimatedValue<_type>::BeginKeysBatchChange()
	{
		mBatchChange = true;
	}

	template<typename _type>
	void AnimatedValue<_type>::CompleteKeysBatchingChange()
	{
		UpdateApproximation();
		mBatchChange = false;
		mChangedKeys = false;
	}

	template<typename _type>
	void AnimatedValue<_type>::AddKeys(Vector<Key> keys, float smooth /*= 1.0f*/)
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
	int AnimatedValue<_type>::AddKey(const Key& key)
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
	int AnimatedValue<_type>::AddKey(const Key& key, float position)
	{
		Key newkey = key;
		newkey.position = position;
		return AddKey(newkey);
	}

	template<typename _type>
	int AnimatedValue<_type>::AddSmoothKey(const Key& key, float smooth)
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
	int AnimatedValue<_type>::AddKey(float position, const _type& value,
									  float leftCoef, float leftCoefPosition,
									  float rightCoef, float rightCoefPosition)
	{
		return AddKey(Key(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition));
	}

	template<typename _type>
	int AnimatedValue<_type>::AddKey(float position, const _type& value, float smooth /*= 1.0f*/)
	{
		return AddSmoothKey(Key(position, value, 0.0f, 0.0f, 1.0f, 1.0f), smooth);
	}

	template<typename _type>
	typename AnimatedValue<_type>::Key AnimatedValue<_type>::GetKey(float position) const
	{
		for (auto& key : mKeys)
		{
			if (Math::Equals(key.position, position))
				return key;
		}

		return Key();
	}

	template<typename _type>
	typename AnimatedValue<_type>::Key AnimatedValue<_type>::FindKey(UInt64 uid) const
	{
		for (auto& key : mKeys)
		{
			if (key.uid == uid)
				return key;
		}

		return Key();
	}

	template<typename _type>
	int AnimatedValue<_type>::FindKeyIdx(UInt64 uid) const
	{
		int idx = 0;
		for (auto& key : mKeys)
		{
			if (key.uid == uid)
				return idx;
		}

		return -1;
	}

	template<typename _type>
	bool AnimatedValue<_type>::RemoveKey(float position)
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
	bool AnimatedValue<_type>::RemoveKeyAt(int idx)
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
	void AnimatedValue<_type>::RemoveAllKeys()
	{
		mKeys.Clear();
		onKeysChanged();
	}

	template<typename _type>
	bool AnimatedValue<_type>::ContainsKey(float position) const
	{
		for (auto& key : mKeys)
		{
			if (Math::Equals(key.position, position))
				return true;
		}

		return false;
	}

	template<typename _type>
	const typename AnimatedValue<_type>::KeysVec& AnimatedValue<_type>::GetKeys() const
	{
		return mKeys;
	}

	template<typename _type>
	void AnimatedValue<_type>::SetKeys(const KeysVec& keys)
	{
		mKeys = keys;

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	template<typename _type>
	void AnimatedValue<_type>::SmoothKey(float position, float smooth)
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

		mKeys[pos].curvePrevCoef = 1.0f;
		mKeys[pos].curvePrevCoefPos = 1.0f - resSmooth;

		mKeys[pos].curveNextCoef = 0.0f;
		mKeys[pos].curveNextCoefPos = resSmooth;

		if (mBatchChange)
			mChangedKeys = true;
		else
			UpdateApproximation();
	}

	template<typename _type>
	typename AnimatedValue<_type>::Key AnimatedValue<_type>::operator[](float position) const
	{
		return GetKey(position);
	}

	template<typename _type>
	void AnimatedValue<_type>::Evaluate()
	{
		if (mKeys.Count() == 0)
			return;

		mValue = Evaluate(mInDurationTime);

		if (mTarget)
		{
			*mTarget = mValue;
			mTargetDelegate();
		}
		else if (mTargetProxy)
			mTargetProxy->SetValue(mValue);
	}

	template<typename _type>
	_type AnimatedValue<_type>::Evaluate(float position) const
	{
		int count = mKeys.Count();

		if (count == 1)
			return mKeys[0].value;
		else if (count == 0)
			return _type();

		int begi = -1, endi = -1;

		for (int i = 1; i < count; i++)
		{
			begi = i - 1;
			endi = i;

			if (mKeys[i].position > position)
				break;
		}

		if (begi < 0)
			return _type();

		const Key& beginKey = mKeys[begi];
		const Key& endKey = mKeys[endi];

		int segBeg = 0;
		int segEnd = 1;

		for (int i = 1; i < Key::mApproxValuesCount; i++)
		{
			segBeg = i - 1;
			segEnd = i;

			if (endKey.mCurveApproxValues[i].x > position)
				break;
		}

		Vec2F begs = endKey.mCurveApproxValues[segBeg];
		Vec2F ends = endKey.mCurveApproxValues[segEnd];

		float dist = ends.x - begs.x;
		float coef = (position - begs.x)/dist;

		float curveCoef = Math::Lerp(begs.y, ends.y, coef);

		return Math::Lerp(beginKey.value, endKey.value, curveCoef);
	}

	template<typename _type>
	typename AnimatedValue<_type>::KeysVec AnimatedValue<_type>::GetKeysNonContant()
	{
		return mKeys;
	}

	template<typename _type>
	void AnimatedValue<_type>::UpdateApproximation()
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

			for (int j = 0; j < Key::mApproxValuesCount; j++)
			{
				float coef = (float)j/(float)(Key::mApproxValuesCount - 1);
				endKey.mCurveApproxValues[j] = Bezier(curvea, curveb, curvec, curved, coef);
			}
		}

		float lastDuration = mDuration;
		mDuration = mKeys.Last().position;

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;

		onKeysChanged();
	}

	template<typename _type>
	void AnimatedValue<_type>::OnDeserialized(const DataNode& node)
	{
		UpdateApproximation();
		mEndTime = mDuration;
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTargetVoid(void* target)
	{
		SetTarget((_type*)target);
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTargetVoid(void* target, const Function<void()>& changeEvent)
	{
		SetTarget((_type*)target, changeEvent);
	}

	template<typename _type>
	void AnimatedValue<_type>::SetTargetProxyVoid(void* target)
	{
		SetTargetProxy((IValueProxy<_type>*)target);
	}

	template<typename _type>
	AnimatedValue<_type> AnimatedValue<_type>::Parametric(const _type& begin, const _type& end, float duration,
														  float beginCoef, float beginCoefPosition,
														  float endCoef, float endCoefPosition)
	{
		AnimatedValue<_type> res;
		res.AddKey(0.0f, begin, 0.0f, 0.0f, Math::Lerp(begin, end, beginCoef), beginCoefPosition*duration);
		res.AddKey(duration*duration, end, Math::Lerp(begin, end, endCoef), endCoefPosition, 0.0f, 0.0f);
		return res;
	}

	template<typename _type>
	AnimatedValue<_type> AnimatedValue<_type>::EaseIn(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.5f, 1.0f, 1.0f);
	}

	template<typename _type>
	AnimatedValue<_type> AnimatedValue<_type>::EaseOut(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 0.5f);
	}

	template<typename _type>
	AnimatedValue<_type> AnimatedValue<_type>::EaseInOut(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 1.0f);
	}

	template<typename _type>
	AnimatedValue<_type> AnimatedValue<_type>::Linear(const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		return Parametric(begin, end, duration, 0.0f, 0.0f, 1.0f, 1.0f);
	}

	template<typename _type>
	AnimatedValue<_type>::Key::Key():
		uid(Math::Random()), position(0), curvePrevCoef(1.0f), curvePrevCoefPos(1.0f), curveNextCoef(0.0f), curveNextCoefPos(0.0f)
	{}

	template<typename _type>
	AnimatedValue<_type>::Key::Key(const _type& value) :
		uid(Math::Random()), position(0), curvePrevCoef(1.0f), curvePrevCoefPos(1.0f), curveNextCoef(0.0f), curveNextCoefPos(0.0f),
		value(value)
	{}

	template<typename _type>
	AnimatedValue<_type>::Key::Key(float position, const _type& value):
		uid(Math::Random()), position(position), curvePrevCoef(1.0f), curvePrevCoefPos(1.0f), curveNextCoef(0.0f), curveNextCoefPos(0.0f),
		value(value)
	{}

	template<typename _type>
	AnimatedValue<_type>::Key::Key(float position, const _type& value,
								   float curvePrevCoef, float curvePrevCoefPos,
								   float curveNextCoef, float curveNextCoefPos):
		uid(Math::Random()), position(position), curvePrevCoef(curvePrevCoef), curvePrevCoefPos(curvePrevCoefPos),
		curveNextCoef(curveNextCoef), curveNextCoefPos(curveNextCoefPos), value(value)
	{}

	template<typename _type>
	AnimatedValue<_type>::Key::Key(const Key& other):
		uid(other.uid), position(other.position), curvePrevCoef(other.curvePrevCoef), curvePrevCoefPos(other.curvePrevCoefPos),
		curveNextCoef(other.curveNextCoef), curveNextCoefPos(other.curveNextCoefPos), value(other.value)
	{
		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));
	}

	template<typename _type>
	typename AnimatedValue<_type>::Key& AnimatedValue<_type>::Key::operator=(const _type& value)
	{
		this->value = value;
		return *this;
	}

	template<typename _type>
	AnimatedValue<_type>::Key::operator _type() const
	{
		return value;
	}

	template<typename _type>
	typename AnimatedValue<_type>::Key& AnimatedValue<_type>::Key::operator=(const Key& other)
	{
		uid = other.uid;
		position = other.position;
		value = other.value;
		curvePrevCoef = other.curvePrevCoef;
		curvePrevCoefPos = other.curvePrevCoefPos;
		curveNextCoef = other.curveNextCoef;
		curveNextCoefPos = other.curveNextCoefPos;

		memcpy(mCurveApproxValues, other.mCurveApproxValues, mApproxValuesCount*sizeof(Vec2F));

		return *this;
	}

	template<typename _type>
	bool AnimatedValue<_type>::Key::operator==(const Key& other) const
	{
		return Math::Equals(position, other.position) && Math::Equals(value, other.value);
	}
};

CLASS_BASES_META(o2::IAnimatedValue)
{
	BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::IAnimatedValue)
{
	PUBLIC_FIELD(onKeysChanged);
}
END_META;
CLASS_METHODS_META(o2::IAnimatedValue)
{

	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PROTECTED_FUNCTION(void, SetTargetProxyVoid, void*);
	PROTECTED_FUNCTION(void, RegInAnimatable, AnimationState*, const String&);
	PROTECTED_FUNCTION(void, ForceSetTime, float, float);
}
END_META;

META_TEMPLATES(typename _type)
CLASS_BASES_META(o2::AnimatedValue<_type>)
{
	BASE_CLASS(o2::IAnimatedValue);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(o2::AnimatedValue<_type>)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(target);
	PUBLIC_FIELD(targetDelegate);
	PUBLIC_FIELD(targetProxy);
	PUBLIC_FIELD(keys);
	PROTECTED_FIELD(mBatchChange);
	PROTECTED_FIELD(mChangedKeys);
	PROTECTED_FIELD(mKeys).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mValue);
	PROTECTED_FIELD(mTarget);
	PROTECTED_FIELD(mTargetDelegate);
	PROTECTED_FIELD(mTargetProxy);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(o2::AnimatedValue<_type>)
{

	PUBLIC_FUNCTION(void, SetTarget, _type*);
	PUBLIC_FUNCTION(void, SetTarget, _type*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<_type>*);
	PUBLIC_FUNCTION(_type, GetValue);
	PUBLIC_FUNCTION(_type, GetValue, float);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
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
	PUBLIC_FUNCTION(const KeysVec&, GetKeys);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(Key, FindKey, UInt64);
	PUBLIC_FUNCTION(void, SetKeys, const KeysVec&);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(_type, Evaluate, float);
	PROTECTED_FUNCTION(KeysVec, GetKeysNonContant);
	PROTECTED_FUNCTION(void, UpdateApproximation);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PROTECTED_FUNCTION(void, SetTargetProxyVoid, void*);
	PROTECTED_FUNCTION(void, RegInAnimatable, AnimationState*, const String&);
}
END_META;

META_TEMPLATES(typename _type)
CLASS_BASES_META(o2::AnimatedValue<_type>::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(o2::AnimatedValue<_type>::Key)
{
	PUBLIC_FIELD(uid).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(position).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(value).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curvePrevCoef).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curvePrevCoefPos).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curveNextCoef).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curveNextCoefPos).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mCurveApproxValues);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(o2::AnimatedValue<_type>::Key)
{
}
END_META;
