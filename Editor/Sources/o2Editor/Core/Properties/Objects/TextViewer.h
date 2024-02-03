#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Text.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2/Core/Utils/Ref.h"
#include "o2/Core/Utils/WeakRef.h"

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
	class TextViewer : public TObjectPropertiesViewer<Ref<Text>>
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
		void RebuildProperties(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjets) override;
	};
}

// --- META ---

CLASS_BASES_META(Editor::TextViewer)
{
    BASE_CLASS(Editor::TObjectPropertiesViewer<Ref<Text>>);
}
END_META;

CLASS_FIELDS_META(Editor::TextViewer)
{
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<ColorProperty>()).NAME(mColorProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<FloatProperty>()).NAME(mAlphaProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<AssetProperty>()).NAME(mFontProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<WStringProperty>()).NAME(mTextProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<FloatProperty>()).NAME(mHeightProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<EnumProperty>()).NAME(mHorAlignProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<EnumProperty>()).NAME(mVerAlignProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<BooleanProperty>()).NAME(mWordWrapProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<BooleanProperty>()).NAME(mDotsEndingsProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<FloatProperty>()).NAME(mSymbolsDistCoefProperty);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<FloatProperty>()).NAME(mLinesDistCoefProperty);
}
END_META;

CLASS_METHODS_META(Editor::TextViewer)
{
    typedef const Vector<Pair<Ref<IObject>, Ref<IObject>>>& _tmp1;

    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, const Ref<List<Pair<Ref<IObject>, Ref<IObject>>>>& targetObjets);
}
END_META;
// --- END META ---