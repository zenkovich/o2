#include "stdafx.h"
#include "DefaultActorTransformViewer.h"

#include "Core/Actions/PropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/Properties/Widgets/FloatProperty.h"
#include "Core/Properties/Widgets/Vector2FloatProperty.h"
#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	DefaultActorTransformViewer::DefaultActorTransformViewer()
	{
		mNameCaption->text = "Transform";
		mPropertiesLayout->spacing = 5;
		mPropertiesLayout->border = BorderF(5, 5, 5, 5);
		mPropertiesLayout->fitByChildren = true;
		mPropertiesLayout->expandWidth = true;
		mPropertiesLayout->expandHeight = false;

		// Position
		auto positionPropertyContainer = mnew UIWidget();
		positionPropertyContainer->name = "position";
		positionPropertyContainer->layout->minHeight = 20;
		mPropertiesLayout->AddChild(positionPropertyContainer);

		auto positionIcon = o2UI.CreateImage("ui/UI2_position_icon.png");
		*positionIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		positionPropertyContainer->AddChild(positionIcon);

		mPositionProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPositionProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		positionPropertyContainer->AddChild(mPositionProperty);

		// Pivot
		auto pivotPropertyContainer = mnew UIWidget();
		pivotPropertyContainer->name = "pivot";
		pivotPropertyContainer->layout->minHeight = 20;
		mPropertiesLayout->AddChild(pivotPropertyContainer);

		auto pivotIcon = o2UI.CreateImage("ui/UI2_pivot_icon.png");
		*pivotIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		pivotPropertyContainer->AddChild(pivotIcon);

		mPivotProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPivotProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPivotProperty->SetValuePath("transform/pivot");
		mPivotProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		pivotPropertyContainer->AddChild(mPivotProperty);

		// Size
		auto sizePropertyContainer = mnew UIWidget();
		sizePropertyContainer->name = "size";
		sizePropertyContainer->layout->minHeight = 20;
		mPropertiesLayout->AddChild(sizePropertyContainer);

		auto sizeIcon = o2UI.CreateImage("ui/UI3_icon_size.png");
		*sizeIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 0));
		sizePropertyContainer->AddChild(sizeIcon);

		mSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mSizeProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		sizePropertyContainer->AddChild(mSizeProperty);

		// Scale
		auto scalePropertyContainer = mnew UIWidget();
		scalePropertyContainer->name = "scale";
		scalePropertyContainer->layout->minHeight = 20;
		mPropertiesLayout->AddChild(scalePropertyContainer);

		auto scaleIcon = o2UI.CreateImage("ui/UI2_scale_icon.png");
		*scaleIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		scalePropertyContainer->AddChild(scaleIcon);

		mScaleProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mScaleProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mScaleProperty->SetValuePath("transform/scale");
		mScaleProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		scalePropertyContainer->AddChild(mScaleProperty);

		// Rotation
		auto rotationAndDepthPropertyContainer = mnew UIWidget();
		rotationAndDepthPropertyContainer->name = "rotation and depth";
		rotationAndDepthPropertyContainer->layout->minHeight = 20;
		mPropertiesLayout->AddChild(rotationAndDepthPropertyContainer);

		auto rotateIcon = o2UI.CreateImage("ui/UI2_rotate_icon.png");
		*rotateIcon->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.0f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		rotationAndDepthPropertyContainer->AddChild(rotateIcon);

		mRotationProperty = o2UI.CreateWidget<FloatProperty>();
		*mRotationProperty->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.5f, 1.0f), Vec2F(40, 0), Vec2F(10, 0));
		mRotationProperty->SetValuePath("transform/angleDegree");
		mRotationProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		rotationAndDepthPropertyContainer->AddChild(mRotationProperty);

		// Depth
		auto depthIcon = o2UI.CreateImage("ui/UI2_layer_icon_t.png");
		*depthIcon->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(0.5f, 1.0f), Vec2F(10, 0), Vec2F(30, 0));
		rotationAndDepthPropertyContainer->AddChild(depthIcon);

		mDepthProperty = o2UI.CreateWidget<FloatProperty>();
		*mDepthProperty->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(1, 1.0f), Vec2F(30, 0), Vec2F());
		mDepthProperty->SetValuePath("drawDepth");
		mDepthProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		rotationAndDepthPropertyContainer->AddChild(mDepthProperty);

		// Layout
		mLayoutSpoiler = o2UI.CreateWidget<UISpoiler>("expand with caption");
		mLayoutSpoiler->name = "Layout";
		mLayoutSpoiler->caption = "Layout";
		mLayoutSpoiler->spacing = 5;
		mLayoutSpoiler->fitByChildren = true;
		mLayoutSpoiler->expandWidth = true;
		mLayoutSpoiler->expandHeight = false;
		mPropertiesLayout->AddChild(mLayoutSpoiler);

		// Anchors
		// Right top
		auto rightTopAnchorPropertyContainer = mnew UIWidget();
		rightTopAnchorPropertyContainer->name = "right top anchor";
		rightTopAnchorPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(rightTopAnchorPropertyContainer);

		auto anchorIcon = o2UI.CreateImage("ui/UI3_icon_anchor.png");
		*anchorIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopAnchorPropertyContainer->AddChild(anchorIcon);

		mAnchorRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorRightTopProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorRightTopProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "R";
		mAnchorRightTopProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "T";
		mAnchorRightTopProperty->SetValuePath("layout/anchorMax");
		mAnchorRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		rightTopAnchorPropertyContainer->AddChild(mAnchorRightTopProperty);

		// Left bottom
		auto leftBottomAnchorPropertyContainer = mnew UIWidget();
		leftBottomAnchorPropertyContainer->name = "left bottom anchor";
		leftBottomAnchorPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(leftBottomAnchorPropertyContainer);

		mAnchorLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorLeftBottomProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorLeftBottomProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "L";
		mAnchorLeftBottomProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "B";
		mAnchorLeftBottomProperty->SetValuePath("layout/anchorMin");
		mAnchorLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		leftBottomAnchorPropertyContainer->AddChild(mAnchorLeftBottomProperty);

		// Offsets
		// Right top
		auto rightTopOffsetPropertyContainer = mnew UIWidget();
		rightTopOffsetPropertyContainer->name = "right top offset";
		rightTopOffsetPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(rightTopOffsetPropertyContainer);

		auto offsetIcon = o2UI.CreateImage("ui/UI3_icon_offsets.png");
		*offsetIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopOffsetPropertyContainer->AddChild(offsetIcon);

		mOffsetRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetRightTopProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetRightTopProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "R";
		mOffsetRightTopProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "T";
		mOffsetRightTopProperty->SetValuePath("layout/offsetMax");
		mOffsetRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		rightTopOffsetPropertyContainer->AddChild(mOffsetRightTopProperty);

		// Left bottom
		auto leftBottomOffsetPropertyContainer = mnew UIWidget();
		leftBottomOffsetPropertyContainer->name = "left bottom offset";
		leftBottomOffsetPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(leftBottomOffsetPropertyContainer);

		mOffsetLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetLeftBottomProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetLeftBottomProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "L";
		mOffsetLeftBottomProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "B";
		mOffsetLeftBottomProperty->SetValuePath("layout/offsetMin");
		mOffsetLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		leftBottomOffsetPropertyContainer->AddChild(mOffsetLeftBottomProperty);

		// Min size
		auto minSizePropertyContainer = mnew UIWidget();
		minSizePropertyContainer->name = "right top anchor";
		minSizePropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(minSizePropertyContainer);

		auto minSizeIcon = o2UI.CreateImage("ui/UI3_icon_min_size.png");
		*minSizeIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		minSizePropertyContainer->AddChild(minSizeIcon);

		mMinSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mMinSizeProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mMinSizeProperty->SetValuePath("layout/minSize");
		mMinSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		minSizePropertyContainer->AddChild(mMinSizeProperty);

		// Max size
		auto maxSizePropertyContainer = mnew UIWidget();
		maxSizePropertyContainer->name = "right top anchor";
		maxSizePropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(maxSizePropertyContainer);

		auto maxSizeIcon = o2UI.CreateImage("ui/UI3_icon_max_size.png");
		*maxSizeIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		maxSizePropertyContainer->AddChild(maxSizeIcon);

		mMaxSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mMaxSizeProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mMaxSizeProperty->SetValuePath("layout/maxSize");
		mMaxSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		maxSizePropertyContainer->AddChild(mMaxSizeProperty);

		// Weight
		auto weightPropertyContainer = mnew UIWidget();
		weightPropertyContainer->name = "right top anchor";
		weightPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(weightPropertyContainer);

		auto weightIcon = o2UI.CreateImage("ui/UI3_icon_weight.png");
		*weightIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		weightPropertyContainer->AddChild(weightIcon);

		mWeightProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mWeightProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mWeightProperty->SetValuePath("layout/weight");
		mWeightProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		weightPropertyContainer->AddChild(mWeightProperty);
	}

	DefaultActorTransformViewer::~DefaultActorTransformViewer()
	{
		if (mPositionProperty)
			delete mPositionProperty;

		if (mPivotProperty)
			delete mPivotProperty;

		if (mScaleProperty)
			delete mScaleProperty;

		if (mSizeProperty)
			delete mSizeProperty;

		if (mRotationProperty)
			delete mRotationProperty;

		if (mDepthProperty)
			delete mDepthProperty;

		if (mAnchorRightTopProperty)
			delete mAnchorRightTopProperty;

		if (mAnchorLeftBottomProperty)
			delete mAnchorLeftBottomProperty;

		if (mOffsetRightTopProperty)
			delete mOffsetRightTopProperty;

		if (mOffsetLeftBottomProperty)
			delete mOffsetLeftBottomProperty;

		if (mMinSizeProperty)
			delete mMinSizeProperty;

		if (mMaxSizeProperty)
			delete mMaxSizeProperty;

		if (mWeightProperty)
			delete mWeightProperty;
	}

	void DefaultActorTransformViewer::SetTargetActors(const Vector<Actor*>& actors)
	{
		mTargetActors = actors;

		auto prototypes = actors.Select<Actor*>([](Actor* x) { return x->GetPrototypeLink().Get(); });

		mPositionProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::position)>(
			actors, prototypes, [](Actor* x) { return &x->transform->position; });

		mPivotProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::pivot)>(
			actors, prototypes, [](Actor* x) { return &x->transform->pivot; });

		mScaleProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::scale)>(
			actors, prototypes, [](Actor* x) { return &x->transform->scale; });

		mSizeProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::size)>(
			actors, prototypes, [](Actor* x) { return &x->transform->size; });

		mRotationProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::angleDegree)>(
			actors, prototypes, [](Actor* x) { return &x->transform->angleDegree; });

		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> depthTargets;
		for (auto actor : mTargetActors)
		{
			if (auto comp = actor->GetComponent<DrawableComponent>())
			{
				DrawableComponent* proto = dynamic_cast<DrawableComponent*>(comp->GetPrototypeLink());
				depthTargets.Add(Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
					mnew PropertyValueProxy<float, decltype(comp->drawDepth)>(&comp->drawDepth),
					proto ? mnew PropertyValueProxy<float, decltype(comp->drawDepth)>(&proto->drawDepth) : nullptr));
			}
		}

		mDepthProperty->SetValueAndPrototypeProxy(depthTargets);

		Vector<UIWidget*> targetWidgets = mTargetActors
			.FindAll([](Actor* x) { return dynamic_cast<UIWidget*>(x) != nullptr; })
			.Select<UIWidget*>([](Actor* x) { return dynamic_cast<UIWidget*>(x); });

		mLayoutEnabled = !targetWidgets.IsEmpty();
		mLayoutSpoiler->enabled = mLayoutEnabled;

		if (mLayoutEnabled)
		{
			auto widgetPrototypes = targetWidgets.Select<UIWidget*>(
				[](Actor* x) { return dynamic_cast<UIWidget*>(x->GetPrototypeLink().Get()); });

			mAnchorRightTopProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::anchorMax)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->anchorMax; });

			mAnchorLeftBottomProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::anchorMin)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->anchorMin; });

			mOffsetRightTopProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::offsetMax)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->offsetMax; });

			mOffsetLeftBottomProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::offsetMin)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->offsetMin; });

			mMinSizeProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::minSize)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->minSize; });

			mMaxSizeProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::maxSize)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->maxSize; });

			mWeightProperty->SelectValueAndPrototypeProperties<UIWidget, decltype(UIWidgetLayout::weight)>(
				targetWidgets, widgetPrototypes, [](UIWidget* x) { return &x->layout->weight; });
		}
	}

	void DefaultActorTransformViewer::Refresh()
	{
		mPositionProperty->Refresh();
		mPivotProperty->Refresh();
		mScaleProperty->Refresh();
		mSizeProperty->Refresh();
		mRotationProperty->Refresh();
		mDepthProperty->Refresh();

		if (mLayoutEnabled)
		{
			mAnchorRightTopProperty->Refresh();
			mAnchorLeftBottomProperty->Refresh();
			mOffsetRightTopProperty->Refresh();
			mOffsetLeftBottomProperty->Refresh();
			mMinSizeProperty->Refresh();
			mMaxSizeProperty->Refresh();
			mWeightProperty->Refresh();
		}
	}

	void DefaultActorTransformViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultActorTransformViewer);
