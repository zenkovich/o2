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
		Curve                    curve;          // Animation curve
		Getter<float>            value;			 // Current value getter
		Setter<float*>           target;		 // Bind target setter
		Setter<Function<void()>> targetDelegate; // Bind target change event setter
		Setter<Setter<float>*>   targetProperty; // Bind property setter
		Accessor<Key, float>     key;			 // Animation keys accessor
		Property<KeysVec>        keys;			 // Keys property

		// Default constructor
		AnimatedValue();

		// Copy-constructor
		AnimatedValue(const AnimatedValue<float>& other);

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
		void SetTargetProperty(Setter<float>* setter);

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

		SERIALIZABLE_IMPL(AnimatedValue<float>);

		IOBJECT(AnimatedValue<float>)
		{
			BASE_CLASS(IAnimatedValue);
			SRLZ_FIELD(curve);
		}

	protected:
		float            mValue;		  // Current animation value
		float*           mTarget;		  // Animation target value pointer
		Function<void()> mTargetDelegate; // Animation target value change event
		Setter<float>*   mTargetProperty; // Animation target property pointer

	protected:
		// Evaluates value
		void Evaluate();

		// Returns keys (for property)
		KeysVec GetKeysNonContant();

		// Calls when curve updated keys and calculated duration
		void OnCurveChanged();

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Sets target value pointer
		void SetTargetVoid(void* target);

		// Sets target value pointer and change event
		void SetTargetVoid(void* target, const Function<void()>& changeEvent);

		// Sets target property pointer
		void SetTargetPropertyVoid(void* target);

		// Initializes properties
		void InitializeProperties();
	};
}