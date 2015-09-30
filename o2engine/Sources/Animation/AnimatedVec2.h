#pragma once

#include "AnimatedValue.h"

namespace o2
{
	template<>
	class AnimatedValue<Vec2F>: public IAnimatedValue
	{
	public:
		class Key;
		typedef Vector<Key> KeysVec;

	public:
		Getter<Vec2F>               value;
		Setter<Vec2F*>              target;
		Setter<Function<void()>>    targetDelegate;
		Setter<Setter<Vec2F>*>      targetProperty;
		Accessor<Key, float>        key;
		Property<KeysVec>           keys;

		AnimatedValue();

		AnimatedValue(const AnimatedValue<Vec2F>& other);

		AnimatedValue<Vec2F>& operator=(const AnimatedValue<Vec2F>& other);

		void SetTarget(Vec2F* value);

		void SetTarget(Vec2F* value, const Function<void()>& changeEvent);

		void SetTargetDelegate(const Function<void()>& changeEvent);

		void SetTargetProperty(Setter<Vec2F>* setter);

		Vec2F GetValue();

		void AddKeys(Vector<Key> keys, float smooth = 1.0f);

		void AddKey(const Key& key);

		void AddKey(const Key& key, float smooth);

		void AddKey(float position, const Vec2F& value, const Vec2F& prevSupport, const Vec2F& nextSupport,
					float leftCoef, float leftCoefPosition, float rightCoef, float rightCoefPosition);

		void AddKey(float position, const Vec2F& value, float smooth = 1.0f);

		Key GetKey(float position);

		bool RemoveKey(float position);

		void RemoveAllKeys();

		bool ContainsKey(float position);

		const KeysVec& GetKeys() const;

		void SetKeys(const KeysVec& keys);

		void SmoothKey(float position, float smooth);

		Key operator[](float position);

		static AnimatedValue<Vec2F> Parametric(const Vec2F& begin, const Vec2F& end, float duration,
											   float beginCoef, float beginCoefPosition,
											   float endCoef, float endCoefPosition);

		static AnimatedValue<Vec2F> EaseIn(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);
		static AnimatedValue<Vec2F> EaseOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);
		static AnimatedValue<Vec2F> EaseInOut(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);
		static AnimatedValue<Vec2F> Linear(const Vec2F& begin, const Vec2F& end, float duration = 1.0f);

	public:
		class Key
		{
		public:
			float position;
			Vec2F value;
			Vec2F prevSupportValue;
			Vec2F nextSupportValue;
			float curvePrevCoef;
			float curvePrevCoefPos;
			float curveNextCoef;
			float curveNextCoefPos;

		public:
			Key();
			Key(float position, const Vec2F& value);
			Key(float position, const Vec2F& value, const Vec2F& prevSupportValue, const Vec2F& nextSupportValue, 
				float curvePrevCoef, float curvePrevCoefPos, float curveNextCoef, float curveNextCoefPos);

			Key(const Key& other);

			Key& operator=(const Key& other);

			bool operator==(const Key& other) const;

		public:
			static const int mApproxValuesCount = 20;
			Vec2F mApproxValues[mApproxValuesCount];
			Vec2F mCurveApproxValues[mApproxValuesCount];
			float mApproxLengths[mApproxValuesCount];
			float mApproxTotalLength;
		};

	protected:
		KeysVec          mKeys;
		Vec2F            mValue;
		Vec2F*           mTarget;
		Function<void()> mTargetDelegate;
		Setter<Vec2F>*   mTargetProperty;

	protected:
		void Evaluate();
		Vec2F Evaluate(float position);
		KeysVec GetKeysNonContant();
		void UpdateApproximation();

		void InitializeProperties();
	};
}
