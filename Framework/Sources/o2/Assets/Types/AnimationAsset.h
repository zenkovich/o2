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

	protected:
		// Default constructor
		AnimationAsset();

		// Copy-constructor
		AnimationAsset(const AnimationAsset& asset);

		friend class Assets;
	};

	typedef Ref<AnimationAsset> AnimationAssetRef;
}
