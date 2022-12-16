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

	// --------------------
	// Editor sprite viewer
	// --------------------
	class SpriteViewer : public TObjectPropertiesViewer<Sprite>
	{
	public:
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
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when type enum selected, shows required property spoiler
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
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHiddenProperties);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mImageProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mColorProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAlphaProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSizePivotProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPivotProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mModeProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mFillPropertiesSpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mFillProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSlicedPropertiesSpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSliceBorderProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTiledPropertiesSpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTileScaleProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSlicesEditor);
}
END_META;
CLASS_METHODS_META(Editor::SpriteViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

	FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, _tmp2);
	FUNCTION().PROTECTED().SIGNATURE(void, OnModeSelected);
}
END_META;
