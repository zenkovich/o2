#pragma once

#include "Assets/ImageAsset.h"
#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class Spoiler;
}

namespace Editor
{
	template<typename _type>
	class AssetProperty;

	class FloatProperty;
	class ColorProperty;
	class EnumProperty;
	class BorderIProperty;

	// ----------------------
	// Editor sprite property
	// ----------------------
	class SpriteViewer : public IObjectPropertiesViewer
	{
	public:
		// Default constructor. Initializes fields controls
		SpriteViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const TargetsVec& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		IOBJECT(SpriteViewer);

	protected:
		typedef AssetProperty<ImageAssetRef> ImageAssetProperty;

		FieldPropertiesInfo mFieldProperties; // Field properties information

		VerticalLayout*   mHiddenProperties = nullptr; // Hidden properties

		ImageAssetProperty* mImageProperty = nullptr;
		ColorProperty*      mColorProperty = nullptr;
		FloatProperty*      mAlphaProperty = nullptr;
		EnumProperty*       mModeProperty = nullptr;

		Spoiler*          mFillPropertiesSpoiler = nullptr;
		FloatProperty*      mFillProperty = nullptr;

		Spoiler*          mSlicedPropertiesSpoiler = nullptr;
		BorderIProperty*    mSliceBorderProperty = nullptr;

		Spoiler*          mTiledPropertiesSpoiler = nullptr;
		FloatProperty*      mTileScaleProperty = nullptr;

	protected:
		// It is called when type enum selected, shows required property spoiler
		void OnModeSelected();
	};
}

CLASS_BASES_META(Editor::SpriteViewer)
{
	BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::SpriteViewer)
{
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mHiddenProperties);
	PROTECTED_FIELD(mImageProperty);
	PROTECTED_FIELD(mColorProperty);
	PROTECTED_FIELD(mAlphaProperty);
	PROTECTED_FIELD(mModeProperty);
	PROTECTED_FIELD(mFillPropertiesSpoiler);
	PROTECTED_FIELD(mFillProperty);
	PROTECTED_FIELD(mSlicedPropertiesSpoiler);
	PROTECTED_FIELD(mSliceBorderProperty);
	PROTECTED_FIELD(mTiledPropertiesSpoiler);
	PROTECTED_FIELD(mTileScaleProperty);
}
END_META;
CLASS_METHODS_META(Editor::SpriteViewer)
{

	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PROTECTED_FUNCTION(void, OnModeSelected);
}
END_META;
