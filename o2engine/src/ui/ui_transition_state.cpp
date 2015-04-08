#include "ui_transition_state.h"

#include "util/callback.h"

OPEN_O2_NAMESPACE

UITransitionState::UITransitionState(const String& name):
UIState(name), mState(false), mChangingState(false)
{
}

UITransitionState::UITransitionState(const UITransitionState& transitionState):
UIState(transitionState), mChangingState(false)
{
	foreach_const(transitionState.mProperties, prop)
		AddProperty((*prop)->Clone());

	SetState(transitionState.mState, true);
}

UITransitionState::~UITransitionState()
{
	ReleaseArray(mProperties);
}

UIState* UITransitionState::Clone() const
{
	UITransitionState* res = mnew UITransitionState(*this);
	return res;
}

void UITransitionState::SetState(bool state, bool forcible)
{
	if (!forcible && mState == state)
		return;

	mState = state;
	mChangingState = !forcible;

	if (state)
		onBeginActiveStateEvent.Invoke();
	else
		onBeginDeactiveStateEvent.Invoke();

	foreach(mProperties, prop)
		(*prop)->setState(state, forcible);

	if (forcible)
	{
		if (state)
			onActiveStateEvent.Invoke();
		else
			onDeactiveStateEvent.Invoke();
	}
}

bool UITransitionState::GetState() const
{
	return mState;
}

void UITransitionState::Update(float dt)
{
	if (!mChangingState)
		return;

	bool complete = true;
	foreach(mProperties, prop)
	{
		if ((*prop)->IsComplete())
			continue;

		(*prop)->Update(dt);
		complete = false;
	}

	mChangingState = !complete;
	if (complete)
	{
		if (mState)
			onActiveStateEvent.Invoke();
		else
			onDeactiveStateEvent.Invoke();
	}
}

UITransitionState::IProperty* UITransitionState::AddProperty(IProperty* property)
{
	mProperties.Add(property);

	if (mOwnerWidget)
		property->SetOwner(this);

	return property;
}

void UITransitionState::SetOwnerWidget(UIWidget* ownerWidget)
{
	UIState::SetOwnerWidget(ownerWidget);
	foreach(mProperties, prop)
		(*prop)->SetOwner(this);

	SetState(GetState(), true);
}

CLOSE_O2_NAMESPACE