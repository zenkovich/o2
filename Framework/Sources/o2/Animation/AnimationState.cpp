#include "o2/stdafx.h"
#include "AnimationState.h"

#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
	AnimationState::AnimationState():
		weight(1.0f), workWeight(1.0f), mOwner(nullptr)
	{}

	AnimationState::AnimationState(const String& name) :
		name(name), weight(1.0f), workWeight(1.0f), mOwner(nullptr)
	{}
}

DECLARE_CLASS(o2::AnimationState);
