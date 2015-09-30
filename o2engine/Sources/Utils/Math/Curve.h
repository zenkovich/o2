#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Math/Math.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"

namespace o2
{
	class Curve
	{
	public:
		class Key;
		typedef Vector<Key> KeysVec;

	public:
		Accessor<float, float> value;
		Accessor<Key, float>   key;
		Property<KeysVec>      keys;
		Getter<float>          length;
		Function<void()>       onKeysChanged;

		Curve();
		Curve(float beginCoef, float beginCoefPosition, float endCoef, float endCoefPosition);
		Curve(Vector<Vec2F> values, float smooth = 1.0f);
		Curve(const Curve& other);

		Curve& operator=(const Curve& other);

		float Evaluate(float position);

		void AddKeys(Vector<Vec2F> values, float smooth = 1.0f);

		void AddKey(const Key& key);

		void AddKey(float position, float value, float leftCoef, float leftCoefPosition,
					float rightCoef, float rightCoefPosition);

		void AddKey(float position, float value, float smooth = 1.0f);

		Key GetKey(float position);

		bool RemoveKey(float position);

		void RemoveAllKeys();

		bool ContainsKey(float position);

		const KeysVec& GetKeys() const;

		void SetKeys(const KeysVec& keys);

		void SmoothKey(float position, float smooth);

		float Length() const;

		bool IsEmpty() const;

		Key operator[](float position);

		static Curve EaseIn();
		static Curve EaseOut();
		static Curve EaseInOut();
		static Curve Linear();

	public:
		class Key
		{
		public:
			float value;
			float position;
			float leftCoef;
			float leftCoefPosition;
			float rightCoef;
			float rightCoefPosition;

		public:
			Key();
			Key(float position, float value, float leftCoef, float leftCoefPosition,
				float rightCoef, float rightCoefPosition);

			Key(const Key& other);

			Key& operator=(const Key& other);

			bool operator==(const Key& other) const;

		public:
			static const int mApproxValuesCount = 20;
			Vec2F mApproxValues[mApproxValuesCount];

			friend class Curve;
		};

	protected:
		KeysVec mKeys;

		void UpdateApproximation();

		KeysVec GetKeysNonContant();

		void InitializeProperties();
	};
}