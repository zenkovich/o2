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
		ColorProperty*   mColorProperty = nullptr;
		FloatProperty*   mAlphaProperty = nullptr;
		AssetProperty*   mFontProperty = nullptr;
		WStringProperty* mTextProperty = nullptr;
		FloatProperty*   mHeightProperty = nullptr;
		EnumProperty*    mHorAlignProperty = nullptr;
		EnumProperty*    mVerAlignProperty = nullptr;
		BooleanProperty* mWordWrapProperty = nullptr;
		BooleanProperty* mDotsEndingsProperty = nullptr;
		FloatProperty*   mSymbolsDistCoefProperty = nullptr;
		FloatProperty*   mLinesDistCoefProperty = nullptr;

	protected:
		// It is called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;
	};
}

CLASS_BASES_META(Editor::TextViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<Text>);
}
END_META;
CLASS_FIELDS_META(Editor::TextViewer)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mColorProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAlphaProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mFontProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTextProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mHeightProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mHorAlignProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mVerAlignProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mWordWrapProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mDotsEndingsProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSymbolsDistCoefProperty).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLinesDistCoefProperty).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::TextViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PROTECTED_FUNCTION(void, RebuildProperties, _tmp1);
}
END_META;
