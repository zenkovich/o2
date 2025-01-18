#pragma once

#include "o2/Utils/Editor/Attributes/ScriptableAttribute.h"
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
    class Curve: public ISerializable, public RefCounterable
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
        // Default constructor @SCRIPTABLE
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

        // Returns value by position @SCRIPTABLE
        float Evaluate(float position, float randomRangeCoef = 0.0f) const;

        // Returns value by position
        float Evaluate(float position, float randomRangeCoef, bool direction, int& cacheKey, int& cacheKeyApprox) const;

        // Called when beginning keys batch change. After this call all keys modifications will not be update approximation
        // Used for optimizing many keys change
        void BeginKeysBatchChange();

        // Called when keys batch change completed. Updates approximation
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
        int InsertKey(float position, float value, float valueRange, float leftCoef, float leftCoefPosition,
                      float rightCoef, float rightCoefPosition);

        // Inserts value at position with smoothing and returns index
        int InsertKey(float position, float value, float valueRange = 0.0f, float smoothCoef = 1.0f);

        // Inserts flat key
        int InsertFlatKey(float position, float value, float valueRange = 0.0f);

        // Appends key at end with offset and returns index
        int AppendKey(float offset, float value, float valueRange, float leftCoef, float leftCoefPosition,
                      float rightCoef, float rightCoefPosition);

        // Appends key at end with offset and smoothing and returns index
        int AppendKey(float offset, float value, float valueRange = 0.0f, float smoothCoef = 1.0f);

        // Appends flat key at end with offset
        int AppendKey(float offset, float value, float valueRange = 0.0f);

        // Prepends key at beginning with offset and returns index
        int PrependKey(float offset, float value, float valueRange, float leftCoef, float leftCoefPosition,
                       float rightCoef, float rightCoefPosition);

        // Prepends key at beginning with offset and smoothing and returns index
        int PrependKey(float offset, float value, float valueRange = 0.0f, float smoothCoef = 1.0f);

        // Prepends flat key at beginning with offset
        int PrependKey(float offset, float value, float valueRange = 0.0f);

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

        // Returns curve max position @SCRIPTABLE
        float Length() const;

        // Returns true if curve is empty @SCRIPTABLE
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
            float  valueRange;           // Value range @SERIALIZABLE
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
            Key(float position, float value, float valueRange);

            // Constructor
            Key(float position, float value, float valueRange, float leftSupportValue, float leftSupportPosition,
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

            // Returns approximated points for top range
            const ApproximationValue* GetTopApproximatedPoints() const;

            // Returns approximated points for bottom range
            const ApproximationValue* GetBottomApproximatedPoints() const;

            // Returns approximated points count
            int GetApproximatedPointsCount() const;

            // Returns bounds of approximation values
            const RectF& GetGetApproximatedPointsBounds() const;

            SERIALIZABLE(Key);

        public:
            static const int   mApproxValuesCount = 20;                 // Approximation values count
            ApproximationValue mApproxTopValues[mApproxValuesCount];    // Approximation values for top range
            ApproximationValue mApproxBottomValues[mApproxValuesCount]; // Approximation values for top range
            RectF              mApproxValuesBounds;                     // Bounds of approximation values

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
// --- META ---

PRE_ENUM_META(o2::Curve::Key::Type);

CLASS_BASES_META(o2::Curve)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(o2::Curve)
{
    FIELD().PUBLIC().NAME(keys);
    FIELD().PUBLIC().NAME(length);
    FIELD().PUBLIC().NAME(onKeysChanged);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mBatchChange);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mChangedKeys);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mKeys);
}
END_META;
CLASS_METHODS_META(o2::Curve)
{

    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(float, float, float, float);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<Vec2F>&, bool);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Curve&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(float, Evaluate, float, float);
    FUNCTION().PUBLIC().SIGNATURE(float, Evaluate, float, float, bool, int&, int&);
    FUNCTION().PUBLIC().SIGNATURE(void, BeginKeysBatchChange);
    FUNCTION().PUBLIC().SIGNATURE(void, CompleteKeysBatchingChange);
    FUNCTION().PUBLIC().SIGNATURE(void, MoveKeys, float);
    FUNCTION().PUBLIC().SIGNATURE(void, MoveKeysFrom, float, float);
    FUNCTION().PUBLIC().SIGNATURE(void, AppendCurve, const Curve&);
    FUNCTION().PUBLIC().SIGNATURE(void, PrependCurve, const Curve&);
    FUNCTION().PUBLIC().SIGNATURE(void, InsertCurve, const Curve&, float);
    FUNCTION().PUBLIC().SIGNATURE(void, AppendKeys, const Vector<Vec2F>&, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, AppendKeys, const Vector<Key>&);
    FUNCTION().PUBLIC().SIGNATURE(void, PrependKeys, const Vector<Vec2F>&, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, PrependKeys, const Vector<Key>&);
    FUNCTION().PUBLIC().SIGNATURE(void, InsertKeys, const Vector<Vec2F>&, float, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, InsertKeys, const Vector<Key>&, float);
    FUNCTION().PUBLIC().SIGNATURE(int, InsertKey, const Key&);
    FUNCTION().PUBLIC().SIGNATURE(int, InsertKey, float, float, float, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, InsertKey, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, InsertFlatKey, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, AppendKey, float, float, float, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, AppendKey, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, AppendKey, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, PrependKey, float, float, float, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, PrependKey, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(int, PrependKey, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, RemoveKey, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, RemoveKeyAt, int);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllKeys);
    FUNCTION().PUBLIC().SIGNATURE(bool, ContainsKey, float);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Key>&, GetKeys);
    FUNCTION().PUBLIC().SIGNATURE(Key, GetKey, float);
    FUNCTION().PUBLIC().SIGNATURE(Key, GetKeyAt, int);
    FUNCTION().PUBLIC().SIGNATURE(Key, FindKey, UInt64);
    FUNCTION().PUBLIC().SIGNATURE(int, FindKeyIdx, UInt64);
    FUNCTION().PUBLIC().SIGNATURE(void, SetKeys, const Vector<Key>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetKey, const Key&, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SmoothKey, float, float);
    FUNCTION().PUBLIC().SIGNATURE(void, SmoothKeyAt, int, float);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(float, Length);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsEmpty);
    FUNCTION().PUBLIC().SIGNATURE(RectF, GetRect);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Curve, Parametric, float, float, float, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Curve, EaseIn, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Curve, EaseOut, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Curve, EaseInOut, float, float, float, float);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Curve, Linear, float, float, float);
    FUNCTION().PROTECTED().SIGNATURE(void, CheckSmoothKeys);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateApproximation);
    FUNCTION().PROTECTED().SIGNATURE(Vector<Key>, GetKeysNonContant);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, InternalSmoothKeyAt, int, float);
    FUNCTION().PROTECTED().SIGNATURE(void, InternalSmoothKey, float, float);
}
END_META;

CLASS_BASES_META(o2::Curve::Key)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Curve::Key)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(uid);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(value);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(valueRange);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(position);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(leftSupportValue);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(leftSupportPosition);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(rightSupportValue);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(rightSupportPosition);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(supportsType);
    FIELD().PUBLIC().NAME(mApproxTopValues);
    FIELD().PUBLIC().NAME(mApproxBottomValues);
    FIELD().PUBLIC().NAME(mApproxValuesBounds);
}
END_META;
CLASS_METHODS_META(o2::Curve::Key)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(float, float, float);
    FUNCTION().PUBLIC().CONSTRUCTOR(float, float, float, float, float, float, float);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Key&);
    FUNCTION().PUBLIC().SIGNATURE(const ApproximationValue*, GetTopApproximatedPoints);
    FUNCTION().PUBLIC().SIGNATURE(const ApproximationValue*, GetBottomApproximatedPoints);
    FUNCTION().PUBLIC().SIGNATURE(int, GetApproximatedPointsCount);
    FUNCTION().PUBLIC().SIGNATURE(const RectF&, GetGetApproximatedPointsBounds);
}
END_META;
// --- END META ---
