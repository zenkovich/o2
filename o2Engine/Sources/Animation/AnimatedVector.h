#pragma once

#include "AnimatedValue.h"
#include "Utils/Math/Vector2.h"

namespace o2
{
	// --------------------
	// Animated Vec2F value
	// --------------------
	template<>
	class AnimatedValue<o2::Vec2F>: public IAnimatedValue
	{
	public:
		class Key;
		typedef Vector<Key> KeysVec;

	public:
		GETTER(AnimatedValue<o2::Vec2F>, o2::Vec2F, value, GetValue);                           // Current value getter
		SETTER(AnimatedValue<o2::Vec2F>, o2::Vec2F*, target, SetTarget);                        // Bind target setter
		SETTER(AnimatedValue<o2::Vec2F>, Function<void()>, targetDelegate, SetTargetDelegate);  // Bind target change event setter
		SETTER(AnimatedValue<o2::Vec2F>, IValueProxy<o2::Vec2F>*, targetProxy, SetTargetProxy); // Bind proxy setter
		PROPERTY(AnimatedValue<o2::Vec2F>, KeysVec, keys, SetKeys, GetKeysNonContant);          // Keys property

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
		void SetTargetProxy(IValueProxy<Vec2F>* setter);

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
			static const int mApproxValuesCount = 10;     // Transition curve approximation values count
			Vec2F mApproxValues[mApproxValuesCount];      // Approximation values
			Vec2F mCurveApproxValues[mApproxValuesCount]; // Transition curve approximation values
			float mApproxLengths[mApproxValuesCount];     // Approximation segments lengths
			float mApproxTotalLength;                     // Total animation segment length (sum of mApproxLengths)
		};

	protected:
		KeysVec             mKeys;			        // Animation keys @SERIALIZABLE
		Vec2F               mValue;		            // Current animation value
		Vec2F*              mTarget = nullptr;		// Animation target value pointer
		Function<void()>    mTargetDelegate;        // Animation target value change event
		IValueProxy<Vec2F>* mTargetProxy = nullptr; // Animation target property pointer

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
		void SetTargetProxyVoid(void* target);

		// Registering this in animatable value agent
		void RegInAnimatable(AnimationState* state, const String& path);
	};
}

CLASS_BASES_META(o2::AnimatedValue<o2::Vec2F>)
{
	BASE_CLASS(o2::IAnimatedValue);
}
END_META;
CLASS_FIELDS_META(o2::AnimatedValue<o2::Vec2F>)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(target);
	PUBLIC_FIELD(targetDelegate);
	PUBLIC_FIELD(targetProxy);
	PUBLIC_FIELD(keys);
	PROTECTED_FIELD(mKeys).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mValue);
	PROTECTED_FIELD(mTarget);
	PROTECTED_FIELD(mTargetDelegate);
	PROTECTED_FIELD(mTargetProxy);
}
END_META;
CLASS_METHODS_META(o2::AnimatedValue<o2::Vec2F>)
{

	PUBLIC_FUNCTION(void, SetTarget, Vec2F*);
	PUBLIC_FUNCTION(void, SetTarget, Vec2F*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxy, IValueProxy<Vec2F>*);
	PUBLIC_FUNCTION(Vec2F, GetValue);
	PUBLIC_FUNCTION(Vec2F, GetValue, float);
	PUBLIC_FUNCTION(void, AddKeys, Vector<Key>, float);
	PUBLIC_FUNCTION(void, AddKey, const Key&);
	PUBLIC_FUNCTION(void, AddKey, const Key&, float);
	PUBLIC_FUNCTION(void, AddSmoothKey, const Key&, float);
	PUBLIC_FUNCTION(void, AddKey, float, const Vec2F&, const Vec2F&, const Vec2F&, float, float, float, float);
	PUBLIC_FUNCTION(void, AddKey, float, const Vec2F&, float);
	PUBLIC_FUNCTION(Key, GetKey, float);
	PUBLIC_FUNCTION(bool, RemoveKey, float);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(bool, ContainsKey, float);
	PUBLIC_FUNCTION(const KeysVec&, GetKeys);
	PUBLIC_FUNCTION(void, SetKeys, const KeysVec&);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(Vec2F, Evaluate, float);
	PROTECTED_FUNCTION(KeysVec, GetKeysNonContant);
	PROTECTED_FUNCTION(void, UpdateApproximation);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*);
	PROTECTED_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PROTECTED_FUNCTION(void, SetTargetProxyVoid, void*);
	PROTECTED_FUNCTION(void, RegInAnimatable, AnimationState*, const String&);
}
END_META;

CLASS_BASES_META(o2::AnimatedValue<o2::Vec2F>::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimatedValue<o2::Vec2F>::Key)
{
	PUBLIC_FIELD(position).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(value).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(prevSupportValue).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(nextSupportValue).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curvePrevCoef).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curvePrevCoefPos).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curveNextCoef).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(curveNextCoefPos).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mApproxValues);
	PUBLIC_FIELD(mCurveApproxValues);
	PUBLIC_FIELD(mApproxLengths);
	PUBLIC_FIELD(mApproxTotalLength);
}
END_META;
CLASS_METHODS_META(o2::AnimatedValue<o2::Vec2F>::Key)
{
}
END_META;
