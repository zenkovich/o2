#include "stdafx.h"
#include "DefaultWidgetLayerLayoutViewer.h"

#include "Core/Actions/PropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Basic/Vector2FloatProperty.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayerLayout.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/Widgets/Label.h"
#include "SceneWindow/SceneEditScreen.h"
#include "Utils/Math/Layout.h"

namespace Editor
{

	DefaultWidgetLayerLayoutViewer::DefaultWidgetLayerLayoutViewer()
	{
		PushScopeEnterOnStack scope;

		// Position
		auto positionPropertyContainer = mnew Widget();
		positionPropertyContainer->name = "position";
		positionPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(positionPropertyContainer);

		auto positionIcon = o2UI.CreateImage("ui/UI2_position_icon.png");
		*positionIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		positionPropertyContainer->AddChild(positionIcon);

		mPositionProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPositionProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		positionPropertyContainer->AddChild(mPositionProperty);

		// Size
		auto sizePropertyContainer = mnew Widget();
		sizePropertyContainer->name = "size";
		sizePropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(sizePropertyContainer);

		auto sizeIcon = o2UI.CreateImage("ui/UI3_icon_size.png");
		*sizeIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 0));
		sizePropertyContainer->AddChild(sizeIcon);

		mSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mSizeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		sizePropertyContainer->AddChild(mSizeProperty);

		// Anchors
		// Right top
		auto rightTopAnchorPropertyContainer = mnew Widget();
		rightTopAnchorPropertyContainer->name = "right top anchor";
		rightTopAnchorPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rightTopAnchorPropertyContainer);

		auto anchorIcon = o2UI.CreateImage("ui/UI3_icon_anchor.png");
		*anchorIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopAnchorPropertyContainer->AddChild(anchorIcon);

		mAnchorRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorRightTopProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorRightTopProperty->GetChildByType<Label>("layout/properties/x label")->text = "R";
		mAnchorRightTopProperty->GetChildByType<Label>("layout/properties/y label")->text = "T";
		mAnchorRightTopProperty->SetValuePath("layout/anchorMax");
		mAnchorRightTopProperty->onChanged = THIS_FUNC(OnPropertyChanged);
		mAnchorRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		rightTopAnchorPropertyContainer->AddChild(mAnchorRightTopProperty);

		// Left bottom
		auto leftBottomAnchorPropertyContainer = mnew Widget();
		leftBottomAnchorPropertyContainer->name = "left bottom anchor";
		leftBottomAnchorPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(leftBottomAnchorPropertyContainer);

		mAnchorLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorLeftBottomProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorLeftBottomProperty->GetChildByType<Label>("layout/properties/x label")->text = "L";
		mAnchorLeftBottomProperty->GetChildByType<Label>("layout/properties/y label")->text = "B";
		mAnchorLeftBottomProperty->SetValuePath("layout/anchorMin");
		mAnchorLeftBottomProperty->onChanged = THIS_FUNC(OnPropertyChanged);
		mAnchorLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		leftBottomAnchorPropertyContainer->AddChild(mAnchorLeftBottomProperty);

		// Offsets
		// Right top
		auto rightTopOffsetPropertyContainer = mnew Widget();
		rightTopOffsetPropertyContainer->name = "right top offset";
		rightTopOffsetPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rightTopOffsetPropertyContainer);

		auto offsetIcon = o2UI.CreateImage("ui/UI3_icon_offsets.png");
		*offsetIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopOffsetPropertyContainer->AddChild(offsetIcon);

		mOffsetRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetRightTopProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetRightTopProperty->GetChildByType<Label>("layout/properties/x label")->text = "R";
		mOffsetRightTopProperty->GetChildByType<Label>("layout/properties/y label")->text = "T";
		mOffsetRightTopProperty->SetValuePath("layout/offsetMax");
		mOffsetRightTopProperty->onChanged = THIS_FUNC(OnPropertyChanged);
		mOffsetRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		rightTopOffsetPropertyContainer->AddChild(mOffsetRightTopProperty);

		// Left bottom
		auto leftBottomOffsetPropertyContainer = mnew Widget();
		leftBottomOffsetPropertyContainer->name = "left bottom offset";
		leftBottomOffsetPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(leftBottomOffsetPropertyContainer);

		mOffsetLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetLeftBottomProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetLeftBottomProperty->GetChildByType<Label>("layout/properties/x label")->text = "L";
		mOffsetLeftBottomProperty->GetChildByType<Label>("layout/properties/y label")->text = "B";
		mOffsetLeftBottomProperty->SetValuePath("layout/offsetMin");
		mOffsetLeftBottomProperty->onChanged = THIS_FUNC(OnPropertyChanged);
		mOffsetLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		leftBottomOffsetPropertyContainer->AddChild(mOffsetLeftBottomProperty);
	}

	DefaultWidgetLayerLayoutViewer::~DefaultWidgetLayerLayoutViewer()
	{
		delete mSpoiler;
	}

	void DefaultWidgetLayerLayoutViewer::SetTargetLayers(const Vector<WidgetLayer*>& layers)
	{
		mLayers = layers;

		auto prototypes = layers.Select<WidgetLayer*>([](WidgetLayer* x) { return nullptr; });

		mPositionProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::position)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->layout.position; });

		mSizeProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::size)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->layout.size; });

		mAnchorRightTopProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::anchorMax)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->layout.anchorMax; });

		mAnchorLeftBottomProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::anchorMin)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->layout.anchorMin; });

		mOffsetRightTopProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::offsetMax)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->layout.offsetMax; });

		mOffsetLeftBottomProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::offsetMin)>(
			layers, prototypes, [](WidgetLayer* x) { return &x->layout.offsetMin; });
	}

	void DefaultWidgetLayerLayoutViewer::Refresh()
	{
		mAnchorRightTopProperty->Refresh();
		mAnchorLeftBottomProperty->Refresh();
		mOffsetRightTopProperty->Refresh();
		mOffsetLeftBottomProperty->Refresh();
	}

	void DefaultWidgetLayerLayoutViewer::OnPropertyChangeCompleted(const String& path, const Vector<DataNode>& prevValue, 
														   const Vector<DataNode>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

	void DefaultWidgetLayerLayoutViewer::OnPropertyChanged(IPropertyField* field)
	{
		for (auto layer : mLayers) {
			layer->GetOwnerWidget()->UpdateTransform();
			layer->GetOwnerWidget()->OnChanged();
		}
	}

}

DECLARE_CLASS(Editor::DefaultWidgetLayerLayoutViewer);
