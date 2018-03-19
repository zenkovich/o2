#pragma once
// 
// #include "Animation/IAnimation.h"
// 
// namespace o2
// {
// 	// ------------------
// 	// Tweening animation
// 	// ------------------
// 	template<typename _type>
// 	class TweenValue: public IAnimation
// 	{
// 	public:
// 		_type                    leftValue;       // Left tween value
// 		_type                    rightValue;      // Right tween value
// 		Curve                    transitionCurve; // Transition curve from left to right
// 
// 		PROPERTY(float>          duration;         // Tween duration property
// 		PROPERTY(bool>           state;            // State property
// 		GETTER(_type>            value;            // Current value
// 		SETTER(_type*>           targetValuePtr;   // Bind value setter
// 		SETTER(Function<void()>> targetValueEvent; // Bind value event setter
// 		SETTER(SETTER(_type>*>   targetProperty;   // Bind property setter
// 
// 		// Default constructor
// 		TweenValue();
// 
// 		// Constructor
// 		TweenValue(const _type& left, const _type& right, float duration, const Curve& curve);
// 
// 		// Copy-constructor
// 		TweenValue(const TweenValue& other);
// 
// 		// Assign operator
// 		TweenValue& operator=(const TweenValue& other);
// 
// 		// Returns current value
// 		_type GetValue();
// 
// 		// Binds value by pointer
// 		void SetTarget(_type* value);
// 
// 		// Binds value by pointer and sets change event
// 		void SetTarget(_type* value, const Function<void()>& changeEvent);
// 
// 		// Sets binded value change callback
// 		void SetTargetDelegate(const Function<void()>& changeEvent);
// 
// 		// Binds property by pointer
// 		void SetTargetProperty(SETTER(_type>* setter);
// 
// 		// Sets tween duration
// 		void SetDuration(float duration);
// 
// 		// Returns tween duration
// 		float GetDuration();
// 
// 		// Sets tween state and starts playing
// 		void SetState(bool state);
// 
// 		// Returns current tween state
// 		bool GetState();
// 
// 		// Creates tween from left to right in duration with ease in
// 		static TweenValue EaseIn(const _type& left, const _type& right, float duration);
// 
// 		// Creates tween from left to right in duration with ease ou
// 		static TweenValue EaseOut(const _type& left, const _type& right, float duration);
// 
// 		// Creates tween from left to right in duration with ease in-out
// 		static TweenValue EaseInOut(const _type& left, const _type& right, float duration);
// 
// 		// Creates tween from left to right in duration with linear transition
// 		static TweenValue Linear(const _type& left, const _type& right, float duration);
// 
// 	protected:
// 		_type            mValue;                // Current tween value
// 		_type*           mBindValue;            // Binded value pointer, nullptr when no binded value
// 		Function<void()> mBindValueChangeEvent; // Binded value change event
// 		SETTER(_type>*   mBindProperty;         // Binded property pointer, nullptr when no binded property
// 
// 	protected:
// 		// Updates value and bindings
// 		void Evaluate();
// 
// 		// Initializes properties
// 		void InitializeProperties();
// 	};
// 
// 	template<typename _type>
// 	TweenValue<_type>::TweenValue():
// 		mBindValue(nullptr), mBindProperty(nullptr)
// 	{
// 		SetReverse(true);
// 		InitializeProperties();
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type>::TweenValue(const _type& left, const _type& right, float duration, const Curve& curve):
// 		leftValue(left), rightValue(right), transitionCurve(curve), mBindValue(nullptr), mBindProperty(nullptr)
// 	{
// 		float lastDuration = mDuration;
// 		mDuration = duration;
// 
// 		if (Math::Equals(lastDuration, mEndTime))
// 			mEndTime = mDuration;
// 
// 		SetReverse(true);
// 		InitializeProperties();
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type>::TweenValue(const TweenValue& other):
// 		IAnimation(other), leftValue(other.leftValue), rightValue(other.rightValue), transitionCurve(other.transitionCurve),
// 		mBindValue(other.mBindValue), mBindProperty(other.mBindProperty), mBindValueChangeEvent(other.mBindValueChangeEvent)
// 	{
// 		InitializeProperties();
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type>& TweenValue<_type>::operator=(const TweenValue& other)
// 	{
// 		IAnimation::operator =(other);
// 
// 		leftValue = other.leftValue;
// 		rightValue = other.rightValue;
// 		transitionCurve = other.transitionCurve;
// 		mBindValue = other.mBindValue;
// 		mBindProperty = other.mBindProperty;
// 		mBindValueChangeEvent = other.mBindValueChangeEvent;
// 
// 		return *this;
// 	}
// 
// 	template<typename _type>
// 	_type TweenValue<_type>::GetValue()
// 	{
// 		return mValue;
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::SetTarget(_type* value)
// 	{
// 		mBindProperty = nullptr;
// 		mBindValue = value;
// 		mBindValueChangeEvent.Clear();
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::SetTarget(_type* value, const Function<void()>& changeEvent)
// 	{
// 		mBindProperty = nullptr;
// 		mBindValue = value;
// 		mBindValueChangeEvent = changeEvent;
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::SetTargetDelegate(const Function<void()>& changeEvent)
// 	{
// 		mBindValueChangeEvent = changeEvent;
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::SetTargetProperty(SETTER(_type>* setter)
// 	{
// 		mBindValue = nullptr;
// 		mBindValueChangeEvent.Clear();
// 		mBindProperty = setter;
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::SetDuration(float duration)
// 	{
// 		float lastDuration = mDuration;
// 		mDuration = duration;
// 
// 		if (Math::Equals(lastDuration, mEndTime))
// 			mEndTime = mDuration;
// 	}
// 
// 	template<typename _type>
// 	float TweenValue<_type>::GetDuration()
// 	{
// 		return mDuration;
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::SetState(bool state)
// 	{
// 		if (state)
// 			PlayForward();
// 		else
// 			PlayBack();
// 	}
// 
// 	template<typename _type>
// 	bool TweenValue<_type>::GetState()
// 	{
// 		return !IsReversed();
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type> TweenValue<_type>::EaseIn(const _type& left, const _type& right, float duration)
// 	{
// 		return TweenValue(left, right, duration, Curve::EaseIn());
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type> TweenValue<_type>::EaseOut(const _type& left, const _type& right, float duration)
// 	{
// 		return TweenValue(left, right, duration, Curve::EaseOut());
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type> TweenValue<_type>::EaseInOut(const _type& left, const _type& right, float duration)
// 	{
// 		return TweenValue(left, right, duration, Curve::EaseInOut());
// 	}
// 
// 	template<typename _type>
// 	TweenValue<_type> TweenValue<_type>::Linear(const _type& left, const _type& right, float duration)
// 	{
// 		return TweenValue(left, right, duration, Curve::Linear());
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::Evaluate()
// 	{
// 		float coef = mInDurationTime/mDuration;
// 		float curvedCoef = transitionCurve.Evaluate(coef*transitionCurve.Length());
// 
// 		mValue = Math::Lerp(leftValue, rightValue, curvedCoef);
// 
// 		if (mBindValue)
// 		{
// 			*mBindValue = mValue;
// 			mBindValueChangeEvent();
// 		}
// 		else if (mBindProperty)
// 			*mBindProperty = mValue;
// 	}
// 
// 	template<typename _type>
// 	void TweenValue<_type>::InitializeProperties()
// 	{
// 		INITIALIZE_PROPERTY(TweenValue<_type>, duration, SetDuration, GetDuration);
// 		INITIALIZE_PROPERTY(TweenValue<_type>, state, SetState, GetState);
// 		INITIALIZE_GETTER(TweenValue<_type>, value, GetValue);
// 		INITIALIZE_SETTER(TweenValue<_type>, targetValuePtr, SetTarget);
// 		INITIALIZE_SETTER(TweenValue<_type>, targetValueEvent, SetTargetDelegate);
// 		INITIALIZE_SETTER(TweenValue<_type>, targetProperty, SetTargetProperty);
// 	}
// }
// 
//
//
//
