#pragma once

#include "ui_state.h"
#include "util/animation/frame.h"
#include "util/animation/frame_interpolation.h"
#include "util/property_list.h"

OPEN_O2_NAMESPACE

/** Widget transition state. Containing properties, what changing widgets properties. */
class UITransitionState: public UIState
{
public:
	/**Property interface. */
	class IProperty
	{
	public:
		/** Returns clone of property. */
		virtual IProperty* Clone() const = 0;

		/** Updates property changing. */
		virtual void Update(float dt) = 0;

		/** Setting property state. */
		virtual void SetState(bool state, bool forcible = false) = 0;

		/** Returns property state. */
		virtual bool IsComplete() = 0;

		/** Sets transition state owner. */
		virtual void SetOwner(UITransitionState* owner) = 0;
	};

	typedef Array<IProperty*> PropertiesArr;

protected:
	PropertiesArr mProperties;    /** Properties array. */
	bool          mState;         /** Current state. */
	bool          mChangingState; /** True, when state changing. */

public:
	/** ctor. */
	UITransitionState(const String& name);

	/** copy-ctor. */
	UITransitionState(const UITransitionState& transitionState);

	/** dtor. */
	~UITransitionState();

	/** Returns a copy. */
	UIState* Clone() const;

	/** Sets current state. */
	void SetState(bool state, bool forcible = false);

	/** Returns state. */
	bool GetState() const;

	/** Adding property. */
	IProperty* AddProperty(IProperty* property);

	/** Updating state. */
	void Update(float dt);

protected:
	/** Calls when setting owner widget. Here updating properties. */
	void SetOwnerWidget(UIWidget* ownerWidget);

public:
	/** Template property value. Contains animation frames of On/OFF states. */
	template<typename T>
	class ValueProperty: public UITransitionState::IProperty
	{
		PropertyList::PropertyBase<T>* mProperty;
		AnimationFrame<T>                   mStateOff;         /** Off state animation frame. */
		AnimationFrame<T>                   mStateOn;          /** On State animation frame. */
		FrameInterpolation<T>          mInterpolator;     /** Frame interpolator. */
		float                          mCoef;             /** Interpolation coeficient. 0 - off state, 1 - on state. */
		float                          mCoefTime;         /** Coef time multiplier. */
		bool                           mTargetState;      /** Target state. */
		bool                           mComplete;         /** True, if animation completed. */
		ICallback*                     mOnChanged;        /** On change callback. */
		String                         mWidgetPropertyId; /** Widget property id. */
		UITransitionState*             mStateOwner;       /** Transition owner state. */

	public:
		/** ctor. */
		ValueProperty() {}

		static ValueProperty<T>* Create(PropertyList::PropertyBase<T>* prop, const T& stateOff, const T& stateOn,
										float duration, ICallback* onChanged = NULL)
		{
			ValueProperty<T>* res = mnew ValueProperty<T>();

			res->mProperty = prop;
			res->mStateOff = AnimationFrame<T>(stateOff, duration);
			res->mStateOn = AnimationFrame<T>(stateOn, duration);
			res->mInterpolator.Initialize(&res->mStateOff, &res->mStateOn);
			res->mCoef = 0;
			res->mCoefTime = 1.0f;
			res->mTargetState = false;
			res->mComplete = true;
			res->mOnChanged = onChanged;
			res->mWidgetPropertyId = prop->GetPath();
			res->mStateOwner = NULL;

			return res;
		}

		static ValueProperty<T>* Create(PropertyList::PropertyBase<T>* prop,
										const AnimationFrame<T>& stateOff, const AnimationFrame<T>& stateOn,
										ICallback* onChanged = NULL)
		{
			ValueProperty<T>* res = mnew ValueProperty<T>();

			res->mProperty = prop;
			res->mStateOff = stateOff;
			res->mStateOn = stateOn;
			res->mInterpolator.Initialize(&res->mStateOff, &res->mStateOn);
			res->mCoef = 0;
			res->mCoefTime = 1.0f;
			res->mTargetState = false;
			res->mComplete = true;
			res->mOnChanged = onChanged;
			res->mWidgetPropertyId = prop->GetPath();
			res->mStateOwner = NULL;

			return res;
		}

		/** ctor. */
		ValueProperty(const ValueProperty& prop)
		{
			mProperty = prop.mProperty;
			mStateOff = prop.mStateOff;
			mStateOn = prop.mStateOn;
			mInterpolator.Initialize(&mStateOff, &mStateOn);
			mCoef = prop.mCoef;
			mCoefTime = prop.mCoefTime;
			mTargetState = prop.mTargetState;
			mComplete = prop.mComplete;
			mOnChanged = prop.mOnChanged;
			mWidgetPropertyId = prop.mWidgetPropertyId;
			mStateOwner = NULL;
		}

		/** Return a copy. */
		UITransitionState::IProperty* Clone() const
		{
			return mnew ValueProperty(*this);
		}

		/** Updates animation. */
		void Update(float dt)
		{
			if (mTargetState)
			{
				mCoef += dt*mCoefTime;
				if (mCoef > 1.0f)
				{
					mComplete = true;
					mCoef = 1.0f;
				}
			}
			else
			{
				mCoef -= dt*mCoefTime;
				if (mCoef < 0)
				{
					mCoef = 0;
					mComplete = true;
				}
			}

			*mProperty = mInterpolator.GetValue(mCoef);

			//printf("coef %.3f value %.3f %s\n", mCoef, mInterpolator.getValue(mCoef), mWidgetPropertyId.c_str());

			mProperty->onChangeEvent.Invoke();

			if (mOnChanged)
				mOnChanged->Invoke();
		}

		/** Setting state. */
		void SetState(bool state, bool forcible = false)
		{
			if (!mProperty)
				return;

			if (forcible)
			{
				mComplete = true;

				if (mStateOwner)
				{
					*mProperty = mInterpolator.GetValue(state ? 1.0f:0.0f);
					mProperty->onChangeEvent.Invoke();
				}

				mCoef = state ? 1.0f:0.0f;

				return;
			}

			mComplete = false;
			mTargetState = state;
			if (mTargetState)
			{
				mCoefTime = 1.0f/mStateOn.mTime;
				//mTime = 0;
			}
			else
			{
				mCoefTime = 1.0f/mStateOff.mTime;
				//mTime = mStateOff.mTime;
			}
		}

		/** Returns true, if animation is complete. */
		bool IsComplete()
		{
			return mComplete;
		}

		/** Sets state owner. */
		void SetOwner(UITransitionState* owner)
		{
			mStateOwner = owner;
			mProperty = owner->mOwnerWidget->GetProperty<T>(mWidgetPropertyId);
		}
	};


	/** Adding property. */
	template<typename T>
	ValueProperty<T>* AddProperty_(ValueProperty<T>* property)
	{
		AddProperty(property);
		return property;
	}

	/** Adding property. */
	template<typename T>
	ValueProperty<T>* AddProperty(PropertyList::PropertyBase<T>* prop, const T& stateOff, const T& stateOn, float duration,
								  ICallback* onChanged = NULL)
	{
		return AddProperty_(ValueProperty<T>::Create(prop, stateOff, stateOn, duration, onChanged));
	}

	/** Adding property. */
	template<typename T>
	ValueProperty<T>* AddProperty(PropertyList::PropertyBase<T>* prop, const AnimationFrame<T>& stateOff, const AnimationFrame<T>& stateOn,
								  ICallback* onChanged = NULL)
	{
		return AddProperty_(ValueProperty<T>::Create(prop, stateOff, stateOn, onChanged));
	}
};

CLOSE_O2_NAMESPACE
