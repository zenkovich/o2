#include "stdafx.h"
#include "Curve.h"

#include "Utils/Math/Interpolation.h"

namespace o2
{
	Curve::Curve()
	{}

	Curve::Curve(float beginCoef, float beginCoefPosition, float endCoef, float endCoefPosition)
	{
		mKeys.Add(Key(0.0f, 0.0f, 0.0f, 0.0f, beginCoef, Math::Clamp01(beginCoefPosition)));
		mKeys.Add(Key(1.0f, 1.0f, 1.0f - endCoef, -(1.0f - Math::Clamp01(endCoefPosition)), 0.0f, 0.0f));
		UpdateApproximation();
	}

	Curve::Curve(Vector<Vec2F> values, bool smooth /*= true*/)
	{
		AppendKeys(values, smooth);
	}

	Curve::Curve(const Curve& other):
		mKeys(other.mKeys), keys(this), length(this)
	{}

	bool Curve::operator!=(const Curve& other) const
	{
		return !(*this == other);
	}

	bool Curve::operator==(const Curve& other) const
	{
		if (mKeys.Count() != other.mKeys.Count())
			return false;

		for (int i = 0; i < mKeys.Count(); i++)
			if (mKeys[i] != other.mKeys[i])
				return false;

		return true;
	}

	Curve& Curve::operator=(const Curve& other)
	{
		mKeys = other.mKeys;
		UpdateApproximation();
		onKeysChanged();
		return *this;
	}

	Curve& Curve::operator+=(const Curve& other)
	{
		AppendCurve(other);
		return *this;
	}

	float Curve::Evaluate(float position)
	{
		int count = mKeys.Count();

		if (count == 1)
			return mKeys[0].value;
		else if (count == 0)
			return 0.0f;

		int begi = -1, endi = -1;

		for (int i = 1; i < count; i++)
		{
			begi = i - 1;
			endi = i;

			if (mKeys[i].position > position)
				break;
		}

		if (begi < 0)
			return 0.0f;

		Key& beginKey = mKeys[begi];
		Key& endKey = mKeys[endi];

		int segBeg = 0;
		int segEnd = 1;

		for (int i = 1; i < Key::mApproxValuesCount; i++)
		{
			segBeg = i - 1;
			segEnd = i;

			if (endKey.mApproxValues[i].x > position)
				break;
		}

		Vec2F begs = endKey.mApproxValues[segBeg];
		Vec2F ends = endKey.mApproxValues[segEnd];

		float dist = ends.x - begs.x;
		float coef = (position - begs.x)/dist;

		return Math::Lerp(begs.y, ends.y, coef);
	}

	void Curve::MoveKeys(float offset)
	{
		for (auto& key : mKeys)
			key.position += offset;

		UpdateApproximation();
	}

	void Curve::MoveKeysFrom(float begin, float offset)
	{
		for (auto& key : mKeys)
		{
			if (key.position >= begin)
				key.position += offset;
		}

		UpdateApproximation();
	}

	void Curve::AppendCurve(const Curve& curve)
	{
		AppendKeys(curve.mKeys);
	}

	void Curve::PrependCurve(const Curve& curve)
	{
		PrependKeys(curve.mKeys);
	}

	void Curve::InsertCurve(const Curve& curve, float position)
	{
		InsertKeys(curve.mKeys, position);
	}

	void Curve::AppendKeys(Vector<Vec2F> values, bool smooth /*= true*/)
	{
		AppendKeys(values.Select<Key>([=](const Vec2F& v) {
			Key res(v.x, v.y, 0, 0, 0, 0);
			res.supportsType = smooth ? Key::Type::Smooth : Key::Type::Broken;
			return res;
		}));
	}

	void Curve::AppendKeys(const KeysVec& keys)
	{
		if (keys.IsEmpty())
			return;

		float offset = (mKeys.IsEmpty() ? 0.0f : mKeys.Last().position) - keys[0].position;

		for (auto key : keys)
		{
			key.position += offset;
			mKeys.Add(key);
		}

		CheckSmoothKeys();
		UpdateApproximation();
	}

	void Curve::PrependKeys(Vector<Vec2F> values, bool smooth /*= true*/)
	{
		PrependKeys(values.Select<Key>([=](const Vec2F& v) {
			Key res(v.x, v.y, 0, 0, 0, 0);
			res.supportsType = smooth ? Key::Type::Smooth : Key::Type::Broken;
			return res;
		}));
	}

	void Curve::PrependKeys(const KeysVec& keys)
	{
		if (keys.IsEmpty())
			return;

		float currentBegin = mKeys.IsEmpty() ? 0.0f : mKeys[0].position;
		float offset = keys.Last().position - Math::Min(currentBegin, 0.0f);

		for (int i = 0; i < mKeys.Count(); i++)
			mKeys[i].position += offset;

		mKeys.Insert(keys, 0);

		CheckSmoothKeys();
		UpdateApproximation();
	}

	void Curve::InsertKeys(Vector<Vec2F> values, float position, bool smooth /*= true*/)
	{
		InsertKeys(values.Select<Key>([=](const Vec2F& v) {
			Key res(v.x, v.y, 0, 0, 0, 0);
			res.supportsType = smooth ? Key::Type::Smooth : Key::Type::Broken;
			return res;
		}), position);
	}

	void Curve::InsertKeys(const KeysVec& keys, float position)
	{
		if (keys.IsEmpty())
			return;

		int pos = mKeys.Count();
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (mKeys[i].position > position)
			{
				pos = i;
				break;
			}
		}

		mKeys.Insert(keys, pos);

		float insertOffset = position - keys[0].position;
		for (int i = 0; i < keys.Count(); i++)
			mKeys[i + pos].position += insertOffset;

		float offset = keys.Last().position - keys[0].position;
		for (int i = pos + keys.Count(); i < mKeys.Count(); i++)
			mKeys[i].position += offset;

		CheckSmoothKeys();
		UpdateApproximation();
	}

	int Curve::InsertKey(const Key& key)
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

		CheckSmoothKeys();
		UpdateApproximation();

		return pos;
	}

	int Curve::InsertKey(float position, float value, float leftCoef, float leftCoefPosition,
						 float rightCoef, float rightCoefPosition)
	{
		return InsertKey(Key(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition));
	}

	int Curve::InsertKey(float position, float value, float smooth /*= 1.0f*/)
	{
		int pos = mKeys.Count();
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (mKeys[i].position > position)
			{
				pos = i;
				break;
			}
		}

		pos = Math::Clamp(pos, 0, mKeys.Count());
		mKeys.Insert(Key(position, value, value, 1.0f, value, 0.0f), pos);
		SmoothKey(position, smooth);

		UpdateApproximation();

		return pos;
	}

	int Curve::InsertFlatKey(float position, float value)
	{
		int pos = mKeys.Count();
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (mKeys[i].position > position)
			{
				pos = i;
				break;
			}
		}

		pos = Math::Clamp(pos, 0, mKeys.Count());
		float leftSupport = 0, rightSupport = 0;
		const float supportCoef = 0.4f;

		if (pos > 0)
			leftSupport = (position - mKeys[pos - 1].position)*supportCoef;

		if (pos < mKeys.Count() - 1)
			rightSupport = (mKeys[pos + 1] - position)*supportCoef;

		mKeys.Insert(Key(position, value, value, leftSupport, value, rightSupport), pos);

		CheckSmoothKeys();
		UpdateApproximation();

		return pos;
	}

	int Curve::AppendKey(float offset, float value, float leftCoef, float leftCoefPosition,
						 float rightCoef, float rightCoefPosition)
	{
		Key newKey((mKeys.IsEmpty() ? 0.0f : mKeys.Last().position) + offset, value, leftCoef, leftCoefPosition,
				   rightCoef, rightCoefPosition);

		newKey.supportsType = Key::Type::Broken;

		mKeys.Add(newKey);
		CheckSmoothKeys();
		UpdateApproximation();

		return mKeys.Count() - 1;
	}

	int Curve::AppendKey(float offset, float value, float smoothCoef /*= 1.0f*/)
	{
		Key newKey((mKeys.IsEmpty() ? 0.0f : mKeys.Last().position) + offset, value, value, 0, value, 0);
		newKey.supportsType = Key::Type::Smooth;

		mKeys.Add(newKey);
		CheckSmoothKeys();
		UpdateApproximation();

		return mKeys.Count() - 1;
	}

	int Curve::AppendKey(float offset, float value)
	{
		Key newKey((mKeys.IsEmpty() ? 0.0f : mKeys.Last().position) + offset, value, value, 0, value, 0);
		newKey.supportsType = Key::Type::Broken;

		mKeys.Add(newKey);
		CheckSmoothKeys();
		UpdateApproximation();

		return mKeys.Count() - 1;
	}

	int Curve::PrependKey(float offset, float value, float leftCoef, float leftCoefPosition,
						  float rightCoef, float rightCoefPosition)
	{
		float begin = mKeys.IsEmpty() ? 0.0f : mKeys[0].position;

		for (auto& key : mKeys)
			key.position += offset;

		Key newKey(begin, value, leftCoef, leftCoefPosition,
				   rightCoef, rightCoefPosition);

		newKey.supportsType = Key::Type::Broken;

		mKeys.Add(newKey);
		CheckSmoothKeys();
		UpdateApproximation();

		return 0;
	}

	int Curve::PrependKey(float offset, float value, float smoothCoef /*= 1.0f*/)
	{
		float begin = mKeys.IsEmpty() ? 0.0f : mKeys[0].position;

		for (auto& key : mKeys)
			key.position += offset;

		Key newKey(begin, value, value, 0, value, 0);
		newKey.supportsType = Key::Type::Smooth;

		mKeys.Add(newKey);
		CheckSmoothKeys();
		UpdateApproximation();

		return 0;
	}

	int Curve::PrependKey(float offset, float value)
	{
		float begin = mKeys.IsEmpty() ? 0.0f : mKeys[0].position;

		for (auto& key : mKeys)
			key.position += offset;

		Key newKey(begin, value, value, 0, value, 0);
		newKey.supportsType = Key::Type::Broken;

		mKeys.Add(newKey);
		CheckSmoothKeys();
		UpdateApproximation();

		return 0;
	}

	Curve::Key Curve::GetKey(float position)
	{
		for (auto& key : mKeys)
			if (Math::Equals(key.position, position))
				return key;

		return Key();
	}

	Curve::Key Curve::GetKeyAt(int idx)
	{
		if (idx < 0 || idx > mKeys.Count() - 1)
			return Key();

		return mKeys[idx];
	}

	bool Curve::RemoveKey(float position)
	{
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (Math::Equals(mKeys[i].position, position))
			{
				mKeys.RemoveAt(i);
				UpdateApproximation();
				return true;
			}
		}

		return false;
	}

	bool Curve::RemoveKeyAt(int idx)
	{
		if (idx < 0 || idx >= mKeys.Count())
			return false;

		mKeys.RemoveAt(idx);
		UpdateApproximation();
		return true;
	}

	void Curve::RemoveAllKeys()
	{
		mKeys.Clear();
		onKeysChanged();
	}

	bool Curve::ContainsKey(float position)
	{
		for (auto& key : mKeys)
			if (Math::Equals(key.position, position))
				return true;

		return false;
	}

	const Curve::KeysVec& Curve::GetKeys() const
	{
		return mKeys;
	}

	void Curve::SetKeys(const KeysVec& keys)
	{
		mKeys = keys;
		CheckSmoothKeys();
		UpdateApproximation();
	}

	void Curve::SetKey(const Key& key, int position)
	{
		if (position < 0 || position > mKeys.Count() - 1)
			return;

		mKeys[position] = key;
		CheckSmoothKeys();
		UpdateApproximation();
	}

	void Curve::SmoothKey(float position, float smoothCoef)
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

		SmoothKeyAt(pos, smoothCoef);
	}

	void Curve::SmoothKeyAt(int idx, float smoothCoef)
	{
		InternalSmoothKeyAt(idx, smoothCoef);
		UpdateApproximation();
	}

	float Curve::Length() const
	{
		if (mKeys.Count() == 0)
			return 0.0f;

		return mKeys.Last().position - mKeys[0].position;
	}

	bool Curve::IsEmpty() const
	{
		return mKeys.Count() == 0;
	}

	RectF Curve::GetRect() const
	{
		RectF res;

		if (mKeys.IsEmpty())
			return res;

		res.left = mKeys[0].position; res.right = mKeys[0].position;
		res.top = mKeys[0].value; res.bottom = mKeys[0].value;

		for (auto& key : mKeys)
		{
			for (int i = 0; i < Key::mApproxValuesCount; i++)
			{
				res.left = Math::Min(key.mApproxValues[i].x, res.left);
				res.right = Math::Max(key.mApproxValues[i].x, res.right);
				res.top = Math::Min(key.mApproxValues[i].y, res.top);
				res.bottom = Math::Max(key.mApproxValues[i].y, res.bottom);
			}
		}

		return res;
	}

	Curve::Key Curve::operator[](float position)
	{
		for (auto& key : mKeys)
			if (Math::Equals(key.position, position))
				return key;

		return Key();
	}

	Curve Curve::EaseIn()
	{
		return Curve(0.0f, 0.5f, 1.0f, 1.0f);
	}

	Curve Curve::EaseOut()
	{
		return Curve(0.0f, 0.0f, 1.0f, 0.5f);
	}

	Curve Curve::EaseInOut()
	{
		return Curve(0.0f, 0.4f, 1.0f, 0.6f);
	}

	Curve Curve::Linear()
	{
		return Curve(0.0f, 0.0f, 1.0f, 1.0f);
	}

	void Curve::CheckSmoothKeys()
	{
		for (int i = 0; i < mKeys.Count(); i++)
		{
			if (mKeys[i].supportsType == Key::Type::Smooth)
				InternalSmoothKeyAt(i);
		}
	}

	void Curve::UpdateApproximation()
	{
		for (int i = 1; i < mKeys.Count(); i++)
		{
			Key& beginKey = mKeys[i - 1];
			Key& endKey = mKeys[i];

			Vec2F rightSupport(beginKey.rightSupportPosition, beginKey.rightSupportValue);
			Vec2F leftSupport(endKey.leftSupportPosition, endKey.leftSupportValue);

			if (rightSupport.x < 0.0f)
				rightSupport.x = 0;

			if (rightSupport.x > endKey.position - beginKey.position && rightSupport.x != 0.0f)
				rightSupport *= (endKey.position - beginKey.position)/rightSupport.x;

			if (leftSupport.x > 0.0f)
				leftSupport.x = 0;

			if (leftSupport.x < beginKey.position - endKey.position && leftSupport.x != 0.0f)
				leftSupport *= (beginKey.position - endKey.position)/leftSupport.x;

			Vec2F a(beginKey.position, beginKey.value);
			Vec2F d(endKey.position, endKey.value);
			Vec2F b = a + rightSupport;
			Vec2F c = d + leftSupport;

			for (int j = 0; j < Key::mApproxValuesCount; j++)
			{
				float coef = (float)j/(float)(Key::mApproxValuesCount - 1);
				endKey.mApproxValues[j] = Bezier(a, b, c, d, coef);
			}
		}

		onKeysChanged();
	}

	Curve::KeysVec Curve::GetKeysNonContant()
	{
		return mKeys;
	}

	void Curve::OnDeserialized(const DataNode& node)
	{
		UpdateApproximation();
	}

	void Curve::InternalSmoothKeyAt(int idx, float smoothCoef /*= 1.0f*/)
	{
		const float baseSmoothCoef = 0.5f;

		Key& key = mKeys[idx];
		Vec2F thisKeyPoint(key.position, key.value);

		if (idx == 0)
		{
			Key& nextKey = mKeys[Math::Min(idx + 1, mKeys.Count())];

			Vec2F nextKeyPoint(nextKey.position, nextKey.value);
			Vec2F supportVec = (nextKeyPoint - thisKeyPoint)*baseSmoothCoef*smoothCoef;

			key.rightSupportPosition = supportVec.x;
			key.rightSupportValue = supportVec.y;
		}
		else if (idx == mKeys.Count() - 1)
		{
			Key& lastKey = mKeys[Math::Max(0, idx - 1)];

			Vec2F lastKeyPoint(lastKey.position, lastKey.value);
			Vec2F supportVec = (lastKeyPoint - thisKeyPoint)*baseSmoothCoef*smoothCoef;

			key.leftSupportPosition = supportVec.x;
			key.leftSupportValue = supportVec.y;
		}
		else
		{
			Key& lastKey = mKeys[Math::Max(0, idx - 1)];
			Key& nextKey = mKeys[Math::Min(idx + 1, mKeys.Count())];

			Vec2F lastKeyPoint(lastKey.position, lastKey.value);
			Vec2F thisKeyPoint(key.position, key.value);
			Vec2F nextKeyPoint(nextKey.position, nextKey.value);

			Vec2F thisToLast = lastKeyPoint - thisKeyPoint;
			Vec2F thisToNext = nextKeyPoint - thisKeyPoint;

			float supportLength = Math::Min(thisToNext.Length(), thisToLast.Length())*baseSmoothCoef*smoothCoef;
			Vec2F supportVec = Math::CalculateEllipseTangent(lastKeyPoint, thisKeyPoint, nextKeyPoint)*supportLength;

			key.leftSupportPosition = -supportVec.x; key.leftSupportValue = -supportVec.y;
			key.rightSupportPosition = supportVec.x; key.rightSupportValue = supportVec.y;
		}
	}

	void Curve::InternalSmoothKey(float position, float smoothCoef /*= 1.0f*/)
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

		InternalSmoothKeyAt(pos, smoothCoef);
	}

	Curve::Key::Key():
		value(0), position(0), leftSupportValue(0), leftSupportPosition(0), rightSupportValue(0), rightSupportPosition(0),
		supportsType(Type::Smooth)
	{}

	Curve::Key::Key(float position, float value, float leftSupportValue, float leftSupportPosition,
					float rightSupportValue, float rightSupportPosition) :
		value(value), position(position), leftSupportValue(leftSupportValue), leftSupportPosition(leftSupportPosition),
		rightSupportValue(rightSupportValue), rightSupportPosition(rightSupportPosition), supportsType(Type::Broken)
	{}

	Curve::Key::Key(const Key& other) :
		value(other.value), position(other.position), leftSupportValue(other.leftSupportValue),
		leftSupportPosition(other.leftSupportPosition), rightSupportValue(other.rightSupportValue),
		rightSupportPosition(other.rightSupportPosition), supportsType(other.supportsType)
	{
		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));
	}

	Curve::Key::Key(float value):
		value(value), position(0), leftSupportValue(0), leftSupportPosition(0), rightSupportValue(0),
		rightSupportPosition(0), supportsType(Type::Smooth)
	{}

	Curve::Key::operator float() const
	{
		return value;
	}

	const Vec2F* Curve::Key::GetApproximatedPoints() const
	{
		return mApproxValues;
	}

	int Curve::Key::GetApproximatedPointsCount() const
	{
		return mApproxValuesCount;
	}

	Curve::Key& Curve::Key::operator=(float value)
	{
		this->value = value;
		return *this;
	}

	Curve::Key& Curve::Key::operator=(const Key& other)
	{
		value = other.value;
		position = other.position;
		leftSupportValue = other.leftSupportValue;
		leftSupportPosition = other.leftSupportPosition;
		rightSupportValue = other.rightSupportValue;
		rightSupportPosition = other.rightSupportPosition;
		supportsType = other.supportsType;
		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));

		return *this;
	}

	bool Curve::Key::operator!=(const Key& other) const
	{
		return !(*this == other);
	}

	bool Curve::Key::operator==(const Key& other) const
	{
		return Math::Equals(position, other.position) && Math::Equals(value, other.value) &&
			Math::Equals(leftSupportPosition, other.leftSupportPosition) &&
			Math::Equals(leftSupportValue, other.leftSupportValue) &&
			Math::Equals(rightSupportPosition, other.rightSupportPosition) &&
			Math::Equals(rightSupportValue, other.rightSupportValue) &&
			supportsType == other.supportsType;
	}

}

DECLARE_CLASS(o2::Curve);

DECLARE_CLASS(o2::Curve::Key);

ENUM_META_(o2::Curve::Key::Type, Type)
{
	ENUM_ENTRY(Broken);
	ENUM_ENTRY(Discrete);
	ENUM_ENTRY(Flat);
	ENUM_ENTRY(Free);
	ENUM_ENTRY(Smooth);
}
END_ENUM_META;
