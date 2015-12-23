#include "AnimatedValue.h"

#include "Animation/Animatable.h"

namespace o2
{
	void AnimatedValue<RectF>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<RectF>(this, path, state);
	}
}