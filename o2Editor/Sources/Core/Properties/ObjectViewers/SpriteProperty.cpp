#include "stdafx.h"
#include "SpriteProperty.h"

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

	SpriteProperty::SpriteProperty()
	{
		InitializeControls();
	}

	SpriteProperty::SpriteProperty(const SpriteProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	SpriteProperty& SpriteProperty::operator=(const SpriteProperty& other)
	{
		CopyData(other);
		return *this;
	}

	void SpriteProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void SpriteProperty::InitializeControls()
	{
		mSpoiler = FindChildByType<UISpoiler>(false);
		if (!mSpoiler)
		{
			mSpoiler = o2UI.CreateWidget<UISpoiler>("expand with caption");
			AddChild(mSpoiler);
		}

		if (mSpoiler)
			mSpoiler->onExpand = THIS_FUNC(OnExpand);

		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	void SpriteProperty::OnExpand()
	{
		if (mPropertiesInitialized)
			return;

		auto onChangeCompletedFunc =
			[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		const Type& spriteType = TypeOf(Sprite);

		mImageProperty = dynamic_cast<ImageAssetProperty*>(
			o2EditorProperties.BuildField(mSpoiler, spriteType, "image", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		mColorProperty = dynamic_cast<ColorProperty*>(
			o2EditorProperties.BuildField(mSpoiler, spriteType, "color", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		mAlphaProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mSpoiler, spriteType, "transparency", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		mModeProperty = dynamic_cast<EnumProperty*>(
			o2EditorProperties.BuildField(mSpoiler, spriteType, "mode", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		mModeProperty->onChanged += [&](IPropertyField* x) { OnModeSelected(); };

		mHiddenProperties = o2UI.CreateWidget<UIVerticalLayout>();
		mHiddenProperties->expandWidth = true;
		mHiddenProperties->expandHeight = false;
		mHiddenProperties->fitByChildren = true;
		mSpoiler->AddChild(mHiddenProperties);

		// Fill properties
		mFillPropertiesSpoiler = o2UI.CreateWidget<UISpoiler>();
		mHiddenProperties->AddChild(mFillPropertiesSpoiler);

		mFillProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mFillPropertiesSpoiler, spriteType, "fill", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		auto fillSpace = mnew UIWidget();
		fillSpace->layout->minHeight = 5;
		mFillPropertiesSpoiler->AddChildWidget(fillSpace);

		// Slice properties
		mSlicedPropertiesSpoiler = o2UI.CreateWidget<UISpoiler>();
		mHiddenProperties->AddChild(mSlicedPropertiesSpoiler);

		mSliceBorderProperty = dynamic_cast<BorderIProperty*>(
			o2EditorProperties.BuildField(mSlicedPropertiesSpoiler, spriteType, "sliceBorder", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		// Slice properties
		mTiledPropertiesSpoiler = o2UI.CreateWidget<UISpoiler>();
		mHiddenProperties->AddChild(mTiledPropertiesSpoiler);

		mTileScaleProperty = dynamic_cast<FloatProperty*>(
			o2EditorProperties.BuildField(mTiledPropertiesSpoiler, spriteType, "tileScale", mValuesPath, mFieldProperties, onChangeCompleted, onChanged));

		mPropertiesInitialized = true;
	}

	void SpriteProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mTargetObjects = targets;
		Refresh();
	}

	void SpriteProperty::Refresh()
	{
		mFieldProperties.Set(mTargetObjects.Select<Pair<IObject*, IObject*>>(
			[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
		{
			auto target = GetProxy<Sprite*>(x.first);
			auto prototype = x.second ? GetProxy<Sprite*>(x.second) : nullptr;
			return Pair<IObject*, IObject*>(target, prototype);
		}));
	}

	void SpriteProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);
	}

	WString SpriteProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
	}

	const Type* SpriteProperty::GetFieldType() const
	{
		return &TypeOf(Sprite*);
	}

	void SpriteProperty::OnModeSelected()
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

DECLARE_CLASS(Editor::SpriteProperty);
