#pragma once

#include "o2/Utils/Math/ApproximationValue.h"
#include "o2/Utils/Math/Math.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Containers/Vector.h"

namespace o2
{
	// ------------
	// Bezier curve
	// ------------
	class Curve: public ISerializable
	{
	public:
		class Key;

	public:
		PROPERTIES(Curve);
		PROPERTY(Vector<Key>, keys, SetKeys, GetKeys); // Keys property
		GETTER(float, length, Length);             // Max position getter

	public:
		Function<void()> onKeysChanged; // Keys changed event

	public:
		// Default constructor
		Curve();

		// Constructor by bezier coefficient in 0...1 range (some line as css-bezier)
		Curve(float beginCoef, float beginCoefPosition, float endCoef, float endCoefPosition);

		// Constructor from keys with smoothing
		Curve(const Vector<Vec2F>& values, bool smooth = true);

		// Copy-constructor
		Curve(const Curve& other);

		// Is equals operator
		bool operator==(const Curve& other) const;

		// Is not equals operator
		bool operator!=(const Curve& other) const;

		// Assign operator
		Curve& operator=(const Curve& other);

		// Append other curve at end
		Curve& operator+=(const Curve& other);

		// Returns value by position
		float Evaluate(float position) const;

		// Returns value by position
		float Evaluate(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update approximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange();

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange();

		// Moves all keys positions to offset
		void MoveKeys(float offset);

		// Moves keys from begin position positions to offset
		void MoveKeysFrom(float begin, float offset);

		// Appends curve at end
		void AppendCurve(const Curve& curve);

		// Prepends curve at end
		void PrependCurve(const Curve& curve);

		// Inserts curve at position
		void InsertCurve(const Curve& curve, float position);

		// Adds keys with smoothing at end
		void AppendKeys(const Vector<Vec2F>& values, bool smooth = true);

		// Adds keys at end
		void AppendKeys(const Vector<Key>& keys);

		// Adds keys with smoothing at beginning
		void PrependKeys(const Vector<Vec2F>& values, bool smooth = true);

		// Adds keys at beginning
		void PrependKeys(const Vector<Key>& keys);

		// Inserts keys with smoothing at position
		void InsertKeys(const Vector<Vec2F>& values, float position, bool smooth = true);

		// Inserts keys at position
		void InsertKeys(const Vector<Key>& keys, float position);

		// Inserts key and returns index
		int InsertKey(const Key& key);

		// Inserts key and returns index
		int InsertKey(float position, float value, float leftCoef, float leftCoefPosition,
					  float rightCoef, float rightCoefPosition);

		// Inserts value at position with smoothing and returns index
		int InsertKey(float position, float value, float smoothCoef = 1.0f);

		// Inserts flat key
		int InsertFlatKey(float position, float value);

		// Appends key at end with offset and returns index
		int AppendKey(float offset, float value, float leftCoef, float leftCoefPosition,
					  float rightCoef, float rightCoefPosition);

		// Appends key at end with offset and smoothing and returns index
		int AppendKey(float offset, float value, float smoothCoef = 1.0f);

		// Appends flat key at end with offset
		int AppendKey(float offset, float value);

		// Prepends key at beginning with offset and returns index
		int PrependKey(float offset, float value, float leftCoef, float leftCoefPosition,
					   float rightCoef, float rightCoefPosition);

		// Prepends key at beginning with offset and smoothing and returns index
		int PrependKey(float offset, float value, float smoothCoef = 1.0f);

		// Prepends flat key at beginning with offset
		int PrependKey(float offset, float value);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes key at index
		bool RemoveKeyAt(int idx);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if contains key at position
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

		// Sets key data at position
		void SetKey(const Key& key, int idx);

		// Smooths key at position
		void SmoothKey(float position, float smoothCoef = 1.0f);

		// Smooths key at index
		void SmoothKeyAt(int idx, float smoothCoef = 1.0f);

		// Returns curve max position
		float Length() const;

		// Returns true if curve is empty
		bool IsEmpty() const;

		// Returns bounding approximated keys rectangle
		RectF GetRect() const;

		// Key access operator by position
		Key operator[](float position) const;

		// Returns parametric specified curve
		// Sample: Parametric(someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		static Curve Parametric(float begin, float end, float duration,
								float beginCoef, float beginCoefPosition,
								float endCoef, float endCoefPosition);

		// Returns tween animation from begin to end in duration with ease in
		static Curve EaseIn(float begin = 0.0f, float end = 1.0f, float duration = 1.0f, float strongness = 0.4f);

		// Returns tween animation from begin to end in duration with ease out
		static Curve EaseOut(float begin = 0.0f, float end = 1.0f, float duration = 1.0f, float strongness = 0.4f);

		// Returns tween animation from begin to end in duration with ease in-out
		static Curve EaseInOut(float begin = 0.0f, float end = 1.0f, float duration = 1.0f, float strongness = 0.4f);

		// Returns tween animation from begin to end in duration with linear transition
		static Curve Linear(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);

		SERIALIZABLE(Curve);

	public:
		// ---------
		// Curve key
		// ---------
		class Key: public ISerializable
		{
		public:
			enum class Type { Smooth, Flat, Free, Linear, Broken, Discrete };

		public:
			UInt64 uid;                  // Random unique id @SERIALIZABLE
			float  value;                // Value @SERIALIZABLE
			float  position;             // Position @SERIALIZABLE
			float  leftSupportValue;     // Left bezier support value, relative to key value @SERIALIZABLE
			float  leftSupportPosition;  // Left bezier support position, relative to key position @SERIALIZABLE
			float  rightSupportValue;    // Right bezier support value, relative to key value @SERIALIZABLE
			float  rightSupportPosition; // Right bezier support position, relative to key position @SERIALIZABLE
			Type   supportsType;         // Support points type @SERIALIZABLE

		public:
			//Default constructor
			Key();

			// Constructor from value and position
			Key(float value, float position);

			// Constructor
			Key(float position, float value, float leftSupportValue, float leftSupportPosition,
				float rightSupportValue, float rightSupportPosition);

			// Copy-constructor
			Key(const Key& other);

			// Copy operator
			Key& operator=(const Key& other);

			// Assign from value operator
			Key& operator=(float value);

			// Float cast operator
			operator float() const;

			// Check equals operator
			bool operator==(const Key& other) const;

			// Check not equals operator
			bool operator!=(const Key& other) const;

			// Returns approximated points
			const ApproximationValue* GetApproximatedPoints() const;

			// Returns approximated points count
			int GetApproximatedPointsCount() const;

			// Returns bounds of approximation values
			const RectF& GetGetApproximatedPointsBounds() const;

			SERIALIZABLE(Key);

		public:
			static const int   mApproxValuesCount = 20;           // Approximation values count
			ApproximationValue mApproxValues[mApproxValuesCount]; // Approximation values 
			RectF              mApproxValuesBounds;               // Bounds of approximation values

			friend class Curve;
		};

	protected:
		bool mBatchChange = false; // It is true when began batch change
		bool mChangedKeys = false; // It is true when some keys changed during batch change

		Vector<Key> mKeys; // Curve keys @SERIALIZABLE

	protected:
		// Checks all smooth keys and updates supports points
		void CheckSmoothKeys();

		// Updates approximation
		void UpdateApproximation();

		// Returns keys (for property)
		Vector<Key> GetKeysNonContant();

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Smooths key at index and doesn't updates approximation
		void InternalSmoothKeyAt(int idx, float smoothCoef = 1.0f);

		// Smooths key at position and doesn't updates approximation
		void InternalSmoothKey(float position, float smoothCoef = 1.0f);
	};
}

PRE_ENUM_META(o2::Curve::Key::Type);

CLASS_BASES_META(o2::Curve)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Curve)
{
	FIELD().NAME(keys).PUBLIC();
	FIELD().NAME(length).PUBLIC();
	FIELD().NAME(onKeysChanged).PUBLIC();
	FIELD().DEFAULT_VALUE(false).NAME(mBatchChange).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mChangedKeys).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mKeys).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Curve)
{

	PUBLIC_FUNCTION(float, Evaluate, float);
	PUBLIC_FUNCTION(float, Evaluate, float, bool, int&, int&);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(void, MoveKeys, float);
	PUBLIC_FUNCTION(void, MoveKeysFrom, float, float);
	PUBLIC_FUNCTION(void, AppendCurve, const Curve&);
	PUBLIC_FUNCTION(void, PrependCurve, const Curve&);
	PUBLIC_FUNCTION(void, InsertCurve, const Curve&, float);
	PUBLIC_FUNCTION(void, AppendKeys, const Vector<Vec2F>&, bool);
	PUBLIC_FUNCTION(void, AppendKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, PrependKeys, const Vector<Vec2F>&, bool);
	PUBLIC_FUNCTION(void, PrependKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, InsertKeys, const Vector<Vec2F>&, float, bool);
	PUBLIC_FUNCTION(void, InsertKeys, const Vector<Key>&, float);
	PUBLIC_FUNCTION(int, InsertKey, const Key&);
	PUBLIC_FUNCTION(int, InsertKey, float, float, float, float, float, float);
	PUBLIC_FUNCTION(int, InsertKey, float, float, float);
	PUBLIC_FUNCTION(int, InsertFlatKey, float, float);
	PUBLIC_FUNCTION(int, AppendKey, float, float, float, float, float, float);
	PUBLIC_FUNCTION(int, AppendKey, float, float, float);
	PUBLIC_FUNCTION(int, AppendKey, float, float);
	PUBLIC_FUNCTION(int, PrependKey, float, float, float, float, float, float);
	PUBLIC_FUNCTION(int, PrependKey, float, float, float);
	PUBLIC_FUNCTION(int, PrependKey, float, float);
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
	PUBLIC_FUNCTION(void, SetKey, const Key&, int);
	PUBLIC_FUNCTION(void, SmoothKey, float, float);
	PUBLIC_FUNCTION(void, SmoothKeyAt, int, float);
	PUBLIC_FUNCTION(float, Length);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(RectF, GetRect);
	PUBLIC_STATIC_FUNCTION(Curve, Parametric, float, float, float, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(Curve, EaseIn, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(Curve, EaseOut, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(Curve, EaseInOut, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(Curve, Linear, float, float, float);
	PROTECTED_FUNCTION(void, CheckSmoothKeys);
	PROTECTED_FUNCTION(void, UpdateApproximation);
	PROTECTED_FUNCTION(Vector<Key>, GetKeysNonContant);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, InternalSmoothKeyAt, int, float);
	PROTECTED_FUNCTION(void, InternalSmoothKey, float, float);
}
END_META;

CLASS_BASES_META(o2::Curve::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Curve::Key)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(uid).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(value).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(position).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(leftSupportValue).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(leftSupportPosition).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(rightSupportValue).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(rightSupportPosition).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(supportsType).PUBLIC();
	FIELD().NAME(mApproxValues).PUBLIC();
	FIELD().NAME(mApproxValuesBounds).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::Curve::Key)
{

	PUBLIC_FUNCTION(const ApproximationValue*, GetApproximatedPoints);
	PUBLIC_FUNCTION(int, GetApproximatedPointsCount);
	PUBLIC_FUNCTION(const RectF&, GetGetApproximatedPointsBounds);
}
END_META;
