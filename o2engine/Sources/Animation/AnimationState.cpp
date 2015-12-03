#include "AnimationState.h"

namespace o2
{
	IOBJECT_CPP(AnimationState);

	AnimationState::AnimationState():
		weight(1.0f), workWeight(1.0f)
	{}

	AnimationState::AnimationState(const String& name) :
		name(name), weight(1.0f), workWeight(1.0f)
	{}
}