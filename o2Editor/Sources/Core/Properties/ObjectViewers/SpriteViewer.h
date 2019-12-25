#pragma once

#include "Assets/ImageAsset.h"
#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Render/Sprite.h"

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
	class Vec2FProperty;

	// ----------------------
	// Editor sprite property
	// ----------------------
	class SpriteViewer : public TObjectPropertiesViewer<Sprite>
	{
	public:
		// Default constructor. Initializes fields controls
		SpriteViewer();

		IOBJECT(SpriteViewer);

	protected:
		typedef AssetProperty<ImageAssetRef> ImageAssetProperty;

		VerticalLayout* mHiddenProperties = nullptr; // Hidden properties

		ImageAssetProperty* mImageProperty = nullptr;
		ColorProperty*      mColorProperty = nullptr;
		FloatProperty*      mAlphaProperty = nullptr;
		Vec2FProperty*      mSizePivotProperty = nullptr;
		Vec2FProperty*      mPivotProperty = nullptr;
		EnumProperty*       mModeProperty = nullptr;

		Spoiler*       mFillPropertiesSpoiler = nullptr;
		FloatProperty* mFillProperty = nullptr;

		Spoiler*         mSlicedPropertiesSpoiler = nullptr;
		BorderIProperty* mSliceBorderProperty = nullptr;

		Spoiler*       mTiledPropertiesSpoiler = nullptr;
		FloatProperty* mTileScaleProperty = nullptr;

	protected:
		// It is called when type enum selected, shows required property spoiler
		void OnModeSelected();
	};
}

CLASS_BASES_META(Editor::SpriteViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<Sprite>);
}
END_META;
CLASS_FIELDS_META(Editor::SpriteViewer)
{
	PROTECTED_FIELD(mHiddenProperties);
	PROTECTED_FIELD(mImageProperty);
	PROTECTED_FIELD(mColorProperty);
	PROTECTED_FIELD(mAlphaProperty);
	PROTECTED_FIELD(mSizePivotProperty);
	PROTECTED_FIELD(mPivotProperty);
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

	PROTECTED_FUNCTION(void, OnModeSelected);
}
END_META;
