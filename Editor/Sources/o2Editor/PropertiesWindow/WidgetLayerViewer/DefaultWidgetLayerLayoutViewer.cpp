#include "o2Editor/stdafx.h"
#include "DefaultWidgetLayerLayoutViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayerLayout.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Utils/Math/Layout.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/Vector2FloatProperty.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{

	DefaultWidgetLayerLayoutViewer::DefaultWidgetLayerLayoutViewer()
	{
		PushEditorScopeOnStack scope;

		// Position
		Ref<Widget> positionPropertyContainer = mnew Widget();
		positionPropertyContainer->name = "position";
		positionPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(positionPropertyContainer);

		auto positionIcon = o2UI.CreateImage("ui/UI4_position_icon.png");
		*positionIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		positionPropertyContainer->AddChild(positionIcon);

		mPositionProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPositionProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = Ref<DefaultWidgetLayerLayoutViewer>::Bind(&DefaultWidgetLayerLayoutViewer::OnPropertyChangeCompleted, this);
		positionPropertyContainer->AddChild(mPositionProperty);

		// Size
		Ref<Widget> sizePropertyContainer = mnew Widget();
		sizePropertyContainer->name = "size";
		sizePropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(sizePropertyContainer);

		auto sizeIcon = o2UI.CreateImage("ui/UI4_icon_size.png");
		*sizeIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 0));
		sizePropertyContainer->AddChild(sizeIcon);

		mSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mSizeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = Ref<DefaultWidgetLayerLayoutViewer>::Bind(&DefaultWidgetLayerLayoutViewer::OnPropertyChangeCompleted, this);
		sizePropertyContainer->AddChild(mSizeProperty);

		// Anchors
		// Right top
		Ref<Widget> rightTopAnchorPropertyContainer = mnew Widget();
		rightTopAnchorPropertyContainer->name = "right top anchor";
		rightTopAnchorPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rightTopAnchorPropertyContainer);

		auto anchorIcon = o2UI.CreateImage("ui/UI4_icon_anchor.png");
		*anchorIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopAnchorPropertyContainer->AddChild(anchorIcon);

		mAnchorRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorRightTopProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorRightTopProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "R";
		mAnchorRightTopProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "T";
		mAnchorRightTopProperty->SetValuePath("layout/anchorMax");
		mAnchorRightTopProperty->onChanged = Ref<DefaultWidgetLayerLayoutViewer>::Bind(&DefaultWidgetLayerLayoutViewer::OnPropertyChanged, this);
		mAnchorRightTopProperty->onChangeCompleted = Ref<DefaultWidgetLayerLayoutViewer>::Bind(&DefaultWidgetLayerLayoutViewer::OnPropertyChangeCompleted, this);
		rightTopAnchorPropertyContainer->AddChild(mAnchorRightTopProperty);

		// Left bottom
		Ref<Widget> leftBottomAnchorPropertyContainer = mnew Widget();
		leftBottomAnchorPropertyContainer->name = "left bottom anchor";
		leftBottomAnchorPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(leftBottomAnchorPropertyContainer);

		mAnchorLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorLeftBottomProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorLeftBottomProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "L";
		mAnchorLeftBottomProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "B";
		mAnchorLeftBottomProperty->SetValuePath("layout/anchorMin");
		mAnchorLeftBottomProperty->onChanged = Ref<DefaultWidgetLayerLayoutViewer>::Bind(&DefaultWidgetLayerLayoutViewer::OnPropertyChanged, this);
		mAnchorLeftBottomProperty->onChangeCompleted = Ref<DefaultWidgetLayerLayoutViewer>::Bind(&DefaultWidgetLayerLayoutViewer::OnPropertyChangeCompleted, this);
		leftBottomAnchorPropertyContainer->AddChild(mAnchorLeftBottomProperty);

		// Offsets
		// Right top
		Ref<Widget> rightTopOffsetPropertyContainer = mnew Widget();
		rightTopOffsetPropertyContainer->name = "right top offset";
		rightTopOffsetPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rightTopOffsetPropertyContainer);

		auto offsetIcon = o2UI.CreateImage("ui/UI4_icon_offsets.png");
		*offsetIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopOffsetPropertyContainer->AddChild(offsetIcon);

		moffsetRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*moffsetRightTopProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		moffsetRightTopProperty->GetChildByType<Labe#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
inline Ref<T> mmake(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename Derived, typename Base>
inline Ref<Derived> DynamicCast(Ref<Base>& ptr)
{
  return std::dynamic_pointer_cast<Derived>(ptr);
}

class DefaultWidgetLayerLayoutViewer
{
public:
	DefaultWidgetLayerLayoutViewer()
	{
		mSpoiler = mmake<Widget>();
		mSpoiler->name = "Widget Layer Layout Viewer";
		mSpoiler->layout->childLayout = 2;
		mSpoiler->layout->childSpacing = 2;
		mSpoiler->isExpanded = true;
	}

	~DefaultWidgetLayerLayoutViewer()
	{
		mSpoiler = nullptr;
	}

	void SetTargetLayers(const Vector<WidgetLayer*>& layers)
	{
		mLayers = layers;

		auto prototypes = layers.Convert<WidgetLayer*>([](WidgetLayer* x) { return nullptr; });

		mPositionProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::position)>(
			layers, prototypes, [](WidgetLayer* x) -> Ref<decltype(WidgetLayerLayout::position)> { return x->layout.position; });

		mSizeProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::size)>(
			layers, prototypes, [](WidgetLayer* x) -> Ref<decltype(WidgetLayerLayout::size)> { return x->layout.size; });

		mAnchorRightTopProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::anchorMax)>(
			layers, prototypes, [](WidgetLayer* x) -> Ref<decltype(WidgetLayerLayout::anchorMax)> { return x->layout.anchorMax; });

		mAnchorLeftBottomProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::anchorMin)>(
			layers, prototypes, [](WidgetLayer* x) -> Ref<decltype(WidgetLayerLayout::anchorMin)> { return x->layout.anchorMin; });

		moffsetRightTopProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::offsetMax)>(
			layers, prototypes, [](WidgetLayer* x) -> Ref<decltype(WidgetLayerLayout::offsetMax)> { return x->layout.offsetMax; });

		mOffsetLeftBottomProperty->SelectValueAndPrototypeProperties<WidgetLayer, decltype(WidgetLayerLayout::offsetMin)>(
			layers, prototypes, [](WidgetLayer* x) -> Ref<decltype(WidgetLayerLayout::offsetMin)> { return x->layout.offsetMin; });
	}

	void Refresh()
	{
		mAnchorRightTopProperty->Refresh();
		mAnchorLeftBottomProperty->Refresh();
		moffsetRightTopProperty->Refresh();
		mOffsetLeftBottomProperty->Refresh();
	}

	void OnPropertyChangeCompleted(const String& path, const Vector<DataDocument>& prevValue,
	                              const Vector<DataDocument>& newValue)
	{
		PropertyChangeAction* action = mmake<PropertyChangeAction>(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

	void OnPropertyChanged(const Ref<IPropertyField>& field) const
	{
		for (auto layer : mLayers) {
			layer->GetOwnerWidget()->UpdateTransform();
			layer->GetOwnerWidget()->OnChanged();
		}
	}

private:
	Ref<Widget> mSpoiler;
	Vector<WidgetLayer*> mLayers;

	Ref<PositionVector3Property> mPositionProperty;
	Ref<SizeProperty> mSizeProperty;
	Ref<Vec2FProperty> mAnchorRightTopProperty;
	Ref<Vec2FProperty> mAnchorLeftBottomProperty;
	Ref<Vec2FProperty> moffsetRightTopProperty;
	Ref<Vec2FProperty> mOffsetLeftBottomProperty;
};
// --- META ---

DECLARE_CLASS(Editor::DefaultWidgetLayerLayoutViewer, Editor__DefaultWidgetLayerLayoutViewer);
// --- END META ---