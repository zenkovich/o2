#include "AnimationState.h"

namespace o2
{
	AnimationState::AnimationState():
		weight(1.0f), workWeight(1.0f), mOwner(nullptr)
	{}

	AnimationState::AnimationState(const String& name) :
		name(name), weight(1.0f), workWeight(1.0f), mOwner(nullptr)
	{}
}