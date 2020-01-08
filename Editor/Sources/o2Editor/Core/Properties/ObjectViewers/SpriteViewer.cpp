#include "o2Editor/stdafx.h"
#include "SpriteViewer.h"

#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/BorderIntProperty.h"
#include "o2Editor/Core/Properties/Basic/ColorProperty.h"
#include "o2Editor/Core/Properties/Basic/EnumProperty.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/Vector2FloatProperty.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/UIManager.h"

namespace Editor
{
	SpriteViewer::SpriteViewer()
	{
		PushEditorScopeOnStack scope;

		auto commonFieldsLayout = mnew VerticalLayout();
		commonFieldsLayout->spacing = 5;
		commonFieldsLayout->expandWidth = true;
		commonFieldsLayout->expandHeight = false;
		commonFieldsLayout->fitByChildren = true;
		mLayout->AddChild(commonFieldsLayout);

		const Type& spriteType = TypeOf(Sprite);

		mImageProperty = dynamic_cast<ImageAssetProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "image", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mSizePivotProperty = dynamic_cast<Vec2FProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "szPivot", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mPivotProperty = dynamic_cast<Vec2FProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "pivot", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mColorProperty = dynamic_cast<ColorProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "color", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mAlphaProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "transparency", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mModeProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(commonFieldsLayout, spriteType, "mode", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		mModeProperty->onChanged += [&](IPropertyField* x) { OnModeSelected(); };

		mHiddenProperties = o2UI.CreateWidget<VerticalLayout>();
		mHiddenProperties->expandWidth = true;
		mHiddenProperties->expandHeight = false;
		mHiddenProperties->fitByChildren = true;
		mLayout->AddChild(mHiddenProperties);

		// Fill properties
		mFillPropertiesSpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mFillPropertiesSpoiler);

		auto fillSpace = mnew Widget();
		fillSpace->layout->minHeight = 5;
		mFillPropertiesSpoiler->AddChildWidget(fillSpace);

		mFillProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mFillPropertiesSpoiler, spriteType, "fill", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		// Slice properties
		mSlicedPropertiesSpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mSlicedPropertiesSpoiler);

		auto sliceSpace = mnew Widget();
		sliceSpace->layout->minHeight = 5;
		mSlicedPropertiesSpoiler->AddChildWidget(sliceSpace);

		mSliceBorderProperty = dynamic_cast<BorderIProperty*>(
			o2EditorProperties.BuildField(mSlicedPropertiesSpoiler, spriteType, "sliceBorder", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));

		// Slice properties
		mTiledPropertiesSpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mTiledPropertiesSpoiler);

		auto tiledSpace = mnew Widget();
		tiledSpace->layout->minHeight = 5;
		mTiledPropertiesSpoiler->AddChildWidget(tiledSpace);

		mTileScaleProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mTiledPropertiesSpoiler, spriteType, "tileScale", "", mPropertiesContext, mOnChildFieldChangeCompleted, onChanged));
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

DECLARE_CLASS_MANUAL(Editor::TObjectPropertiesViewer<o2::Sprite>);

DECLARE_CLASS(Editor::SpriteViewer);
