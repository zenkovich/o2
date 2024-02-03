#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Sprite.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include <Ref.h>

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
	class SpriteViewer : public TObjectPropertiesViewer<Ref<Sprite>>
	{
	public:
		IOBJECT(SpriteViewer);

	protected:
		Ref<VerticalLayout> mHiddenProperties = nullptr; // Hidden properties

		Ref<AssetProperty> mImageProperty = nullptr;
		Ref<ColorProperty> mColorProperty = nullptr;
		Ref<FloatProperty> mAlphaProperty = nullptr;
		Ref<Vec2FProperty> mSizePivotProperty = nullptr;
		Ref<Vec2FProperty> mPivotProperty = nullptr;
		Ref<EnumProperty>  mModeProperty = nullptr;

		Ref<Spoiler>       mFillPropertiesSpoiler = nullptr;
		Ref<FloatProperty> mFillProperty = nullptr;

		Ref<Spoiler>         mSlicedPropertiesSpoiler = nullptr;
		Ref<BorderIProperty> mSliceBorderProperty = nullptr;

		Ref<Spoiler>       mTiledPropertiesSpoiler = nullptr;
		Ref<FloatProperty> mTileScaleProperty = nullptr;

		Ref<ImageSlicesEditorWidget> mSlicesEditor = nullptr;

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when type enum selected, shows required property spoiler
		void OnModeSelected();
	};
}
// --- META ---

CLASS_BASES_META(Editor::SpriteViewer)
{
    BASE_CLASS(Editor::TObjectPropertiesViewer<Ref<Sprite>>);
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

    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, const Ref<_tmp1>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, const Ref<_tmp2>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnModeSelected);
}
END_META;
// --- END META ---