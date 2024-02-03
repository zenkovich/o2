#include "o2Editor/stdafx.h"
#include "DefaultActorHeaderViewer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/DropDown.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/BooleanProperty.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/SceneLayerRefProperty.h"
#include "o2Editor/Core/Properties/Basic/StringProperty.h"
#include "o2Editor/Core/Properties/Basic/TagProperty.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    DefaultActorHeaderViewer::DefaultActorHeaderViewer()
    {
        PushEditorScopeOnStack scope;

        mDataView = mmake<Widget>();
        mDataView->name = "actor head";
        mDataView->layout->minHeight = 62;

        mEnableProperty = o2UI.CreateWidget<BooleanProperty>();
        *mEnableProperty->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, 0));
        mEnableProperty->SetValuePath("enabled");
        mEnableProperty->onChangeCompleted = &DefaultActorHeaderViewer::OnPropertyChanged;
        mDataView->AddChild(mEnableProperty);

        mNameProperty = o2UI.CreateWidget<StringProperty>();
        *mNameProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 21, 15, 17, 2);
        mNameProperty->SetValuePath("name");
        mNameProperty->onChangeCompleted = &DefaultActorHeaderViewer::OnPropertyChanged;
        mDataView->AddChild(mNameProperty);

        mLockProperty = o2UI.CreateWidget<BooleanProperty>();
        *mLockProperty->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, -1));
        mLockProperty->SetValuePath("locked");
        mLockProperty->onChangeCompleted = &DefaultActorHeaderViewer::OnPropertyChanged;
        mDataView->AddChild(mLockProperty);


        auto prototypeRoot = mDataView->AddChildWidget(mmake<Widget>());
        prototypeRoot->name = "prototype";
        *prototypeRoot->layout = WidgetLayout::BothStretch();

        prototypeRoot->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.1f));

        auto linkImg = o2UI.CreateImage("ui/UI4_prefab_link_big.png");
        *linkImg->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, -20));
        prototypeRoot->AddChild(linkImg);

        mPrototypeProperty = o2UI.CreateWidget<AssetProperty>();
        *mPrototypeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 65, 17, 25);
        prototypeRoot->AddChild(mPrototypeProperty);

        mPrototypeApplyBtn = o2UI.CreateWidget<Button>();
        *mPrototypeApplyBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(-40, -18));
        mPrototypeApplyBtn->onClick = &DefaultActorHeaderViewer::OnApplyPrototypePressed;
        prototypeRoot->AddChild(mPrototypeApplyBtn);

        mPrototypeRevertBtn = o2UI.CreateWidget<Button>();
        *mPrototypeRevertBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(-20, -18));
        mPrototypeRevertBtn->onClick = &DefaultActorHeaderViewer::OnRevertPrototypePressed;
        prototypeRoot->AddChild(mPrototypeRevertBtn);

        mPrototypeBreakBtn = o2UI.CreateWidget<Button>();
        *mPrototypeBreakBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(0, -18));
        mPrototypeBreakBtn->onClick = &DefaultActorHeaderViewer::OnBreakPrototypePressed;
        prototypeRoot->AddChild(mPrototypeBreakBtn);

        auto tagsImg = o2UI.CreateImage("ui/UI4_tag_big.png");
        *tagsImg->layout = WidgetLayout::Based(BaseCorner::LeftBottom, Vec2F(20, 20), Vec2F(1, 21));
        mDataView->AddChild(tagsImg);

        mTagsProperty = o2UI.CreateWidget<TagsProperty>();
        *mTagsProperty->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 21, 3, 17, 23);
        mTagsProperty->SetValuePath("tags");
        mTagsProperty->onChangeCompleted = &DefaultActorHeaderViewer::OnPropertyChanged;
        mDataView->AddChild(mTagsProperty);

        auto layerImg = o2UI.CreateImage("ui/UI4_layer_big.png");
        *layerImg->layout = WidgetLayout::Based(BaseCorner::LeftBottom, Vec2F(20, 20), Vec2F(1, 0));
        mDataView->AddChild(layerImg);

        mLayerProperty = o2UI.CreateWidget<SceneLayerRefProperty>();
        *mLayerProperty->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 21, 75, 17, 3);
        mLayerProperty->name = "layer";
        mLayerProperty->SetValuePath("layer");
        mLayerProperty->SetUseInheritedValue(true);
        mLayerProperty->onChangeCompleted = &DefaultActorHeaderViewer::OnPropertyChanged;
        mLayerProperty->onSelectedInheritedValue = &DefaultActorHeaderViewer::OnSelectedInheritedLayer;
        mDataView->AddChild(mLayerProperty);

        mDepthProperty = o2UI.CreateWidget<FloatProperty>();
        *mDepthProperty->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 22, 125, 17, 3);
        mDepthProperty->name = "depth";
        mDepthProperty->SetValuePath("depth");
        mDepthProperty->onChangeCompleted = &DefaultActorHeaderViewer::OnPropertyChanged;
        mDataView->AddChild(mDepthProperty);

        mEditBtn = o2UI.CreateWidget<Button>();
        *mEditBtn->layout = WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(25, 25), Vec2F(0, 3));
        mEditBtn->onClick = &DefaultActorHeaderViewer::OnEditPropertiesPressed;
        mDataView->AddChild(mEditBtn);

        mDeleteBtn = o2UI.CreateWidget<Button>();
        *mDeleteBtn->layout = WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(25, 25), Vec2F(25, 3));
        mDeleteBtn->onClick = &DefaultActorHeaderViewer::OnDeletePressed;
        mDataView->AddChild(mDeleteBtn);

        mDrawBadgesBtn = o2UI.CreateWidget<Button>();
        *mDrawBadgesBtn->layout = WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(25, 25), Vec2F(50, 3));
        mDrawBadgesBtn->onClick = &DefaultActorHeaderViewer::OnDrawBadgesPressed;
        mDataView->AddChild(mDrawBadgesBtn);

        mVisibleBtn = o2UI.CreateWidget<Button>();
        *mVisibleBtn->layout = WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(25, 25), Vec2F(75, 3));
        mVisibleBtn->onClick = &DefaultActorHeaderViewer::OnVisiblePressed;
        mDataView->AddChild(mVisibleBtn);
    }
}#include <Ref.h>

// ...

Ref<ActorProperty> mEnableProperty;
Ref<ActorProperty> mNameProperty;
Ref<ActorProperty> mLockProperty;
Ref<ActorProperty> mPrototypeProperty;
Ref<ActorProperty> mTagsProperty;
Ref<SceneLayerProperty> mLayerProperty;
Ref<ActorProperty> mDepthProperty;
Ref<Widget> mDataView;

// ...

void DefaultActorHeaderViewer::SetTargetActors(const Vector<Ref<Actor>>& actors)
{
    mActors = actors;

    auto prototypes = actors.Convert<Ref<Actor>>([](const Ref<Actor>& x) { return x->GetPrototypeLink().Get(); });

    mEnableProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::enabled)>(
        actors, prototypes, [](const Ref<Actor>& x) { return &x->enabled; });

    mEnableProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::enabled)>(
        actors, prototypes, [](const Ref<Actor>& x) -> const Ref<const decltype(Actor::enabled)>& { return x->enabled; });

    mNameProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::name)>(
        actors, prototypes, [](const Ref<Actor>& x) { return &x->name; });

    mLockProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::locked)>(
        actors, prototypes, [](const Ref<Actor>& x) { return &x->locked; });

    mPrototypeProperty->SelectValuesProperties<Actor, decltype(Actor::prototype)>(
        actors, [](const Ref<Actor>& x) { return &x->prototype; });

    mDataView->state["prototype"]->SetState(mPrototypeProperty->GetCommonValue().IsValid());

    mTagsProperty->SelectValueAndPrototypePointers<TagGroup, Actor>(
        actors, prototypes, [](const Ref<Actor>& x) { return &x->tags; });

    mLayerProperty->SelectValueAndPrototypeFunctional<Ref<SceneLayer>, Actor>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->GetLayer(); },
        [](const Ref<Actor>& x, const Ref<SceneLayer>& l) { x->SetLayer(l->GetName()); });

    mDepthProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::drawDepth)>(
        actors, prototypes, [](const Ref<Actor>& x) { return &x->drawDepth; });

    RefreshLayer();
}

// ...

Ref<ActorProperty> mEnableProperty = mmake<ActorProperty>();
Ref<ActorProperty> mNameProperty = mmake<ActorProperty>();
Ref<ActorProperty> mLockProperty = mmake<ActorProperty>();
Ref<ActorProperty> mPrototypeProperty = mmake<ActorProperty>();
Ref<ActorProperty> mTagsProperty = mmake<ActorProperty>();
Ref<SceneLayerProperty> mLayerProperty = mmake<SceneLayerProperty>();
Ref<ActorProperty> mDepthProperty = mmake<ActorProperty>();
Ref<Widget> mDataView = mmake<Widget>();

// ...

void DefaultActorHeaderViewer::SetTargetActors(const Vector<Ref<Actor>>& actors)
{
    mActors = actors;

    auto prototypes = actors.Convert<Ref<Actor>>([](const Ref<Actor>& x) { return x->GetPrototypeLink().Get(); });

    mEnableProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::enabled)>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->enabled.get(); });

    mNameProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::name)>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->name.get(); });

    mLockProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::locked)>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->locked.get(); });

    mPrototypeProperty->SelectValuesProperties<Actor, decltype(Actor::prototype)>(
        actors, [](const Ref<Actor>& x) { return x->prototype.get(); });

    mDataView->state["prototype"]->SetState(mPrototypeProperty->GetCommonValue().IsValid());

    mTagsProperty->SelectValueAndPrototypePointers<TagGroup, Actor>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->tags.get(); });

    mLayerProperty->SelectValueAndPrototypeFunctional<Ref<SceneLayer>, Actor>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->GetLayer(); },
        [](const Ref<Actor>& x, const Ref<SceneLayer>& l) { x->SetLayer(l->GetName()); });

    mDepthProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::drawDepth)>(
        actors, prototypes, [](const Ref<Actor>& x) { return x->drawDepth.get(); });

    RefreshLayer();
}

// ...

void DefaultActorHeaderViewer::OnApplyPrototypePressed()
{
    Vector<Ref<Actor>> actors = GetRootApplyActors();

    for (const auto& actor : actors)
        actor->ApplyChangesToPrototype();
}

void DefaultActorHeaderViewer::OnRevertPrototypePressed()
{
    bool areViewActorsAssets = mActors[0]->IsAsset();
    o2EditorPropertiesWindow.SetTarget(nullptr);

    Vector<Ref<Actor>> revertingActors = GetRootApplyActors();

    for (const auto& actor : revertingActors)
        actor->RevertToPrototype();

    mActors.Clear();
    if (areViewActorsAssets)
    {
        Vector<UID> viewActors = mActors.Convert<UID>([](const Ref<Actor>& x) { return x->GetAssetID(); });
        for (const auto& id : viewActors)
            mActors.Add(Ref<ActorAsset>(id)->GetActor());
    }
    else
    {
        Vector<UInt64> viewActors = mActors.Convert<UInt64>([](const Ref<Actor>& x) { return x->GetID(); });
        for (const auto& id : viewActors)
            mActors.Add(o2Scene.GetActorByID(id));
    }

    o2EditorPropertiesWindow.SetTargets(mActors.Cast<IObject*>());
}#include <Ref.h>

class DefaultActorHeaderViewer
{
public:
	DefaultActorHeaderViewer() : mActors(nullptr) {}

	void OnSelectedInheritedLayer()
	{
		for (const Ref<Actor>& actor : mActors)
			actor->SetDrawingDepthInheritFromParent(true);
	}

	Vector<Ref<Actor>> GetRootApplyActors()
	{
		Vector<Ref<Actor>> applyActors;

		for (const Ref<Actor>& actor : mActors)
		{
			Ref<Actor> applyActor = actor;
			if (!applyActor->GetPrototype())
				continue;

			while (applyActor && !applyActor->GetPrototypeDirectly().IsValid())
				applyActor = applyActor->GetParent();

			if (applyActor)
				applyActors.Add(applyActor);
		}

		return applyActors;
	}

	void OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue,
													 const Vector<DataDocument>& newValue)
	{
		Ref<PropertyChangeAction> action = mmake<PropertyChangeAction>(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

private:
	WeakRef<Vector<Ref<Actor>>> mActors;
};

// --- META ---

DECLARE_CLASS(Editor::DefaultActorHeaderViewer, Editor__DefaultActorHeaderViewer);
// --- END META ---