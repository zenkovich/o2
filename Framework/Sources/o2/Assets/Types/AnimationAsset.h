#pragma once

#include "o2/Animation/Animation.h"
#include "o2/Assets/Asset.h"

namespace o2
{
	// ---------------
	// Animation asset
	// ---------------
	class AnimationAsset: public AssetWithDefaultMeta<AnimationAsset>
	{
	public:
		Animation animation; // Asset data @SERIALIZABLE

	public:
		// Check equals operator
		AnimationAsset& operator=(const AnimationAsset& asset);

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(AnimationAsset);

	protected:
		// Default constructor
		AnimationAsset();

		// Copy-constructor
		AnimationAsset(const AnimationAsset& asset);

		friend class Assets;
	};

	typedef Ref<AnimationAsset> AnimationAssetRef;
}

CLASS_BASES_META(o2::AnimationAsset)
{
	BASE_CLASS(o2::AssetWithDefaultMeta<AnimationAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAsset)
{
	PUBLIC_FIELD(animation).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset)
{

	PUBLIC_FUNCTION(const char*, GetFileExtensions);
}
END_META;
