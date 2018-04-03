#include "stdafx.h"
#include "DefaultActorTransformViewer.h"

#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/EditorApplication.h"
#include "PropertiesWindow/Properties/FloatProperty.h"
#include "PropertiesWindow/Properties/Vector2FloatProperty.h"
#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	DefaultActorTransformViewer::DefaultActorTransformViewer()
	{
		mNameCaption->text = "Transform";
		mPropertiesLayout->layout->minHeight = 130;

		auto positionIcon = o2UI.CreateImage("ui/UI2_position_icon.png");
		*positionIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -5));
		mPropertiesLayout->AddChild(positionIcon);

		mPositionProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		*mPositionProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 5);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mPositionProperty->GetWidget());

		auto pivotIcon = o2UI.CreateImage("ui/UI2_pivot_icon.png");
		*pivotIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -30));
		mPropertiesLayout->AddChild(pivotIcon);

		mPivotProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		*mPivotProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 30);
		mPivotProperty->SetValuePath("transform/pivot");
		mPivotProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mPivotProperty->GetWidget());

		auto sizeIcon = o2UI.CreateImage("ui/UI2_scale_icon.png");
		*sizeIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -55));
		mPropertiesLayout->AddChild(sizeIcon);

		mSizeProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		*mSizeProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 55);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mSizeProperty->GetWidget());

		auto scaleIcon = o2UI.CreateImage("ui/UI2_scale_icon.png");
		*scaleIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -80));
		mPropertiesLayout->AddChild(scaleIcon);

		mScaleProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		*mScaleProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 80);
		mScaleProperty->SetValuePath("transform/scale");
		mScaleProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mScaleProperty->GetWidget());

		auto rotateIcon = o2UI.CreateImage("ui/UI2_rotate_icon.png");
		*rotateIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -105));
		mPropertiesLayout->AddChild(rotateIcon);

		mRotationProperty = mnew FloatProperty();
		*mRotationProperty->GetWidget()->layout = UIWidgetLayout(Vec2F(0, 1), Vec2F(0.5f, 1), Vec2F(40, -125), Vec2F(4, -105));
		mRotationProperty->SetValuePath("transform/angleDegree");
		mRotationProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mRotationProperty->GetWidget());

		auto depthIcon = o2UI.CreateImage("ui/UI2_layer_icon_t.png");
		*depthIcon->layout = UIWidgetLayout::Based(BaseCorner::Top, Vec2F(20, 20), Vec2F(15, -105));
		mPropertiesLayout->AddChild(depthIcon);

		mDepthProperty = mnew FloatProperty();
		*mDepthProperty->GetWidget()->layout = UIWidgetLayout(Vec2F(0.5f, 1), Vec2F(1, 1), Vec2F(23, -125), Vec2F(-5, -105));
		mDepthProperty->SetValuePath("drawDepth");
		mDepthProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mDepthProperty->GetWidget());
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
	}

	void DefaultActorTransformViewer::Refresh()
	{
		mPositionProperty->Refresh();
		mPivotProperty->Refresh();
		mScaleProperty->Refresh();
		mSizeProperty->Refresh();
		mRotationProperty->Refresh();
		mDepthProperty->Refresh();
	}

	void DefaultActorTransformViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue)
	{
		ActorsPropertyChangeAction* action = mnew ActorsPropertyChangeAction(
			o2EditorSceneScreen.GetSelectedActors(), nullptr, path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultActorTransformViewer);
