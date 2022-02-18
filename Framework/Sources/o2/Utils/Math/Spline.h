#pragma once

#include "o2/Utils/Math/ApproximationValue.h"
#include "o2/Utils/Math/Math.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Containers/Vector.h"

namespace o2
{
	// -------------
	// Bezier spline
	// -------------
	class Spline: public ISerializable
	{
	public:
		class Key;

	public:
		PROPERTIES(Spline);
		PROPERTY(Vector<Key>, keys, SetKeys, GetKeys); // Keys property
		GETTER(float, length, Length);                 // Length of spline

	public:
		Function<void()> onKeysChanged; // Keys changed event

	public:
		// Default constructor
		Spline();

		// Constructor from keys with smoothing
		Spline(const Vector<Vec2F>& values, bool smooth = true);

		// Copy-constructor
		Spline(const Spline& other);

		// Is equals operator
		bool operator==(const Spline& other) const;

		// Is not equals operator
		bool operator!=(const Spline& other) const;

		// Assign operator
		Spline& operator=(const Spline& other);

		// Append other Spline at end
		Spline& operator+=(const Spline& other);

		// Returns value by position
		Vec2F Evaluate(float position) const;

		// Returns value by position
		Vec2F Evaluate(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const;

		// It is called when beginning keys batch change. After this call all keys modifications will not be update approximation
		// Used for optimizing many keys change
		void BeginKeysBatchChange();

		// It is called when keys batch change completed. Updates approximation
		void CompleteKeysBatchingChange();

		// Sets spline closed or not
		void SetClosed(bool closed);

		// Reutnrs is spline closed
		bool IsClosed() const;

		// Appends Spline at end
		void AppendSpline(const Spline& Spline);

		// Prepends Spline at end
		void PrependSpline(const Spline& Spline);

		// Inserts Spline at position
		void InsertSpline(const Spline& Spline, int idx);

		// Adds keys with smoothing at end
		void AppendKeys(const Vector<Vec2F>& values, bool smooth = true);

		// Adds keys at end
		void AppendKeys(const Vector<Key>& keys);

		// Adds keys with smoothing at beginning
		void PrependKeys(const Vector<Vec2F>& values, bool smooth = true);

		// Adds keys at beginning
		void PrependKeys(const Vector<Key>& keys);

		// Inserts keys with smoothing at position
		void InsertKeys(const Vector<Vec2F>& values, int idx, bool smooth = true);

		// Inserts keys at position
		void InsertKeys(const Vector<Key>& keys, int idx);

		// Inserts key and returns index
		void InsertKey(const Key& key, int idx);

		// Inserts key and returns index
		void InsertKey(int idx, const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport);

		// Inserts value at position with smoothing and returns index
		void InsertKey(int idx, const Vec2F& position, float smoothCoef = 1.0f);

		// Appends key at end with offset and returns index
		int AppendKey(const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport);

		// Appends key at end with offset and smoothing and returns index
		int AppendKey(const Vec2F& position, float smoothCoef = 1.0f);

		// Prepends key at beginning with offset and returns index
		int PrependKey(const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport);

		 // Prepends key at beginning with offset and smoothing and returns index
		int PrependKey(const Vec2F& position, float smoothCoef = 1.0f);

		// Removes key at index
		bool RemoveKey(int idx);

		// Removes all keys
		void RemoveAllKeys();

		// Returns keys array
		const Vector<Key>& GetKeys() const;

		// Returns key at index
		Key GetKey(int idx) const;

		// Returns key by uid
		Key FindKey(UInt64 uid) const;

		// Returns key index by uid
		int FindKeyIdx(UInt64 uid) const;

		// Sets keys
		void SetKeys(const Vector<Key>& keys);

		// Sets key data at position
		void SetKey(const Key& key, int idx);

		// Smooths key at position
		void SmoothKey(int idx, float smoothCoef = 1.0f);

		// Returns length of spline
		float Length() const;

		// Returns true if Spline is empty
		bool IsEmpty() const;

		// Returns bounding approximated keys rectangle
		RectF GetRect() const;

		// Key access operator by index
		const Key& operator[](int idx) const;

		// Key access operator by index
		Key& operator[](int idx);

		SERIALIZABLE(Spline);

	public:
		// ---------
		// Spline key
		// ---------
		class Key: public ISerializable
		{
		public:
			enum class Type { Smooth, Free, Broken };

		public:
			UInt64 uid = Math::Random();        // Random unique id @SERIALIZABLE
			float  position = 0.0f;             // Position from start, or length @SERIALIZABLE
			Vec2F  value;                       // Main position @SERIALIZABLE
			Vec2F  prevSupport;                 // Previous support point @SERIALIZABLE
			Vec2F  nextSupport;                 // Next support point @SERIALIZABLE
			Type   supportsType = Type::Smooth; // Type of support points @SERIALIZABLE

		public:
			//Default constructor
			Key();

			// Constructor
			Key(const Vec2F& value, const Vec2F& prevSupport, const Vec2F& nextSupport);

			// Copy-constructor
			Key(const Key& other);

			// Copy operator
			Key& operator=(const Key& other);

			// Assign from value operator
			Key& operator=(const Vec2F& position);

			// Vec2F cast operator
			operator Vec2F() const;

			// Check equals operator
			bool operator==(const Key& other) const;

			// Check not equals operator
			bool operator!=(const Key& other) const;

			// Returns approximated points
			const ApproximationVec2F* GetApproximatedPoints() const;

			// Returns approximated points count
			int GetApproximatedPointsCount() const;

			// Returns bounds of approximation values
			const RectF& GetGetApproximatedPointsBounds() const;

			SERIALIZABLE(Key);

		public:
			static const int   mApproxValuesCount = 20;           // Approximation values count
			ApproximationVec2F mApproxValues[mApproxValuesCount]; // Approximation values 
			RectF              mApproxValuesBounds;               // Bounds of approximation values

			friend class Spline;
		};

	protected:
		bool mBatchChange = false; // It is true when began batch change
		bool mChangedKeys = false; // It is true when some keys changed during batch change

		Vector<Key> mKeys; // Spline keys @SERIALIZABLE

		bool mClosed = false; // Is spline closed

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
	};
}

PRE_ENUM_META(o2::Spline::Key::Type);

CLASS_BASES_META(o2::Spline)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Spline)
{
	FIELD().NAME(keys).PUBLIC();
	FIELD().NAME(length).PUBLIC();
	FIELD().NAME(onKeysChanged).PUBLIC();
	FIELD().DEFAULT_VALUE(false).NAME(mBatchChange).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mChangedKeys).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mKeys).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mClosed).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Spline)
{

	PUBLIC_FUNCTION(Vec2F, Evaluate, float);
	PUBLIC_FUNCTION(Vec2F, Evaluate, float, bool, int&, int&);
	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(void, SetClosed, bool);
	PUBLIC_FUNCTION(bool, IsClosed);
	PUBLIC_FUNCTION(void, AppendSpline, const Spline&);
	PUBLIC_FUNCTION(void, PrependSpline, const Spline&);
	PUBLIC_FUNCTION(void, InsertSpline, const Spline&, int);
	PUBLIC_FUNCTION(void, AppendKeys, const Vector<Vec2F>&, bool);
	PUBLIC_FUNCTION(void, AppendKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, PrependKeys, const Vector<Vec2F>&, bool);
	PUBLIC_FUNCTION(void, PrependKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, InsertKeys, const Vector<Vec2F>&, int, bool);
	PUBLIC_FUNCTION(void, InsertKeys, const Vector<Key>&, int);
	PUBLIC_FUNCTION(void, InsertKey, const Key&, int);
	PUBLIC_FUNCTION(void, InsertKey, int, const Vec2F&, const Vec2F&, const Vec2F&);
	PUBLIC_FUNCTION(void, InsertKey, int, const Vec2F&, float);
	PUBLIC_FUNCTION(int, AppendKey, const Vec2F&, const Vec2F&, const Vec2F&);
	PUBLIC_FUNCTION(int, AppendKey, const Vec2F&, float);
	PUBLIC_FUNCTION(int, PrependKey, const Vec2F&, const Vec2F&, const Vec2F&);
	PUBLIC_FUNCTION(int, PrependKey, const Vec2F&, float);
	PUBLIC_FUNCTION(bool, RemoveKey, int);
	PUBLIC_FUNCTION(void, RemoveAllKeys);
	PUBLIC_FUNCTION(const Vector<Key>&, GetKeys);
	PUBLIC_FUNCTION(Key, GetKey, int);
	PUBLIC_FUNCTION(Key, FindKey, UInt64);
	PUBLIC_FUNCTION(int, FindKeyIdx, UInt64);
	PUBLIC_FUNCTION(void, SetKeys, const Vector<Key>&);
	PUBLIC_FUNCTION(void, SetKey, const Key&, int);
	PUBLIC_FUNCTION(void, SmoothKey, int, float);
	PUBLIC_FUNCTION(float, Length);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(RectF, GetRect);
	PROTECTED_FUNCTION(void, CheckSmoothKeys);
	PROTECTED_FUNCTION(void, UpdateApproximation);
	PROTECTED_FUNCTION(Vector<Key>, GetKeysNonContant);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, InternalSmoothKeyAt, int, float);
}
END_META;

CLASS_BASES_META(o2::Spline::Key)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Spline::Key)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Math::Random()).NAME(uid).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(position).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(value).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(prevSupport).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(nextSupport).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Type::Smooth).NAME(supportsType).PUBLIC();
	FIELD().NAME(mApproxValues).PUBLIC();
	FIELD().NAME(mApproxValuesBounds).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::Spline::Key)
{

	PUBLIC_FUNCTION(const ApproximationVec2F*, GetApproximatedPoints);
	PUBLIC_FUNCTION(int, GetApproximatedPointsCount);
	PUBLIC_FUNCTION(const RectF&, GetGetApproximatedPointsBounds);
}
END_META;
