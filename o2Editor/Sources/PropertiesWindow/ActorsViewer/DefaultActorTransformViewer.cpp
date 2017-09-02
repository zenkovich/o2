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

namespace Editor
{
	DefaultActorTransformViewer::DefaultActorTransformViewer()
	{
		mNameCaption->text = "Transform";
		mPropertiesLayout->layout.minHeight = 130;

		auto positionIcon = o2UI.CreateImage("ui/UI2_position_icon.png");
		positionIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -5));
		mPropertiesLayout->AddChild(positionIcon);

		mPositionProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		mPositionProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 5);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mPositionProperty->GetWidget());

		auto pivotIcon = o2UI.CreateImage("ui/UI2_pivot_icon.png");
		pivotIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -30));
		mPropertiesLayout->AddChild(pivotIcon);

		mPivotProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		mPivotProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 30);
		mPivotProperty->SetValuePath("transform/pivot");
		mPivotProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mPivotProperty->GetWidget());

		auto sizeIcon = o2UI.CreateImage("ui/UI2_scale_icon.png");
		sizeIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -55));
		mPropertiesLayout->AddChild(sizeIcon);

		mSizeProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		mSizeProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 55);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mSizeProperty->GetWidget());

		auto scaleIcon = o2UI.CreateImage("ui/UI2_scale_icon.png");
		scaleIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -80));
		mPropertiesLayout->AddChild(scaleIcon);

		mScaleProperty = mnew Vec2FProperty(o2UI.CreateWidget<UIWidget>("colored vector2 property"));
		mScaleProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 5, 20, 80);
		mScaleProperty->SetValuePath("transform/scale");
		mScaleProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mScaleProperty->GetWidget());

		auto rotateIcon = o2UI.CreateImage("ui/UI2_rotate_icon.png");
		rotateIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(5, -105));
		mPropertiesLayout->AddChild(rotateIcon);

		mRotationProperty = mnew FloatProperty();
		mRotationProperty->GetWidget()->layout = UIWidgetLayout(Vec2F(0, 1), Vec2F(0.5f, 1), Vec2F(40, -125), Vec2F(4, -105));
		mRotationProperty->SetValuePath("transform/angleDegree");
		mRotationProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mRotationProperty->GetWidget());

		auto depthIcon = o2UI.CreateImage("ui/UI2_layer_icon_t.png");
		depthIcon->layout = UIWidgetLayout::Based(BaseCorner::Top, Vec2F(20, 20), Vec2F(15, -105));
		mPropertiesLayout->AddChild(depthIcon);

		mDepthProperty = mnew FloatProperty();
		mDepthProperty->GetWidget()->layout = UIWidgetLayout(Vec2F(0.5f, 1), Vec2F(1, 1), Vec2F(23, -125), Vec2F(-5, -105));
		mDepthProperty->SetValuePath("drawDepth");
		mDepthProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mPropertiesLayout->AddChild(mDepthProperty->GetWidget());
	}

	DefaultActorTransformViewer::~DefaultActorTransformViewer()
	{}

	void DefaultActorTransformViewer::SetTargetActors(const Vector<Actor*>& actors)
	{
		mTargetActors = actors;

		auto getTargetsPair = [&](Actor* x, auto getter) {
			return Pair<void*, void*>(getter(x), x->GetPrototypeLink() ? getter(x->GetPrototypeLink().Get()) : nullptr);
		};

		auto getTargets = [&](auto getter) {
			return mTargetActors.Select<Pair<void*, void*>>([&](Actor* x) { return getTargetsPair(x, getter); });
		};

		mPositionProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->transform->position; }), true);
		mPivotProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->transform->pivot; }), true);
		mScaleProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->transform->scale; }), true);
		mSizeProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->transform->size; }), true);
		mRotationProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->transform->angleDegree; }), true);

		Vector<Pair<void*, void*>> depthTargets;
		for (auto actor : mTargetActors)
		{
			if (auto comp = actor->GetComponent<DrawableComponent>())
			{
				DrawableComponent* proto = dynamic_cast<DrawableComponent*>(comp->GetPrototypeLink());
				depthTargets.Add(Pair<void*, void*>(&comp->drawDepth, proto ? &proto->drawDepth : nullptr));
			}
		}

		mDepthProperty->SetValueAndPrototypePtr(depthTargets, true);
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
		ActorsPropertyChangeAction* action = new ActorsPropertyChangeAction(
			o2EditorSceneScreen.GetSelectedActors(), nullptr, path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

CLASS_META(Editor::DefaultActorTransformViewer)
{
	BASE_CLASS(Editor::IActorTransformViewer);

	PROTECTED_FIELD(mTargetActors);
	PROTECTED_FIELD(mPositionProperty);
	PROTECTED_FIELD(mPivotProperty);
	PROTECTED_FIELD(mScaleProperty);
	PROTECTED_FIELD(mSizeProperty);
	PROTECTED_FIELD(mRotationProperty);
	PROTECTED_FIELD(mDepthProperty);

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
