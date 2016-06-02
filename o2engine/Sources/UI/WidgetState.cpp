#include "WidgetState.h"

#include "UI/Widget.h"

namespace o2
{
	UIWidgetState::UIWidgetState()
	{}

	UIWidgetState::UIWidgetState(const UIWidgetState& state):
		name(state.name), animation(state.animation), mState(state.mState), mOwner(state.mOwner),
		offStateAnimationSpeed(state.offStateAnimationSpeed)
	{
		animation.SetTarget(mOwner);
		animation.relTime = mState ? 1.0f:0.0f;
	}

	UIWidgetState::~UIWidgetState()
	{}

	UIWidgetState::operator bool()
	{
		return GetState();
	}

	void UIWidgetState::SetState(bool state)
	{
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

	void UIWidgetState::SetStateForcible(bool state)
	{
		mState = state;

		if (mState)
		{
			animation.GoToEnd();
			onStateBecomesTrue();
			onStateFullyTrue();
		}
		else
		{
			animation.GoToBegin();
			onStateBecomesFalse();
			onStateFullyFalse();
		}
	}

	bool UIWidgetState::GetState() const
	{
		return mState;
	}

	void UIWidgetState::Update(float dt)
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

	UIWidgetState& UIWidgetState::operator=(bool state)
	{
		SetState(state);
		return *this;
	}
}