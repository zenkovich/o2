#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Text.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Text;
	class Spoiler;
}

namespace Editor
{
	class AssetProperty;
	class FloatProperty;
	class ColorProperty;
	class EnumProperty;
	class WStringProperty;
	class BooleanProperty;

	// --------------------
	// Editor text property
	// --------------------
	class TextViewer : public TObjectPropertiesViewer<Text>
	{
	public:
		IOBJECT(TextViewer);

	protected:
		Ref<ColorProperty>   mColorProperty;
		Ref<FloatProperty>   mAlphaProperty;
		Ref<AssetProperty>   mFontProperty;
		Ref<WStringProperty> mTextProperty;
		Ref<FloatProperty>   mHeightProperty;
		Ref<EnumProperty>    mHorAlignProperty;
		Ref<EnumProperty>    mVerAlignProperty;
		Ref<BooleanProperty> mWordWrapProperty;
		Ref<BooleanProperty> mDotsEndingsProperty;
		Ref<FloatProperty>   mSymbolsDistCoefProperty;
		Ref<FloatProperty>   mLinesDistCoefProperty;

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;
	};
}
// --- META ---

CLASS_BASES_META(Editor::TextViewer)
{
    BASE_CLASS(Editor::TObjectPropertiesViewer<Text>);
}
END_META;
CLASS_FIELDS_META(Editor::TextViewer)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mColorProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAlphaProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mFontProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHeightProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHorAlignProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mVerAlignProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mWordWrapProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDotsEndingsProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSymbolsDistCoefProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLinesDistCoefProperty);
}
END_META;
CLASS_METHODS_META(Editor::TextViewer)
{

    typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
}
END_META;
// --- END META ---
