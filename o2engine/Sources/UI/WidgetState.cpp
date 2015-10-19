#include "WidgetState.h"

#include "UI/Widget.h"

namespace o2
{
	IOBJECT_CPP(WidgetState);

	WidgetState::WidgetState():
		mState(false), offStateAnimationSpeed(1)
	{}

	WidgetState::WidgetState(const WidgetState& state):
		name(state.name), animation(state.animation), mState(state.mState), mOwner(state.mOwner),
		offStateAnimationSpeed(state.offStateAnimationSpeed)
	{
		animation.SetTarget(mOwner);
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
		mState = state;

		if (state)
		{
			animation.speed = 1.0f;
			animation.PlayForward();
		}
		else
		{
			animation.speed = offStateAnimationSpeed;
			animation.PlayBack();
		}

	}

	bool WidgetState::GetState() const
	{
		return mState;
	}

	void WidgetState::Update(float dt)
	{
		animation.Update(dt);
	}

	WidgetState& WidgetState::operator=(bool state)
	{
		SetState(state);
		return *this;
	}
}