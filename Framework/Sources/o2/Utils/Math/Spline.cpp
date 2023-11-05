#include "o2/stdafx.h"
#include "Spline.h"

#include "o2/Utils/Math/Interpolation.h"
#include "o2/Utils/Tools/KeySearch.h"

namespace o2
{
    Spline::Spline()
    {}

    Spline::Spline(const Vector<Vec2F>& values, bool smooth /*= true*/)
    {
        AppendKeys(values, smooth);
    }

    Spline::Spline(const Spline& other) :
        mKeys(other.mKeys), keys(this), length(this)
    { }

    bool Spline::operator!=(const Spline& other) const
    {
        return !(*this == other);
    }

    bool Spline::operator==(const Spline& other) const
    {
        if (mKeys.Count() != other.mKeys.Count())
            return false;

        for (int i = 0; i < mKeys.Count(); i++)
        {
            if (mKeys[i] != other.mKeys[i])
                return false;
        }

        return true;
    }

    Spline& Spline::operator=(const Spline& other)
    {
        mKeys = other.mKeys;

        UpdateApproximation();

        onKeysChanged();

        return *this;
    }

    Spline& Spline::operator+=(const Spline& other)
    {
        AppendSpline(other);
        return *this;
    }

    Vec2F Spline::Evaluate(float position) const
    {
        int cacheKey = 0, cacheKeyApprox = 0;
        return Evaluate(position, true, cacheKey, cacheKeyApprox);
    }

    Vec2F Spline::Evaluate(float position, bool direction, int& cacheKey, int& cacheKeyApprox) const
    {
        int count = mKeys.Count();

        if (count == 1)
            return mKeys[0].value;
        else if (count == 0)
            return Vec2F();

        int prevCacheKey = cacheKey;
        int keyLeftIdx = -1, keyRightIdx = -1;

        if (mClosed)
            SearchKeyClosed(mKeys, count, position, keyLeftIdx, keyRightIdx, direction, cacheKey);
        else
            SearchKey(mKeys, count, position, keyLeftIdx, keyRightIdx, direction, cacheKey);

        if (keyLeftIdx < 0)
            return Vec2F();

        const Key& rightKey = mKeys[keyRightIdx];

        int segLeftIdx = 0;
        int segRightIdx = 1;

        if (keyLeftIdx != prevCacheKey)
            cacheKeyApprox = 0;

        SearchKey(rightKey.mApproxValues, Key::mApproxValuesCount, position, segLeftIdx, segRightIdx, direction, cacheKeyApprox);

        const ApproximationVec2F& segLeft = rightKey.mApproxValues[segLeftIdx];
        const ApproximationVec2F& segRight = rightKey.mApproxValues[segRightIdx];

        float dist = segRight.position - segLeft.position;
        float coef = (position - segLeft.position) / dist;

        return Math::Lerp(segLeft.value, segRight.value, coef);
    }

    void Spline::BeginKeysBatchChange()
    {
        mBatchChange = true;
    }

    void Spline::CompleteKeysBatchingChange()
    {
        UpdateApproximation();
        mBatchChange = false;
        mChangedKeys = false;
    }

    void Spline::SetClosed(bool closed)
    {
        mClosed = closed;

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    bool Spline::IsClosed() const
    {
        return mClosed;
    }

    void Spline::AppendSpline(const Spline& Spline)
    {
        AppendKeys(Spline.mKeys);
    }

    void Spline::PrependSpline(const Spline& Spline)
    {
        PrependKeys(Spline.mKeys);
    }

    void Spline::InsertSpline(const Spline& Spline, int idx)
    {
        InsertKeys(Spline.mKeys, idx);
    }

    void Spline::AppendKeys(const Vector<Vec2F>& values, bool smooth /*= true*/)
    {
        AppendKeys(values.Convert<Key>([=](const Vec2F& v) {
            Key res(v, Vec2F(), Vec2F());
            res.supportsType = smooth ? Key::Type::Smooth : Key::Type::Broken;
            return res;
        }));
    }

    void Spline::AppendKeys(const Vector<Key>& keys)
    {
        if (keys.IsEmpty())
            return;

        mKeys.Add(keys);
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    void Spline::PrependKeys(const Vector<Vec2F>& values, bool smooth /*= true*/)
    {
        PrependKeys(values.Convert<Key>([=](const Vec2F& v) {
            Key res(v, Vec2F(), Vec2F());
            res.supportsType = smooth ? Key::Type::Smooth : Key::Type::Broken;
            return res;
        }));
    }

    void Spline::PrependKeys(const Vector<Key>& keys)
    {
        if (keys.IsEmpty())
            return;

        mKeys.Insert(keys, 0);

        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    void Spline::InsertKeys(const Vector<Vec2F>& values, int idx, bool smooth /*= true*/)
    {
        InsertKeys(values.Convert<Key>([=](const Vec2F& v) {
            Key res(v, Vec2F(), Vec2F());
            res.supportsType = smooth ? Key::Type::Smooth : Key::Type::Broken;
            return res;
        }), idx);
    }

    void Spline::InsertKeys(const Vector<Key>& keys, int idx)
    {
        if (keys.IsEmpty())
            return;

        mKeys.Insert(keys, idx);

        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    void Spline::InsertKey(const Key& key, int idx)
    {
        mKeys.Insert(key, idx);

        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    void Spline::InsertKey(int idx, const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport)
    {
        InsertKey(Key(position, prevSupport, nextSupport), idx);
    }

    void Spline::InsertKey(int idx, const Vec2F& position, float smoothCoef /*= 1.0f*/)
    {
        
        mKeys.Insert(Key(position, Vec2F(), Vec2F()), idx);
        SmoothKey(idx, smoothCoef);

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    int Spline::AppendKey(const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport)
    {
        Key newKey(position, prevSupport, nextSupport);

        newKey.supportsType = Key::Type::Broken;

        mKeys.Add(newKey);
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();

        return mKeys.Count() - 1;
    }

    int Spline::AppendKey(const Vec2F& position, float smoothCoef /*= 1.0f*/)
    {
        Key newKey(position, Vec2F(), Vec2F());
        newKey.supportsType = Key::Type::Smooth;

        mKeys.Add(newKey);
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();

        return mKeys.Count() - 1;
    }

    int Spline::PrependKey(const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport)
    {
        Key newKey(position, prevSupport, nextSupport);
        newKey.supportsType = Key::Type::Broken;

        mKeys.Add(newKey);
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();

        return 0;
    }

    int Spline::PrependKey(const Vec2F& position, float smoothCoef /*= 1.0f*/)
    {
        Key newKey(position, Vec2F(), Vec2F());
        newKey.supportsType = Key::Type::Smooth;

        mKeys.Add(newKey);
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();

        return 0;
    }

    Spline::Key Spline::GetKey(int idx) const
    {
        if (idx < 0 || idx > mKeys.Count() - 1)
            return Key();

        return mKeys[idx];
    }

    Spline::Key Spline::FindKey(UInt64 uid) const
    {
        for (auto& key : mKeys)
        {
            if (key.uid == uid)
                return key;
        }

        return Key();
    }

    int Spline::FindKeyIdx(UInt64 uid) const
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

    bool Spline::RemoveKey(int idx)
    {
        if (idx < 0 || idx >= mKeys.Count())
            return false;

        mKeys.RemoveAt(idx);

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();

        return true;
    }

    void Spline::RemoveAllKeys()
    {
        mKeys.Clear();
        onKeysChanged();
    }

    const Vector<Spline::Key>& Spline::GetKeys() const
    {
        return mKeys;
    }

    void Spline::SetKeys(const Vector<Key>& keys)
    {
        mKeys = keys;
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    void Spline::SetKey(const Key& key, int idx)
    {
        if (idx < 0 || idx > mKeys.Count() - 1)
            return;

        mKeys[idx] = key;
        CheckSmoothKeys();

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    void Spline::SmoothKey(int idx, float smoothCoef)
    {
        InternalSmoothKeyAt(idx, smoothCoef);

        if (mBatchChange)
            mChangedKeys = true;
        else
            UpdateApproximation();
    }

    float Spline::Length() const
    {
        if (mKeys.Count() == 0)
            return 0.0f;

        return mClosed ? mKeys[0].position : mKeys.Last().position;
    }

    bool Spline::IsEmpty() const
    {
        return mKeys.Count() == 0;
    }

    RectF Spline::GetRect() const
    {
        RectF res;

        if (mKeys.Count() < 2)
            return res;

        res.left = mKeys[1].mApproxValues[0].value.x; res.right = mKeys[1].mApproxValues[0].value.x;
        res.top = mKeys[1].mApproxValues[0].value.y; res.bottom = mKeys[1].mApproxValues[0].value.y;

        for (int k = 1; k < mKeys.Count(); k++)
        {
            auto& key = mKeys[k];
            for (int i = 0; i < Key::mApproxValuesCount; i++)
            {
                res.left = Math::Min(key.mApproxValues[i].value.x, res.left);
                res.right = Math::Max(key.mApproxValues[i].value.x, res.right);
                res.top = Math::Max(key.mApproxValues[i].value.y, res.top);
                res.bottom = Math::Min(key.mApproxValues[i].value.y, res.bottom);
            }
        }

        return res;
    }

    const Spline::Key& Spline::operator[](int idx) const
    {
        if (idx >=0 && idx <= mKeys.Count() - 1)
            return mKeys[idx];

        static Key empty;
        return empty;
    }

    Spline::Key& Spline::operator[](int idx)
    {
        if (idx >=0 && idx <= mKeys.Count() - 1)
            return mKeys[idx];

        static Key empty;
        return empty;
    }

    void Spline::CheckSmoothKeys()
    {
        for (int i = 0; i < mKeys.Count(); i++)
        {
            if (mKeys[i].supportsType == Key::Type::Smooth)
                InternalSmoothKeyAt(i);
        }
    }

    void Spline::UpdateApproximation()
    {
        float length = 0.0f;
        Vec2F prev;
        if (!mKeys.IsEmpty())
        {
            prev = mKeys[0].value;
            mKeys[0].position = 0.0f;
        }

        auto calculateApproximation = [&](int begin, int end) {
            Key& beginKey = mKeys[begin];
            Key& endKey = mKeys[end];

            Vec2F a = beginKey.value;
            Vec2F b = beginKey.value + beginKey.nextSupport;
            Vec2F c = endKey.value + endKey.prevSupport;
            Vec2F d = endKey.value;

            endKey.mApproxValuesBounds.Set(a, a);
            for (int j = 0; j < Key::mApproxValuesCount; j++)
            {
                float coef = (float)j / (float)(Key::mApproxValuesCount - 1);
                Vec2F p = Bezier(a, b, c, d, coef);

                length += (p - prev).Length();
                prev = p;

                endKey.mApproxValues[j].value = p;
                endKey.mApproxValues[j].position = length;

                endKey.mApproxValuesBounds.left = Math::Min(endKey.mApproxValuesBounds.left, p.x);
                endKey.mApproxValuesBounds.right = Math::Max(endKey.mApproxValuesBounds.right, p.x);
                endKey.mApproxValuesBounds.top = Math::Max(endKey.mApproxValuesBounds.top, p.y);
                endKey.mApproxValuesBounds.bottom = Math::Min(endKey.mApproxValuesBounds.bottom, p.y);
            }

            mKeys[end].position = length;
        };

        for (int i = 1; i < mKeys.Count(); i++)
            calculateApproximation(i - 1, i);

        if (mClosed && !mKeys.IsEmpty())
            calculateApproximation(mKeys.Count() - 1, 0);

        onKeysChanged();
    }

    Vector<Spline::Key> Spline::GetKeysNonContant()
    {
        return mKeys;
    }

    void Spline::OnDeserialized(const DataValue& node)
    {
        UpdateApproximation();
    }

    void Spline::InternalSmoothKeyAt(int idx, float smoothCoef /*= 1.0f*/)
    {
        const float baseSmoothCoef = 0.5f;

        Key& key = mKeys[idx];
        Vec2F thisKeyPoint = key.value;

        if (idx == 0)
        {
            Key& nextKey = mKeys[Math::Min(idx + 1, mKeys.Count() - 1)];

            Vec2F nextKeyPoint = nextKey.value;
            Vec2F supportVec = (nextKeyPoint - thisKeyPoint)*baseSmoothCoef*smoothCoef;

            key.nextSupport = supportVec;
        }
        else if (idx == mKeys.Count() - 1)
        {
            Key& lastKey = mKeys[Math::Max(0, idx - 1)];

            Vec2F lastKeyPoint = lastKey.value;
            Vec2F supportVec = (lastKeyPoint - thisKeyPoint)*baseSmoothCoef*smoothCoef;

            key.prevSupport = supportVec;
        }
        else
        {
            Key& lastKey = mKeys[Math::Max(0, idx - 1)];
            Key& nextKey = mKeys[Math::Min(idx + 1, mKeys.Count())];

            Vec2F lastKeyPoint = lastKey.value;
            Vec2F nextKeyPoint = nextKey.value;

            Vec2F thisToLast = lastKeyPoint - thisKeyPoint;
            Vec2F thisToNext = nextKeyPoint - thisKeyPoint;

            float supportLength = Math::Min(thisToNext.Length(), thisToLast.Length())*baseSmoothCoef*smoothCoef;
            Vec2F supportVec = Math::CalculateEllipseTangent(lastKeyPoint, thisKeyPoint, nextKeyPoint)*supportLength;

            key.prevSupport = supportVec.Inverted();
            key.nextSupport = supportVec;
        }
    }

    Spline::Key::Key()
    {}

    Spline::Key::Key(const Vec2F& value, const Vec2F& prevSupport, const Vec2F& nextSupport) :
        value(value), prevSupport(prevSupport), nextSupport(nextSupport), supportsType(Type::Broken)
    { }

    Spline::Key::Key(const Key& other) :
        uid(other.uid), value(other.value), position(other.position), prevSupport(other.prevSupport),
        nextSupport(other.nextSupport), supportsType(other.supportsType), mApproxValuesBounds(other.mApproxValuesBounds)
    {
        memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount * sizeof(ApproximationVec2F));
    }

    Spline::Key::operator Vec2F() const
    {
        return value;
    }

    const ApproximationVec2F* Spline::Key::GetApproximatedPoints() const
    {
        return mApproxValues;
    }

    int Spline::Key::GetApproximatedPointsCount() const
    {
        return mApproxValuesCount;
    }

    const RectF& Spline::Key::GetGetApproximatedPointsBounds() const
    {
        return mApproxValuesBounds;
    }

    Spline::Key& Spline::Key::operator=(const Vec2F& value)
    {
        this->value = value;
        return *this;
    }

    Spline::Key& Spline::Key::operator=(const Key& other)
    {
        uid = other.uid;
        value = other.value;
        position = other.position;
        prevSupport = other.prevSupport;
        nextSupport = other.nextSupport;
        supportsType = other.supportsType;
        mApproxValuesBounds = other.mApproxValuesBounds;
        memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount * sizeof(Vec2F));

        return *this;
    }

    bool Spline::Key::operator!=(const Key& other) const
    {
        return !(*this == other);
    }

    bool Spline::Key::operator==(const Key& other) const
    {
        return Math::Equals(position, other.position) && Math::Equals(value, other.value) &&
            Math::Equals(prevSupport, other.prevSupport) &&
            Math::Equals(nextSupport, other.nextSupport) &&
            supportsType == other.supportsType;
    }

}
// --- META ---

ENUM_META(o2::Spline::Key::Type)
{
    ENUM_ENTRY(Broken);
    ENUM_ENTRY(Free);
    ENUM_ENTRY(Smooth);
}
END_ENUM_META;

DECLARE_CLASS(o2::Spline);

DECLARE_CLASS(o2::Spline::Key);
// --- END META ---
