#pragma once

#include "Animation/IAnimation.h"

namespace o2
{
	// ------------------
	// Tweening animation
	// ------------------
	template<typename _type>
	class Tween: public IAnimation
	{
	public:
		_type                    leftValue;       // Left tween value
		_type                    rightValue;      // Right tween value
		Curve                    transitionCurve; // Transition curve from left to right

		Property<float>          duration;        // Tween duration property
		Property<bool>           state;           // State property
		Getter<_type>            value;           // Current value
		Setter<_type*>           bindValuePtr;    // Bind value setter
		Setter<Function<void()>> bindValueEvent;  // Bind value event setter
		Setter<Setter<_type>*>   bindProperty;    // Bind property setter

		// Default constructor
		Tween();

		// Constructor
		Tween(const _type& left, const _type& right, float duration, const Curve& curve);

		// Copy-constructor
		Tween(const Tween& other);

		// Assign operator
		Tween& operator=(const Tween& other);

		// Returns current value
		_type GetValue();

		// Binds value by pointer
		void BindValue(_type* value);

		// Binds value by pointer and sets change event
		void BindValue(_type* value, const Function<void()>& changeEvent);

		// Sets binded value change callback
		void SetBindValueChangeEvent(const Function<void()>& changeEvent);

		// Binds property by pointer
		void BindProperty(Setter<_type>* setter);

		// Sets tween duration
		void SetDuration(float duration);

		// Returns tween duration
		float GetDuration();

		// Sets tween state and starts playing
		void SetState(bool state);

		// Returns current tween state
		bool GetState();

		// Creates tween from left to right in duration with ease in
		static Tween EaseIn(const _type& left, const _type& right, float duration);

		// Creates tween from left to right in duration with ease ou
		static Tween EaseOut(const _type& left, const _type& right, float duration);

		// Creates tween from left to right in duration with ease in-out
		static Tween EaseInOut(const _type& left, const _type& right, float duration);

		// Creates tween from left to right in duration with linear transition
		static Tween Linear(const _type& left, const _type& right, float duration);

	protected:
		_type            mValue;                // Current tween value
		_type*           mBindValue;            // Binded value pointer, nullptr when no binded value
		Function<void()> mBindValueChangeEvent; // Binded value change event
		Setter<_type>*   mBindProperty;         // Binded property pointer, nullptr when no binded property

	protected:
		// Updates value and bindings
		void Evaluate();

		// Initializes properties
		void InitializeProperties();
	};

	template<typename _type>
	Tween<_type>::Tween():
		mBindValue(nullptr), mBindProperty(nullptr)
	{
		SetReverse(true);
		InitializeProperties();
	}

	template<typename _type>
	Tween<_type>::Tween(const _type& left, const _type& right, float duration, const Curve& curve):
		leftValue(left), rightValue(right), transitionCurve(curve), mBindValue(nullptr), mBindProperty(nullptr)
	{
		float lastDuration = mDuration;
		mDuration = duration;

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;

		SetReverse(true);
		InitializeProperties();
	}

	template<typename _type>
	Tween<_type>::Tween(const Tween& other):
		IAnimation(other), leftValue(other.leftValue), rightValue(other.rightValue), transitionCurve(other.transitionCurve),
		mBindValue(other.mBindValue), mBindProperty(other.mBindProperty), mBindValueChangeEvent(other.mBindValueChangeEvent)
	{
		InitializeProperties();
	}

	template<typename _type>
	Tween<_type>& Tween<_type>::operator=(const Tween& other)
	{
		IAnimation::operator =(other);

		leftValue = other.leftValue;
		rightValue = other.rightValue;
		transitionCurve = other.transitionCurve;
		mBindValue = other.mBindValue;
		mBindProperty = other.mBindProperty;
		mBindValueChangeEvent = other.mBindValueChangeEvent;

		return *this;
	}

	template<typename _type>
	_type Tween<_type>::GetValue()
	{
		return mValue;
	}

	template<typename _type>
	void Tween<_type>::BindValue(_type* value)
	{
		mBindProperty = nullptr;
		mBindValue = value;
		mBindValueChangeEvent.Clear();
	}

	template<typename _type>
	void Tween<_type>::BindValue(_type* value, const Function<void()>& changeEvent)
	{
		mBindProperty = nullptr;
		mBindValue = value;
		mBindValueChangeEvent = changeEvent;
	}

	template<typename _type>
	void Tween<_type>::SetBindValueChangeEvent(const Function<void()>& changeEvent)
	{
		mBindValueChangeEvent = changeEvent;
	}

	template<typename _type>
	void Tween<_type>::BindProperty(Setter<_type>* setter)
	{
		mBindValue = nullptr;
		mBindValueChangeEvent.Clear();
		mBindProperty = setter;
	}

	template<typename _type>
	void Tween<_type>::SetDuration(float duration)
	{
		float lastDuration = mDuration;
		mDuration = duration;

		if (Math::Equals(lastDuration, mEndTime))
			mEndTime = mDuration;
	}

	template<typename _type>
	float Tween<_type>::GetDuration()
	{
		return mDuration;
	}

	template<typename _type>
	void Tween<_type>::SetState(bool state)
	{
		if (state)
			PlayForward();
		else
			PlayBack();
	}

	template<typename _type>
	bool Tween<_type>::GetState()
	{
		return !IsReversed();
	}

	template<typename _type>
	Tween<_type> Tween<_type>::EaseIn(const _type& left, const _type& right, float duration)
	{
		return Tween(left, right, duration, Curve::EaseIn());
	}

	template<typename _type>
	Tween<_type> Tween<_type>::EaseOut(const _type& left, const _type& right, float duration)
	{
		return Tween(left, right, duration, Curve::EaseOut());
	}

	template<typename _type>
	Tween<_type> Tween<_type>::EaseInOut(const _type& left, const _type& right, float duration)
	{
		return Tween(left, right, duration, Curve::EaseInOut());
	}

	template<typename _type>
	Tween<_type> Tween<_type>::Linear(const _type& left, const _type& right, float duration)
	{
		return Tween(left, right, duration, Curve::Linear());
	}

	template<typename _type>
	void Tween<_type>::Evaluate()
	{
		float coef = mInDurationTime/mDuration;
		float curvedCoef = transitionCurve.Evaluate(coef*transitionCurve.Length());

		mValue = Math::Lerp(leftValue, rightValue, curvedCoef);

		if (mBindValue)
		{
			*mBindValue = mValue;
			mBindValueChangeEvent();
		}
		else if (mBindProperty)
			*mBindProperty = mValue;
	}

	template<typename _type>
	void Tween<_type>::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Tween<_type>, duration, SetDuration, GetDuration);
		INITIALIZE_PROPERTY(Tween<_type>, state, SetState, GetState);
		INITIALIZE_GETTER(Tween<_type>, value, GetValue);
		INITIALIZE_SETTER(Tween<_type>, bindValuePtr, BindValue);
		INITIALIZE_SETTER(Tween<_type>, bindValueEvent, SetBindValueChangeEvent);
		INITIALIZE_SETTER(Tween<_type>, bindProperty, BindProperty);
	}
}
