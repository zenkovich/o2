#pragma once

#include "Animation/IAnimation.h"
#include "Utils/Math/Curve.h"

namespace o2
{
	class IAnimatedValue: public IAnimation
	{
	public:
		virtual void SetTarget(void* target) {}
		virtual void SetTarget(void* target, const Function<void()>& changeEvent) {}
		virtual void SetTargetDelegate(const Function<void()>& changeEvent) {}
		virtual void SetTargetProperty(void* target) {}
	};

	template<typename _type>
	class AnimatedValue;

	template<>
	class AnimatedValue<float>: public IAnimatedValue
	{
	public:
		Curve                       curve;
		Getter<float>               value;
		Setter<float*>              target;
		Setter<Function<void()>>    targetDelegate;
		Setter<Setter<float>*>      targetProperty;
		Accessor<Curve::Key, float> key;
		Property<Curve::KeysVec>    keys;

		AnimatedValue();

		AnimatedValue(const AnimatedValue<float>& other);

		AnimatedValue<float>& operator=(const AnimatedValue<float>& other);

		void SetTarget(float* value);

		void SetTarget(float* value, const Function<void()>& changeEvent);

		void SetTargetDelegate(const Function<void()>& changeEvent);

		void SetTargetProperty(Setter<float>* setter);

		float GetValue();

		void AddKeys(Vector<Vec2F> values, float smooth = 1.0f);

		void AddKey(const Curve::Key& key);

		void AddKey(float position, float value, float leftCoef, float leftCoefPosition,
					float rightCoef, float rightCoefPosition);

		void AddKey(float position, float value, float smooth = 1.0f);

		Curve::Key GetKey(float position);

		bool RemoveKey(float position);

		void RemoveAllKeys();

		bool ContainsKey(float position);

		const Curve::KeysVec& GetKeys() const;

		void SetKeys(const Curve::KeysVec& keys);

		void SmoothKey(float position, float smooth);

		Curve::Key operator[](float position);

		static AnimatedValue<float> Parametric(float begin, float end, float duration,
											   float beginCoef, float beginCoefPosition,
											   float endCoef, float endCoefPosition);

		static AnimatedValue<float> EaseIn(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);
		static AnimatedValue<float> EaseOut(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);
		static AnimatedValue<float> EaseInOut(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);
		static AnimatedValue<float> Linear(float begin = 0.0f, float end = 1.0f, float duration = 1.0f);

	protected:
		float            mValue;
		float*           mTarget;
		Function<void()> mTargetDelegate;
		Setter<float>*   mTargetProperty;

	protected:
		void Evaluate();
		Curve::KeysVec GetKeysNonContant();
		void OnCurveChanged();

		void InitializeProperties();
	};
};
