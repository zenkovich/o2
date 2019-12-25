#include "stdafx.h"
#include "TextViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Basic/AssetProperty.h"
#include "Core/Properties/Basic/BooleanProperty.h"
#include "Core/Properties/Basic/ColorProperty.h"
#include "Core/Properties/Basic/EnumProperty.h"
#include "Core/Properties/Basic/FloatProperty.h"
#include "Core/Properties/Basic/WStringProperty.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/UIManager.h"

namespace Editor
{
	TextViewer::TextViewer()
	{
		PushEditorScopeOnStack scope;

		auto commonFieldsLayout = mnew VerticalLayout();
		commonFieldsLayout->spacing = 5;
		commonFieldsLayout->expandWidth = true;
		commonFieldsLayout->expandHeight = false;
		commonFieldsLayout->fitByChildren = true;
		mLayout->AddChild(commonFieldsLayout);

		const Type& textType = TypeOf(Text);

		mColorProperty = dynamic_cast<ColorProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "color", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mAlphaProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "transparency", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mFontProperty = dynamic_cast<FontAssetProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "fontAsset", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mTextProperty = dynamic_cast<WStringProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "text", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mHeightProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "height", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mVerAlignProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "verAlign", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mHorAlignProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "horAlign", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mWordWrapProperty = dynamic_cast<BooleanProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "wordWrap", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mDotsEndingsProperty = dynamic_cast<BooleanProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "dotsEngings", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mSymbolsDistCoefProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "symbolsDistanceCoef", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mLinesDistCoefProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "linesDistanceCoef", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));
	}

	void TextViewer::Refresh(const TargetsVec& targetObjets)
	{
		mPropertiesContext.Set(targetObjets);
	}
}

DECLARE_CLASS_MANUAL(Editor::TObjectPropertiesViewer<o2::Text>);

DECLARE_CLASS(Editor::TextViewer);
