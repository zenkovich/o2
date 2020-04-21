#include "o2/stdafx.h"
#include "AnimationState.h"

#include "o2/Scene/Components/AnimationComponent.h"

namespace o2
{
	AnimationState::AnimationState(const String& name):
		name(name)
	{}

	void AnimationState::SetWeight(float weight)
	{
		mWeight = weight;
	}

	float AnimationState::GetWeight() const
	{
		return mWeight;
	}

	void AnimationState::SetAnimation(const AnimationAssetRef& animationAsset)
	{
		mAnimation = animationAsset;
		player.SetClip(mAnimation ? &mAnimation->animation : nullptr);
	}

	const AnimationAssetRef& AnimationState::GetAnimation() const
	{
		return mAnimation;
	}

	void AnimationState::OnAnimationChanged()
	{
		player.SetClip(mAnimation ? &mAnimation->animation : nullptr);
	}
}

DECLARE_CLASS(o2::AnimationState);
