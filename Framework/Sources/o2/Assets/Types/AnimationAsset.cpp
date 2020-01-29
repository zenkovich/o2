#include "o2/stdafx.h"
#include "AnimationAsset.h"

#include "o2/Assets/Assets.h"

namespace o2
{
	AnimationAsset::AnimationAsset()
	{}

	AnimationAsset::AnimationAsset(const AnimationAsset& other):
		TAsset(other), animation(other.animation), meta(this)
	{}

	AnimationAsset& AnimationAsset::operator=(const AnimationAsset& other)
	{
		Asset::operator=(other);
		animation = other.animation;

		return *this;
	}

	AnimationAsset::Meta* AnimationAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
	}

	const char* AnimationAsset::GetFileExtensions() const
	{
		return "anim";
	}
}

DECLARE_CLASS(o2::AnimationAsset);

DECLARE_CLASS(o2::AnimationAsset::Meta);
