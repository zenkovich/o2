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
		PushScopeEnterOnStack scope;

		auto layout = mnew UIVerticalLayout();
		layout->borderTop = 5;
		layout->expandWidth = true;
		layout->expandHeight = false;
		layout->fitByChildren = true;

		mViewWidget = layout;

		auto commonFieldsLayout = mnew UIVerticalLayout();
		commonFieldsLayout->spacing = 5;
		commonFieldsLayout->expandWidth = true;
		commonFieldsLayout->expandHeight = false;
		commonFieldsLayout->fitByChildren = true;
		layout->AddChild(commonFieldsLayout);

		const Type& textType = TypeOf(Text);

		mColorProperty = dynamic_cast<ColorProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "color", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mAlphaProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "transparency", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mFontProperty = dynamic_cast<FontAssetProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "fontAsset", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mTextProperty = dynamic_cast<WStringProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "text", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mHeightProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "height", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mVerAlignProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "verAlign", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mHorAlignProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "horAlign", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mWordWrapProperty = dynamic_cast<BooleanProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "wordWrap", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mDotsEndingsProperty = dynamic_cast<BooleanProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "dotsEngings", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mSymbolsDistCoefProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "symbolsDistanceCoef", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mLinesDistCoefProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, textType, "linesDistanceCoef", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));
	}

	void TextViewer::Refresh(const TargetsVec& targetObjets)
	{
		mFieldProperties.Set(targetObjets);
	}

	const Type* TextViewer::GetViewingObjectType() const
	{
		return &TypeOf(Text);
	}

}

DECLARE_CLASS(Editor::TextViewer);
