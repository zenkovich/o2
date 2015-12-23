#pragma once

#include "AnimatedValue.h"

namespace o2
{
	// --------------------
	// Animated Vec2F value
	// --------------------
	template<>
	class AnimatedValue<Vec2F>: public IAnimatedValue
	{
	public:
		class Key;
		typedef Vector<Key> KeysVec;

	public:
		Getter<Vec2F>            value;          // Current value getter
		Setter<Vec2F*>           target;         // Bind target setter
		Setter<Function<void()>> targetDelegate; // Bind target change event setter
		Setter<Setter<Vec2F>*>   targetProperty; // Bind property setter
		Accessor<Key, float>     key;			 // Animation keys accessor
		Property<KeysVec>        keys;			 // Keys property

        // Default constructor
		AnimatedValue();

		// Copy-constructor
		AnimatedValue(const AnimatedValue<Vec2F>& other);

		// Assign operator
		AnimatedValue<Vec2F>& operator=(const AnimatedValue<Vec2F>& other);

		// Value type cast operator
		operator Vec2F() const;

		// Sets target pointer
		void SetTarget(Vec2F* value);

		// Sets target pointer and change event
		void SetTarget(Vec2F* value, const Function<void()>& changeEvent);

		// Sets target change event
		void SetTargetDelegate(const Function<void()>& changeEvent);

		// Sets target property pointer
		void SetTargetProperty(Setter<Vec2F>* setter);

		// Returns current value
		Vec2F GetValue();

		// Returns value at time
		Vec2F GetValue(float time);

		// Adds key with smoothing
		void AddKeys(Vector<Key> keys, float smooth = 1.0f);

		// Adds single key
		void AddKey(const Key& key);

		// Adds key at position
		void AddKey(const Key& key, float position);

		// Adds and smooths key
		void AddSmoothKey(const Key& key, float smooth);

		// Adds key
		void AddKey(float position, const Vec2F& value, const Vec2F& prevSupport, const Vec2F& nextSupport,
					float leftCoef, float leftCoefPosition, float rightCoef, float rightCoefPosition);

		// Adds key at position with value and smoothing
		void AddKey(float position, const Vec2F& value, float smooth = 1.0f);

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
		static AnimatedValue<Vec2F> Parametric(const Vec2F& begin, const Vec2F& end, float duration,
											   float beginCoef, float beginCoefPosition,
											   float endCoef, float endCoefPosition);

		// Returns tweening animation from begin to end in duration with ease in
		static AnimatedValue<Vec2F> EaseIn(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with ease out
		static AnimatedValue<Vec2F> EaseOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with ease in-out
		static AnimatedValue<Vec2F> EaseInOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		// Returns tweening animation from begin to end in duration with linear transition
		static AnimatedValue<Vec2F> Linear(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

		SERIALIZABLE(AnimatedValue<Vec2F>);

	public:
		// -------------
		// Animation key
		// -------------
		class Key: public ISerializable
		{
		public:
			float position;			// Position on time line, in seconds @SERIALIZABLE
			Vec2F value;			// Value @SERIALIZABLE
			Vec2F prevSupportValue;	// Previous animation segment support value @SERIALIZABLE
			Vec2F nextSupportValue;	// Next animation segment support value @SERIALIZABLE
			float curvePrevCoef;	// Transition curve coefficient for previous animation segment @SERIALIZABLE
			float curvePrevCoefPos;	// Transition curve coefficient position for previous animation segment (must be in 0...1) @SERIALIZABLE
			float curveNextCoef;	// Transition curve coefficient for next animation segment @SERIALIZABLE
			float curveNextCoefPos;	// Transition curve coefficient position for next animation segment (must be in 0...1) @SERIALIZABLE

		public:
			// Default constructor
			Key();

			// Constructor from value
			Key(const Vec2F& value);

			// Constructor from position and value
			Key(float position, const Vec2F& value);

			// Constructor
			Key(float position, const Vec2F& value, const Vec2F& prevSupportValue, const Vec2F& nextSupportValue, 
				float curvePrevCoef, float curvePrevCoefPos, float curveNextCoef, float curveNextCoefPos);

			// Copy-constructor
			Key(const Key& other);

			// Assign operator from other key
			Key& operator=(const Key& other);

			// Assign operator from value
			Key& operator=(const Vec2F& value);

			// Value type cast operator
			operator Vec2F() const;

			// Equals operator
			bool operator==(const Key& other) const;

			SERIALIZABLE(Key);

		public:
			static const int mApproxValuesCount = 20;     // Transition curve approximation values count
			Vec2F mApproxValues[mApproxValuesCount];      // Approximation values
			Vec2F mCurveApproxValues[mApproxValuesCount]; // Transition curve approximation values
			float mApproxLengths[mApproxValuesCount];     // Approximation segments lengths
			float mApproxTotalLength;                     // Total animation segment length (sum of mApproxLengths)
		};

	protected:
		KeysVec          mKeys;			  // Animation keys @SERIALIZABLE
		Vec2F            mValue;		  // Current animation value
		Vec2F*           mTarget;		  // Animation target value pointer
		Function<void()> mTargetDelegate; // Animation target value change event
		Setter<Vec2F>*   mTargetProperty; // Animation target property pointer

	protected:
		// Evaluates value
		void Evaluate();

		// Returns value for specified time
		Vec2F Evaluate(float position);

		// Returns keys (for property)
		KeysVec GetKeysNonContant();

		// Updates keys approximation
		void UpdateApproximation();

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Sets target value pointer
		void SetTargetVoid(void* target);

		// Sets target value pointer and change event
		void SetTargetVoid(void* target, const Function<void()>& changeEvent);

		// Sets target property pointer
		void SetTargetPropertyVoid(void* target);

		// Registering this in animatable value agent
		void RegInAnimatable(AnimationState* state, const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}
