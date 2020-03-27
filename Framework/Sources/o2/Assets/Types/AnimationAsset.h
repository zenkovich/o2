#pragma once

#include "o2/Animation/Animation.h"
#include "o2/Assets/Asset.h"
#include "o2/Utils/Editor/Attributes/ExpandedByDefaultAttribute.h"

namespace o2
{
	// ---------------
	// Animation asset
	// ---------------
	class AnimationAsset: public AssetWithDefaultMeta<AnimationAsset>
	{
	public:
		Animation animation; // Asset data @SERIALIZABLE @EXPANDED_BY_DEFAULT

	public:
		// Check equals operator
		AnimationAsset& operator=(const AnimationAsset& asset);

		// Returns extensions string
		const char* GetFileExtensions() const override;

		// Returns editor icon
		static String GetEditorIcon() { return "ui/UI4_anim_file_icon.png"; }

		// Returns editor sorting weight
		static int GetEditorSorting() { return 98; }

		// Is this asset type is available to create from editor's assets window
		static bool IsAvailableToCreateFromEditor() { return true; }

		// Is asset reference available to contain instance inside
		static bool IsReferenceCanOwnInstance() { return true; }

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
	PUBLIC_FIELD(animation).EXPANDED_BY_DEFAULT_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset)
{

	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetEditorIcon);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableToCreateFromEditor);
	PUBLIC_STATIC_FUNCTION(bool, IsReferenceCanOwnInstance);
}
END_META;
