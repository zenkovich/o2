#include "o2Editor/stdafx.h"
#include "SpriteViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/BorderIntProperty.h"
#include "o2Editor/Core/Properties/Basic/ColorProperty.h"
#include "o2Editor/Core/Properties/Basic/EnumProperty.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/Vector2FloatProperty.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/ImageSlicesEditorWidget.h"
#include <memory>
namespace Editor
{
	void SpriteViewer::RebuildProperties(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjets)
	{
		const Type& spriteType = TypeOf(Sprite);

		auto commonFieldsLayout = mmake<VerticalLayout>();
		commonFieldsLayout->spacing = 5;
		commonFieldsLayout->expandWidth = true;
		commonFieldsLayout->expandHeight = false;
		commonFieldsLayout->fitByChildren = true;
		mSpoiler->AddChild(commonFieldsLayout);

		mImageProperty = o2EditorProperties.BuildFieldType<AssetProperty>(commonFieldsLayout, spriteType, "image", "",
																		  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mSizePivotProperty = o2EditorProperties.BuildFieldType<Vec2FProperty>(commonFieldsLayout, spriteType, "szPivot", "",
																			  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mPivotProperty = o2EditorProperties.BuildFieldType<Vec2FProperty>(commonFieldsLayout, spriteType, "pivot", "",
																		  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mColorProperty = o2EditorProperties.BuildFieldType<ColorProperty>(commonFieldsLayout, spriteType, "color", "",
																		  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mAlphaProperty = o2EditorProperties.BuildFieldType<FloatProperty>(commonFieldsLayout, spriteType, "transparency", "",
																		  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mModeProperty = o2EditorProperties.BuildFieldType<EnumProperty>(commonFieldsLayout, spriteType, "mode", "",
																		mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mModeProperty->onChanged += [&](IPropertyField* x) { OnModeSelected(); };

		mHiddenProperties = o2UI.CreateWidget<VerticalLayout>();
		mHiddenProperties->expandWidth = true;
		mHiddenProperties->expandHeight = false;
		mHiddenProperties->fitByChildren = true;
		mSpoiler->AddChild(mHiddenProperties);

		// Fill properties
		mFillPropertiesSpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mFillPropertiesSpoiler);

		auto fillSpace = mmake<Widget>();
		fillSpace->layout->minHeight = 5;
		mFillPropertiesSpoiler->AddChildWidget(fillSpace);

		mFillProperty = o2EditorProperties.BuildFieldType<FloatProperty>(mFillPropertiesSpoiler, spriteType, "fill", "",
																		 mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		// Slice properties
		mSlicedPropertiesSpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mSlicedPropertiesSpoiler);

		auto sliceSpace = mmake<Widget>();
		sliceSpace->layout->minHeight = 5;
		mSlicedPropertiesSpoiler->AddChildWidget(sliceSpace);

		auto slicesEditorSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");

		slicesEditorSpoiler->SetCaption("Slices");
		mSliceBorderProperty = o2EditorProperties.BuildFieldType<BorderIProperty>(slicesEditorSpoiler, spriteType, "sliceBorder", "",
																				  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mSlicesEditor = mmake<ImageSlicesEditorWidget>();
		slicesEditorSpoiler->AddChildWidget(mSlicesEditor);
		mSlicedPropertiesSpoiler->AddChild(slicesEditorSpoiler);

		// Slice properties
		mTiledPropertiesSpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mTiledPropertiesSpoiler);

		auto tiledSpace = mmake<Widget>();
		tiledSpace->layout->minHeight = 5;
		mTiledPropertiesSpoiler->AddChildWidget(tiledSpace);

		mTileScaleProperty = o2EditorProperties.BuildFieldType<FloatProperty>(mTiledPropertiesSpoiler, spriteType, "tileScale", "",
																			  mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);
	}

	void SpriteViewer::OnRefreshed(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjets)
	{
		if (mSlicesEditor && !targetObjets.IsEmpty())
			mSlicesEditor->Setup(DynamicCast<Sprite>(targetObjets[0].first.Get())->GetImageAsset(), mSliceBorderProperty);
	}

	void SpriteViewer::OnModeSelected()
	{
		SpriteMode mode = static_cast<SpriteMode>(mModeProperty->GetCommonValue());

		mFillPropertiesSpoiler->SetExpanded(mode == SpriteMode::Fill360CCW ||
											mode == SpriteMode::Fill360CW ||
											mode == SpriteMode::FillDownToUp ||
											mode == SpriteMode::FillLeftToRight ||
											mode == SpriteMode::FillRightToLeft ||
											mode == SpriteMode::FillUpToDown ||
											mode == SpriteMode::Sliced);

		mSlicedPropertiesSpoiler->SetExpanded(mode#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename Base, typename Derived>
Ref<Base> DynamicCast(const Ref<Derived>& ptr)
{
    return std::dynamic_pointer_cast<Base>(ptr);
}

namespace Editor {
    template <typename T>
    class TObjectPropertiesViewer {};

    enum class SpriteMode {
        Simple,
        Sliced,
        Tiled
    };

    namespace o2 {
        class Sprite {};
    }

    class SpriteViewer : public TObjectPropertiesViewer<o2::Sprite>
    {
    public:
        void SetMode(SpriteMode mode)
        {
            mMode = mode;

            mSimplePropertiesSpoiler->SetExpanded(mode == SpriteMode::Simple);

            mSlicedPropertiesSpoiler->SetExpanded(mode == SpriteMode::Sliced);

            mTiledPropertiesSpoiler->SetExpanded(mode == SpriteMode::Tiled);
        }

    private:
        Ref<PropertySpoiler> mSimplePropertiesSpoiler;
        Ref<PropertySpoiler> mSlicedPropertiesSpoiler;
        Ref<PropertySpoiler> mTiledPropertiesSpoiler;

        SpriteMode mMode;
    };
}

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<Editor::o2::Sprite>);
// --- META ---

DECLARE_CLASS(Editor::SpriteViewer, Editor__SpriteViewer);
// --- END META ---