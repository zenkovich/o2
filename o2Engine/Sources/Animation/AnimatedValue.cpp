#include "stdafx.h"
#include "AnimatedValue.h"

#include "Animation/Animatable.h"
#include "Utils/Math/Color.h"

namespace o2
{
	void AnimatedValue<RectF>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<RectF>(this, path, state);
	}

	void AnimatedValue<bool>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<bool>(this, path, state);
	}

	void AnimatedValue<Color4>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<Color4>(this, path, state);
	}
}
DECLARE_CLASS_MANUAL(o2::AnimatedValue<bool>);
DECLARE_CLASS_MANUAL(o2::AnimatedValue<bool>::Key);
DECLARE_CLASS_MANUAL(o2::AnimatedValue<o2::Color4>);
DECLARE_CLASS_MANUAL(o2::AnimatedValue<o2::Color4>::Key);

DECLARE_CLASS(o2::IAnimatedValue);
