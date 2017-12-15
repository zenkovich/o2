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

	void UIWidgetState::SetStateForcible(bool state)
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

CLASS_META(o2::UIWidgetState)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(animation).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(offStateAnimationSpeed).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onStateFullyTrue);
	PUBLIC_FIELD(onStateFullyFalse);
	PUBLIC_FIELD(onStateBecomesTrue);
	PUBLIC_FIELD(onStateBecomesFalse);
	PROTECTED_FIELD(mState).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mOwner);

	PUBLIC_FUNCTION(void, SetState, bool);
	PUBLIC_FUNCTION(void, SetStateForcible, bool);
	PUBLIC_FUNCTION(bool, GetState);
	PUBLIC_FUNCTION(void, Update, float);
}
END_META;
