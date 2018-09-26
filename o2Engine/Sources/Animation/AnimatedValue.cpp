#include "stdafx.h"
#include "AnimatedValue.h"

#include "Animation/Animatable.h"
#include "Utils/Math/Color.h"

namespace o2
{
	template<>
	void AnimatedValue<RectF>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<RectF>(this, path, state);
	}

    template<>
	void AnimatedValue<bool>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<bool>(this, path, state);
	}

    template<>
	void AnimatedValue<Color4>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<Color4>(this, path, state);
	}
}

template<>
DECLARE_CLASS_MANUAL(o2::AnimatedValue<bool>);

template<>
DECLARE_CLASS_MANUAL(o2::AnimatedValue<bool>::Key);

template<>
DECLARE_CLASS_MANUAL(o2::AnimatedValue<o2::Color4>);

template<>
DECLARE_CLASS_MANUAL(o2::AnimatedValue<o2::Color4>::Key);

DECLARE_CLASS(o2::IAnimatedValue);
