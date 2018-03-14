#pragma once

#include "Animation/AnimatedValue.h"

namespace o2
{
	// ------------------------------
	// Animated float value interface
	// ------------------------------
	template<>
	class AnimatedValue<float>: public IAnimatedValue
	{
	public:
		typedef Curve::Key Key;
		typedef Vector<Key> KeysVec;

	public:
		GETTER(AnimatedValue<float>, float, value, GetValue);                               // Current value getter  g
		SETTER(AnimatedValue<float>, float*, target, SetTarget);                            // Bind target setter
		SETTER(AnimatedValue<float>, Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter
		SETTER(AnimatedValue<float>, IValueProxy<float>*, targetProxy, SetTargetProxy);     // Bind proxy setter
		PROPERTY(AnimatedValue<float>, KeysVec, keys, SetKeys, GetKeysNonContant);          // Keys property

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
		void SetTargetDelegate(const Function<void()>& changeEvent);

		// Sets target property pointer
		void SetTargetProxy(IValueProxy<float>* proxy);

		// Returns current value
		float GetValue();

		// Returns value at time
		float GetValue(float time);

		//void Insert(const AnimatedValue<float>& other);

		// Adds key with smoothing
		void AddKeys(Vector<Vec2F> values, float smooth = 1.0f);

		// Adds single key
		void AddKey(const Key& key);

		// Adds key at position
		void AddKey(const Key& key, float position);

		// Adds key
		void AddKey(float position, float value, float leftCoef, float leftCoefPosition,
					float rightCoef, float rightCoefPosition);

		// Adds key at position with value and smoothing
		void AddKey(float position, float value, float smooth = 1.0f);

		// Returns key at position
		Key GetKey(float position);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if animation contains key at position
		bool ContainsKey(float position);

		// Returns keys array
		const KeysVec& GetKeys() const;

		// Sets keys
		void SetKeys(const KeysVec& keys);

		// Smooths key at position
		void SmoothKey(float position, float smooth);

		// Returns key by position
		Key operator[](float position);

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
		float               mValue;		            // Current animation value
		float*              mTarget = nullptr;		// Animation target value pointer
		Function<void()>    mTargetDelegate;        // Animation target value change event
		IValueProxy<float>* mTargetProxy = nullptr; // Animation target proxy pointer

	protected:
		// Evaluates value
		void Evaluate();

		// Returns keys (for property)
		KeysVec GetKeysNonContant();

		// It is called when curve updated keys and calculated duration
		void OnCurveChanged();

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Sets target value pointer
		void SetTargetVoid(void* target);

		// Sets target value pointer and change event
		void SetTargetVoid(void* target, const Function<void()>& changeEvent);

		// Sets target property pointer
		void SetTargetProxyVoid(void* target);

		// Registering this in animatable value agent
		void RegInAnimatable(AnimationState* state, const String& path);
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
	PUBLIC_FUNCTION(void, AddKeys, Vector<Vec2F>, float);
	PUBLIC_FUNCTION(void, AddKey, const Key&);
	PUBLIC_FUNCTION(void, AddKey, const Key&, float);
	PUBLIC_FUNCTION(void, AddKey, float, float, float, float, float, float);
	PUBLIC_FUNCTION(void, AddKey, float, float, float);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(bool, RemoveKey, float);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(bool, ContainsKey, float);
	PUBLIC_FUNCTION(const KeysVec&, GetKeys);
	PUBLIC_FUNCTION(void, SetKeys, const KeysVec&);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(KeysVec, GetKeysNonContant);
	PROTECTED_FUNCTION(void, OnCurveChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PROTECTED_FUNCTION(void, SetTargetProxyVoid, void*);
	PROTECTED_FUNCTION(void, RegInAnimatable, AnimationState*, const String&);
}
END_META;
