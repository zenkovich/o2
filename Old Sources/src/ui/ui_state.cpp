#include "ui_state.h"

OPEN_O2_NAMESPACE

UIState::UIState(const String& name):
mName(name), mOwnerWidget(NULL)
{
	InitializeProperties();
}

UIState::UIState(const UIState& state)
{
	mName = state.mName;
	mOwnerWidget = NULL;
	InitializeProperties();
}

UIState::~UIState()
{
}

void UIState::InitializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(UIState, state, SetStateNonForcible, GetState);
}

void UIState::SetOwnerWidget(UIWidget* ownerWidget)
{
	mOwnerWidget = ownerWidget;
}

void UIState::SetStateNonForcible(bool state)
{
	SetState(state, false);
}

String UIState::GetName() const
{
	return mName;
}

CLOSE_O2_NAMESPACE