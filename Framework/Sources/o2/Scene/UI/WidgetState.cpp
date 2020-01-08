#include "o2/stdafx.h"
#include "WidgetState.h"

#include "o2/Scene/UI/Widget.h"

namespace o2
{
	WidgetState::WidgetState()
	{}

	WidgetState::WidgetState(const WidgetState& state):
		name(state.name), mState(state.mState), mOwner(state.mOwner),
		offStateAnimationSpeed(state.offStateAnimationSpeed), state(this)
	{
		animation = state.animation;
		animation.SetTarget(mOwner, false);
		animation.relTime = mState ? 1.0f:0.0f;
	}

	WidgetState::~WidgetState()
	{}

	WidgetState::operator bool()
	{
		return GetState();
	}

	void WidgetState::SetState(bool state)
	{
 		if (mState == state && !animation.IsPlaying())
 			return;

		mState = state;

		if (state)
		{
			animation.speed = 1.0f;
			animation.PlayForward();

			onStateBecomesTrue();
		}
		else
		{
			animation.speed = offStateAnimationSpeed;
			animation.PlayBack();

			onStateBecomesFalse();
		}
	}

	void WidgetState::SetStateForcible(bool state)
	{
		if (mState == state)
		{
			if ((state && Math::Equals(animation.GetRelTime(), 1.0f)) ||
				(!state && Math::Equals(animation.GetRelTime(), 0.0f)))
			{
				return;
			}
		}

		mState = state;

		if (mState)
		{
			animation.GoToEnd();
			animation.Stop();
			onStateBecomesTrue();
			onStateFullyTrue();
		}
		else
		{
			animation.GoToBegin();
			animation.Stop();
			onStateBecomesFalse();
			onStateFullyFalse();
		}
	}

	bool WidgetState::GetState() const
	{
		return mState;
	}

	void WidgetState::Update(float dt)
	{
		if (animation.IsPlaying())
		{
			animation.Update(dt);

			if (!animation.IsPlaying())
			{
				if (mState) onStateFullyTrue();
				else onStateFullyFalse();
			}
		}
	}

	WidgetState& WidgetState::operator=(bool state)
	{
		SetState(state);
		return *this;
	}
}

DECLARE_CLASS(o2::WidgetState);
