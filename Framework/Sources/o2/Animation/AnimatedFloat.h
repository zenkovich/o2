#pragma once

#include "o2/Animation/AnimatedValue.h"

namespace o2
{
	// ------------------------------
	// Animated float value interface
	// ------------------------------
	template<>
	class AnimatedValue<float> : public IAnimatedValue
	{
	public:
		typedef float ValueType;
		typedef Curve::Key Key;

	public:
		PROPERTIES(AnimatedValue<float>);
		GETTER(float, value, GetValue);                               // Current value getter 
		SETTER(float*, target, SetTarget);                            // Bind target setter
		SETTER(Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter
		SETTER(IValueProxy<float>*, targetProxy, SetTargetProxy);     // Bind proxy setter
		PROPERTY(Vector<Key>, keys, SetKeys, GetKeysNonContant);          // Keys property

	public:
		Curve curve; // Animation curve @SERIALIZABLE

	public:
		// Default constructor
		AnimatedValue();

		// Copy-constructor
		AnimatedValue(const AnimatedValue<float>& other);

		// Destructor
		~AnimatedValue();

		// Assign operator
		AnimatedValue<float>& operator=(const AnimatedValue<float>& other);

		// Value type cast operator
		operator float() const;

		// Sets target pointer
		void SetTarget(float* value);

		// Sets target pointer and change event
		void SetTarget(float* value, const Function<void()>& changeEvent);

		// Sets target change event
		void SetTargetDelegate(const Function<void()>& changeEvent) override;

		// Sets target property pointer
		void SetTargetProxy(IValueProxy<float>* proxy);

		// Returns current value
		float GetValue() const;

		// Returns value at time
		float GetValue(float time) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update pproximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange() override;

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange() override;

		//void Insert(const AnimatedValue<float>& other);

		// Adds key with smoothing
		void AddKeys(Vector<Vec2F> values, float smooth = 1.0f);

		// Adds single key
		int AddKey(const Key& key);

		// Adds key at position
		int AddKey(const Key& key, float position);

		// Adds key
		int AddKey(float position, float value, float leftCoef, float leftCoefPosition,
				   float rightCoef, float rightCoefPosition);

		// Adds key at position with value and smoothing
		int AddKey(float position, float value, float smooth = 1.0f);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes key by index
		bool RemoveKeyAt(int idx);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if animation contains key at position
		bool ContainsKey(float position) const;

		// Returns key at position
		Key GetKey(float position) const;

		// Returns key at index
		Key GetKeyAt(int idx) const;

		// Returns key by uid
		Key FindKey(UInt64 uid) const;

		// Returns key index by uid
		int FindKeyIdx(UInt64 uid) const;

		// Returns keys array
		const Vector<Key>& GetKeys() const;

		// Sets keys
		void SetKeys(const Vector<Key>& keys);

		// Smooths key at position
		void SmoothKey(float position, float smooth);

		// Returns key by position
		Key operator[](float position) const;

		// Returns parametric specified animated value
		// Sample: Parametric(someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		// Works like css-bezier curves
		static AnimatedValue<float> Parametric(float begin, float end, float duration,
											   float beginCoef, float beginCoefPosition,
											   float endCoef, float endCoefPosition);

		// Returns tweening animation from begin to end in duration with ease in
		static AnimatedValue<float> EaseIn(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with ease out
		static AnimatedValue<float> EaseOut(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with ease in-out
		static AnimatedValue<float> EaseInOut(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with linear transition
		static AnimatedValue<float> Linear(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);

		SERIALIZABLE(AnimatedValue<float>);

	protected:
		float mValue; // Current animation value

		float*              mTarget = nullptr;		// Animation target value pointer
		Function<void()>    mTargetDelegate;        // Animation target value change event
		IValueProxy<float>* mTargetProxy = nullptr; // Animation target proxy pointer

	protected:
		// Evaluates value
		void Evaluate() override;

		// Returns keys (for property)
		Vector<Key> GetKeysNonContant();

		// It is called when curve updated keys and calculated duration
		void OnCurveChanged();

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
}

CLASS_BASES_META(o2::AnimatedValue<float>)
{
	BASE_CLASS(o2::IAnimatedValue);
}
END_META;
CLASS_FIELDS_META(o2::AnimatedValue<float>)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(target);
	PUBLIC_FIELD(targetDelegate);
	PUBLIC_FIELD(targetProxy);
	PUBLIC_FIELD(keys);
	PUBLIC_FIELD(curve).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mValue);
	PROTECTED_FIELD(mTarget);
	PROTECTED_FIELD(mTargetDelegate);
	PROTECTED_FIELD(mTargetProxy);
}
END_META;
CLASS_METHODS_META(o2::AnimatedValue<float>)
{

	PUBLIC_FUNCTION(void, SetTarget, float*);
	PUBLIC_FUNCTION(void, SetTarget, float*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<float>*);
	PUBLIC_FUNCTION(float, GetValue);
	PUBLIC_FUNCTION(float, GetValue, float);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(void, AddKeys, Vector<Vec2F>, float);
	PUBLIC_FUNCTION(int, AddKey, const Key&);
	PUBLIC_FUNCTION(int, AddKey, const Key&, float);
	PUBLIC_FUNCTION(int, AddKey, float, float, float, float, float, float);
	PUBLIC_FUNCTION(int, AddKey, float, float, float);
	PUBLIC_FUNCTION(bool, RemoveKey, float);
	PUBLIC_FUNCTION(bool, RemoveKeyAt, int);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(bool, ContainsKey, float);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(Key, GetKeyAt, int);
	PUBLIC_FUNCTION(Key, FindKey, UInt64);
	PUBLIC_FUNCTION(int, FindKeyIdx, UInt64);
	PUBLIC_FUNCTION(const Vector<Key>&, GetKeys);
	PUBLIC_FUNCTION(void, SetKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PUBLIC_STATIC_FUNCTION(AnimatedValue<float>, Parametric, float, float, float, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimatedValue<float>, EaseIn, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimatedValue<float>, EaseOut, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimatedValue<float>, EaseInOut, float, float, float);
	PUBLIC_STATIC_FUNCTION(AnimatedValue<float>, Linear, float, float, float);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(Vector<Key>, GetKeysNonContant);
	PROTECTED_FUNCTION(void, OnCurveChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PROTECTED_FUNCTION(void, SetTargetProxyVoid, void*);
	PROTECTED_FUNCTION(void, RegInAnimatable, AnimationState*, const String&);
}
END_META;
