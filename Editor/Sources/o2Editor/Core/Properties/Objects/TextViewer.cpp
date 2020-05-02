#include "o2Editor/stdafx.h"
#include "TextViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/BooleanProperty.h"
#include "o2Editor/Core/Properties/Basic/ColorProperty.h"
#include "o2Editor/Core/Properties/Basic/EnumProperty.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/WStringProperty.h"
#include "o2Editor/Core/Properties/Properties.h"

namespace Editor
{
	void TextViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		const Type& textType = TypeOf(Text);
		mColorProperty = dynamic_cast<ColorProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "color", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mAlphaProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "transparency", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mFontProperty = dynamic_cast<AssetProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "fontAsset", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mTextProperty = dynamic_cast<WStringProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "text", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mHeightProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "height", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mVerAlignProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "verAlign", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mHorAlignProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "horAlign", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mWordWrapProperty = dynamic_cast<BooleanProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "wordWrap", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mDotsEndingsProperty = dynamic_cast<BooleanProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "dotsEngings", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mSymbolsDistCoefProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "symbolsDistanceCoef", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mLinesDistCoefProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mSpoiler, textType, "linesDistanceCoef", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));
	}
}

DECLARE_CLASS_MANUAL(Editor::TObjectPropertiesViewer<o2::Text>);

DECLARE_CLASS(Editor::TextViewer);
