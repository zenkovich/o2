#include "stdafx.h"
#include "SpriteViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Basic/AssetProperty.h"
#include "Core/Properties/Basic/BorderIntProperty.h"
#include "Core/Properties/Basic/ColorProperty.h"
#include "Core/Properties/Basic/EnumProperty.h"
#include "Core/Properties/Basic/FloatProperty.h"
#include "Core/Properties/Properties.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"

namespace Editor
{
	SpriteViewer::SpriteViewer()
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

		const Type& spriteType = TypeOf(Sprite);

		mImageProperty = dynamic_cast<ImageAssetProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "image", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mColorProperty = dynamic_cast<ColorProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "color", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mAlphaProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "transparency", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mModeProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "mode", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		mModeProperty->onChanged += [&](IPropertyField* x) { OnModeSelected(); };

		mHiddenProperties = o2UI.CreateWidget<UIVerticalLayout>();
		mHiddenProperties->expandWidth = true;
		mHiddenProperties->expandHeight = false;
		mHiddenProperties->fitByChildren = true;
		layout->AddChild(mHiddenProperties);

		// Fill properties
		mFillPropertiesSpoiler = o2UI.CreateWidget<UISpoiler>();
		mHiddenProperties->AddChild(mFillPropertiesSpoiler);

		auto fillSpace = mnew UIWidget();
		fillSpace->layout->minHeight = 5;
		mFillPropertiesSpoiler->AddChildWidget(fillSpace);

		mFillProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mFillPropertiesSpoiler, spriteType, "fill", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		// Slice properties
		mSlicedPropertiesSpoiler = o2UI.CreateWidget<UISpoiler>();
		mHiddenProperties->AddChild(mSlicedPropertiesSpoiler);

		auto sliceSpace = mnew UIWidget();
		sliceSpace->layout->minHeight = 5;
		mSlicedPropertiesSpoiler->AddChildWidget(sliceSpace);

		mSliceBorderProperty = dynamic_cast<BorderIProperty*>(
			o2EditorProperties.BuildField(mSlicedPropertiesSpoiler, spriteType, "sliceBorder", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));

		// Slice properties
		mTiledPropertiesSpoiler = o2UI.CreateWidget<UISpoiler>();
		mHiddenProperties->AddChild(mTiledPropertiesSpoiler);

		auto tiledSpace = mnew UIWidget();
		tiledSpace->layout->minHeight = 5;
		mTiledPropertiesSpoiler->AddChildWidget(tiledSpace);

		mTileScaleProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mTiledPropertiesSpoiler, spriteType, "tileScale", "", mFieldProperties, mOnChildFieldChangeCompleted, onChanged));
	}

	void SpriteViewer::Refresh(const TargetsVec& targetObjets)
	{
		mFieldProperties.Set(targetObjets);
	}

	const Type* SpriteViewer::GetViewingObjectType() const
	{
		return &TypeOf(Sprite);
	}

	void SpriteViewer::OnModeSelected()
	{
		SpriteMode mode = (SpriteMode)(mModeProperty->GetCommonValue());

		mFillPropertiesSpoiler->SetExpanded(mode == SpriteMode::Fill360CCW ||
											mode == SpriteMode::Fill360CW ||
											mode == SpriteMode::FillDownToUp ||
											mode == SpriteMode::FillLeftToRight ||
											mode == SpriteMode::FillRightToLeft ||
											mode == SpriteMode::FillUpToDown ||
											mode == SpriteMode::Sliced);

		mSlicedPropertiesSpoiler->SetExpanded(mode == SpriteMode::Sliced);

		mTiledPropertiesSpoiler->SetExpanded(mode == SpriteMode::Tiled);
	}

}

DECLARE_CLASS(Editor::SpriteViewer);
