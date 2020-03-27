#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Sprite.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class Spoiler;
}

namespace Editor
{
	class AssetProperty;
	class BorderIProperty;
	class ColorProperty;
	class EnumProperty;
	class FloatProperty;
	class ImageSlicesEditorWidget;
	class Vec2FProperty;

	// ----------------------
	// Editor sprite property
	// ----------------------
	class SpriteViewer : public TObjectPropertiesViewer<Sprite>
	{
	public:
		// Default constructor. Initializes fields controls
		SpriteViewer();

		// Refreshing controls and properties by target objects, initializes slices editor
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		IOBJECT(SpriteViewer);

	protected:
		VerticalLayout* mHiddenProperties = nullptr; // Hidden properties

		AssetProperty* mImageProperty = nullptr;
		ColorProperty* mColorProperty = nullptr;
		FloatProperty* mAlphaProperty = nullptr;
		Vec2FProperty* mSizePivotProperty = nullptr;
		Vec2FProperty* mPivotProperty = nullptr;
		EnumProperty*  mModeProperty = nullptr;

		Spoiler*       mFillPropertiesSpoiler = nullptr;
		FloatProperty* mFillProperty = nullptr;

		Spoiler*         mSlicedPropertiesSpoiler = nullptr;
		BorderIProperty* mSliceBorderProperty = nullptr;

		Spoiler*       mTiledPropertiesSpoiler = nullptr;
		FloatProperty* mTileScaleProperty = nullptr;

		ImageSlicesEditorWidget* mSlicesEditor = nullptr;

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
	PROTECTED_FIELD(mSlicesEditor);
}
END_META;
CLASS_METHODS_META(Editor::SpriteViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PROTECTED_FUNCTION(void, OnModeSelected);
}
END_META;
