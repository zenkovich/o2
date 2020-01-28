#pragma once

#include "o2/Animation/Animation.h"
#include "o2/Assets/Asset.h"

namespace o2
{
	// ---------------
	// Animation asset
	// ---------------
	class AnimationAsset: public TAsset<AnimationAsset>
	{
	public:
		class Meta;

	public:
		PROPERTIES(AnimationAsset);
		GETTER(Meta*, meta, GetMeta); // Meta information getter

	public:
		Animation animation; // Asset data @SERIALIZABLE

	public:
		// Check equals operator
		AnimationAsset& operator=(const AnimationAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(AnimationAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public TAssetMeta<AnimationAsset>
		{
		public:
			SERIALIZABLE(Meta);
		};

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
	BASE_CLASS(o2::TAsset<AnimationAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(animation).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
}
END_META;

CLASS_BASES_META(o2::AnimationAsset::Meta)
{
	BASE_CLASS(o2::TAssetMeta<AnimationAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset::Meta)
{
}
END_META;
