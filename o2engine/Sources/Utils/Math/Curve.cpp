#include "Curve.h"

#include "Utils/Math/Interpolation.h"

namespace o2
{
	IOBJECT_CPP(Curve);
	IOBJECT_CPP(Curve::Key);

	Curve::Curve()
	{
		InitializeProperties();
	}

	Curve::Curve(float beginCoef, float beginCoefPosition, float endCoef, float endCoefPosition)
	{
		mKeys.Add(Key(0.0f, 0.0f, 0.0f, 0.0f, beginCoef, Math::Clamp01(beginCoefPosition)));
		mKeys.Add(Key(1.0f, 1.0f, endCoef, Math::Clamp01(endCoefPosition), 0.0f, 0.0f));
		UpdateApproximation();
		InitializeProperties();
	}

	Curve::Curve(Vector<Vec2F> values, float smooth /*= 1.0f*/)
	{
		AddKeys(values, smooth);
		InitializeProperties();
	}

	Curve::Curve(const Curve& other):
		mKeys(other.mKeys)
	{
		InitializeProperties();
	}

	Curve& Curve::operator=(const Curve& other)
	{
		mKeys = other.mKeys;
		onKeysChanged();
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

	void Curve::AddKeys(Vector<Vec2F> values, float smooth /*= 1.0f*/)
	{
		for (auto val : values)
			AddKey(val.x, val.y, smooth);

		for (auto val : values)
			SmoothKey(val.x, smooth);

		UpdateApproximation();
	}

	void Curve::AddKey(const Key& key)
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

		UpdateApproximation();
	}

	void Curve::AddKey(float position, float value, float leftCoef, float leftCoefPosition, 
					   float rightCoef, float rightCoefPosition)
	{
		AddKey(Key(position, value, leftCoef, leftCoefPosition, rightCoef, rightCoefPosition));
	}

	void Curve::AddKey(float position, float value, float smooth /*= 1.0f*/)
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
	}

	Curve::Key Curve::GetKey(float position)
	{
		for (auto& key : mKeys)
			if (Math::Equals(key.position, position))
				return key;

		return Key();
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
		UpdateApproximation();
	}

	void Curve::SmoothKey(float position, float smooth)
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

		if (pos > 0 && pos < mKeys.Count() - 1)
		{
			float baseSmooth = 0.3333f;
			float resSmooth = baseSmooth*smooth;

			Key& prevKey = mKeys[pos - 1];
			Key& curKey  = mKeys[pos];
			Key& nextKey = mKeys[pos + 1];

			Vec2F prev = Vec2F(prevKey.position, prevKey.value);
			Vec2F cur = Vec2F(curKey.position, curKey.value);
			Vec2F next = Vec2F(nextKey.position, nextKey.value);

			Vec2F prevCur = prev - cur;
			Vec2F nextCur = next - cur;

			Vec2F nd = (prevCur.Normalized() + nextCur.Normalized()).Perpendicular().Normalized();
			if (nd == Vec2F()) nd = prevCur.Normalized();
			nd *= Math::Sign(nd.x);

			Vec2F prevCoef = cur + nd*resSmooth*prevCur.x;
			Vec2F nextCoef = cur + nd*resSmooth*nextCur.x;

			curKey.leftCoefPosition = -(prevCoef.x - prev.x)/prevCur.x;
			curKey.leftCoef = prevCoef.y;

			curKey.rightCoefPosition = (nextCoef.x - cur.x)/nextCur.x;
			curKey.rightCoef = nextCoef.y;
		}

		UpdateApproximation();
	}

	float Curve::Length() const
	{
		if (mKeys.Count() == 0)
			return 0.0f;

		return mKeys.Last().position;
	}

	bool Curve::IsEmpty() const
	{
		return mKeys.Count() == 0;
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

	void Curve::UpdateApproximation()
	{
		for (int i = 1; i < mKeys.Count(); i++)
		{
			Key& beginKey = mKeys[i - 1];
			Key& endKey = mKeys[i];

			beginKey.leftCoefPosition = Math::Clamp01(beginKey.leftCoefPosition);
			endKey.rightCoefPosition = Math::Clamp01(endKey.rightCoefPosition);

			float dist = endKey.position - beginKey.position;

			Vec2F a(beginKey.position, beginKey.value);
			Vec2F b(Math::Lerp(beginKey.position, endKey.position, beginKey.rightCoefPosition), beginKey.rightCoef);
			Vec2F c(Math::Lerp(beginKey.position, endKey.position, endKey.leftCoefPosition), endKey.leftCoef);
			Vec2F d(endKey.position, endKey.value);

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

	void Curve::InitializeProperties()
	{
		INITIALIZE_ACCESSOR(Curve, value, Evaluate);
		INITIALIZE_ACCESSOR(Curve, key, GetKey);
		INITIALIZE_PROPERTY(Curve, keys, SetKeys, GetKeysNonContant);
		INITIALIZE_GETTER(Curve, length, Length);
	}

	Curve::Key::Key():
		value(0), position(0), leftCoef(0), leftCoefPosition(0), rightCoef(0), rightCoefPosition(0)
	{}

	Curve::Key::Key(float position, float value, float leftCoef, float leftCoefPosition,
					float rightCoef, float rightCoefPosition):
		value(value), position(position), leftCoef(leftCoef), leftCoefPosition(leftCoefPosition), rightCoef(rightCoef),
		rightCoefPosition(rightCoefPosition)
	{}

	Curve::Key::Key(const Key& other):
		value(other.value), position(other.position), leftCoef(other.leftCoef), leftCoefPosition(other.leftCoefPosition), 
		rightCoef(other.rightCoef), rightCoefPosition(other.rightCoefPosition)
	{
		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));
	}

	Curve::Key::Key(float value):
		value(value), position(0), leftCoef(0), leftCoefPosition(0), rightCoef(0), rightCoefPosition(0)
	{}

	Curve::Key::operator float() const
	{
		return value;
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
		leftCoef = other.leftCoef;
		leftCoefPosition = other.leftCoefPosition;
		rightCoef = other.rightCoef;
		rightCoefPosition = other.rightCoefPosition;
		memcpy(mApproxValues, other.mApproxValues, mApproxValuesCount*sizeof(Vec2F));

		return *this;
	}

	bool Curve::Key::operator==(const Key& other) const
	{
		return Math::Equals(position, other.position) && Math::Equals(value, other.value);
	}

}