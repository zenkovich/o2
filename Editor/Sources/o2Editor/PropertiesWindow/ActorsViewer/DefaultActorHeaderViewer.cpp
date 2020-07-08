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
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/AssetProperty.h"
#include "o2Editor/Core/Properties/Basic/BooleanProperty.h"
#include "o2Editor/Core/Properties/Basic/LayerProperty.h"
#include "o2Editor/Core/Properties/Basic/StringProperty.h"
#include "o2Editor/Core/Properties/Basic/TagProperty.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	DefaultActorHeaderViewer::DefaultActorHeaderViewer()
	{
		PushEditorScopeOnStack scope;

		mDataView = mnew Widget();
		mDataView->name = "actor head";
		mDataView->layout->minHeight = 42;

		mEnableProperty = o2UI.CreateWidget<BooleanProperty>("actor head enable");
		*mEnableProperty->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, 0));
		mEnableProperty->SetValuePath("enabled");
		mEnableProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mEnableProperty);

		mNameProperty = o2UI.CreateWidget<StringProperty>("actor head name");
		*mNameProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 21, 15, 17, 2);
		mNameProperty->SetValuePath("name");
		mNameProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mNameProperty);

		mLockProperty = o2UI.CreateWidget<BooleanProperty>("actor head lock");
		*mLockProperty->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, -1));
		mLockProperty->SetValuePath("locked");
		mLockProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mLockProperty);


		auto prototypeRoot = mDataView->AddChildWidget(mnew Widget());
		prototypeRoot->name = "prototype";
		*prototypeRoot->layout = WidgetLayout::BothStretch();

		prototypeRoot->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.1f));

		auto linkImg = o2UI.CreateImage("ui/UI4_prefab_link_big.png");
		*linkImg->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, -20));
		prototypeRoot->AddChild(linkImg);

// 		mPrototypeProperty = o2UI.CreateWidget<AssetProperty>("actor head asset property");
// 		*mPrototypeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 21, 65, 17, 22);
// 		prototypeRoot->AddChild(mPrototypeProperty);

		mPrototypeApplyBtn = o2UI.CreateWidget<Button>("accept prototype");
		*mPrototypeApplyBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(-40, -18));
		mPrototypeApplyBtn->onClick = THIS_FUNC(OnApplyPrototypePressed);
		prototypeRoot->AddChild(mPrototypeApplyBtn);

		mPrototypeRevertBtn = o2UI.CreateWidget<Button>("revert prototype");
		*mPrototypeRevertBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(-20, -18));
		mPrototypeRevertBtn->onClick = THIS_FUNC(OnRevertPrototypePressed);
		prototypeRoot->AddChild(mPrototypeRevertBtn);

		mPrototypeBreakBtn = o2UI.CreateWidget<Button>("break prototype");
		*mPrototypeBreakBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(0, -18));
		mPrototypeBreakBtn->onClick = THIS_FUNC(OnBreakPrototypePressed);
		prototypeRoot->AddChild(mPrototypeBreakBtn);

		auto tagsImg = o2UI.CreateImage("ui/UI4_tag_big.png");
		*tagsImg->layout = WidgetLayout::Based(BaseCorner::LeftBottom, Vec2F(20, 20), Vec2F(1, 1));
		mDataView->AddChild(tagsImg);

		mTagsProperty = o2UI.CreateWidget<TagsProperty>("actor head tags");
		*mTagsProperty->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 21, 129, 17, 3);
		mTagsProperty->SetValuePath("tags");
		mTagsProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mTagsProperty);

		auto layerImg = o2UI.CreateImage("ui/UI4_layer_big.png");
		*layerImg->layout = WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(20, 20), Vec2F(-109, 1));
		mDataView->AddChild(layerImg);

		mLayerProperty = o2UI.CreateWidget<LayerProperty>("actor head layer");
		*mLayerProperty->layout = WidgetLayout::Based(BaseCorner::RightBottom, Vec2F(106, 17), Vec2F(-4, 3));
		mLayerProperty->SetValuePath("layer");
		mLayerProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mLayerProperty);

		AnimationClip protoStateAnim = AnimationClip::EaseInOut("layout/minHeight", 42.0f, 62.0f, 0.1f);
		*protoStateAnim.AddTrack<bool>("child/prototype/enabled") = AnimationTrack<bool>::Linear(false, true, 0.1f);
		mDataView->AddState("prototype", protoStateAnim);
	}

	DefaultActorHeaderViewer::~DefaultActorHeaderViewer()
	{
		if (mEnableProperty)
			delete mEnableProperty;

		if (mNameProperty)
			delete mNameProperty;

		if (mLockProperty)
			delete mLockProperty;

		if (mPrototypeProperty)
			delete mPrototypeProperty;

		if (mTagsProperty)
			delete mTagsProperty;

		if (mLayerProperty)
			delete mLayerProperty;

		if (mDataView)
			delete mDataView;
	}

	void DefaultActorHeaderViewer::SetTargetActors(const Vector<Actor*>& actors)
	{
		mActors = actors;

		auto prototypes = actors.Convert<Actor*>([](Actor* x) { return x->GetPrototypeLink().Get(); });
				
		mEnableProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::enabled)>(
			actors, prototypes, [](Actor* x) { return &x->enabled; });

		mNameProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::name)>(
			actors, prototypes, [](Actor* x) { return &x->name; });

		mLockProperty->SelectValueAndPrototypeProperties<Actor, decltype(Actor::locked)>(
			actors, prototypes, [](Actor* x) { return &x->locked; });

		// mPrototypeProperty->SelectValuesProperties<Actor, decltype(Actor::prototype)>(
		// 	actors, [](Actor* x) { return &x->prototype; });

		//mDataView->state["prototype"]->SetState(mPrototypeProperty->GetCommonValue().IsValid());

		mTagsProperty->SelectValueAndPrototypePointers<TagGroup, Actor>(
			actors, prototypes, [](Actor* x) { return &x->tags; });

		Vector<void*> layersTargets = actors.Convert<void*>([](Actor* x) { return &x->layer; });
		//mLayerProperty->Setup(tagsTargets, true);
	}

	Widget* DefaultActorHeaderViewer::GetWidget() const
	{
		return mDataView;
	}

	void DefaultActorHeaderViewer::Refresh()
	{
		mEnableProperty->Refresh();
		mNameProperty->Refresh();
		mLockProperty->Refresh();
		//mPrototypeProperty->Refresh();
		mTagsProperty->Refresh();

		//*mDataView->state["prototype"] = mPrototypeProperty->GetCommonValue().IsValid();
	}

	void DefaultActorHeaderViewer::OnApplyPrototypePressed()
	{
		Vector<Actor*> actors = GetRootApplyActors();

		for (auto actor : actors)
			actor->ApplyChangesToPrototype();
	}

	void DefaultActorHeaderViewer::OnRevertPrototypePressed()
	{
		bool areViewActorsAssets = mActors[0]->IsAsset();
		o2EditorPropertiesWindow.SetTarget(nullptr);

		Vector<Actor*> revertingActors = GetRootApplyActors();

		for (auto actor : revertingActors)
			actor->RevertToPrototype();

		mActors.Clear();
		if (areViewActorsAssets)
		{
			Vector<UID> viewActors = mActors.Convert<UID>([](Actor* x) { return x->GetAssetID(); });
			for (auto id : viewActors)
				mActors.Add(ActorAssetRef(id)->GetActor());
		}
		else
		{
			Vector<UInt64> viewActors = mActors.Convert<UInt64>([](Actor* x) { return x->GetID(); });
			for (auto id : viewActors)
				mActors.Add(o2Scene.GetActorByID(id));
		}

		o2EditorPropertiesWindow.SetTargets(mActors.Cast<IObject*>());
	}

	void DefaultActorHeaderViewer::OnBreakPrototypePressed()
	{
		Vector<Actor*> actors = GetRootApplyActors();

		for (auto actor : actors)
			actor->BreakPrototypeLink();

		*mDataView->state["prototype"] = false;
	}

	Vector<Actor*> DefaultActorHeaderViewer::GetRootApplyActors()
	{
		Vector<Actor*> applyActors;

		for (auto actor : mActors)
		{
			Actor* applyActor = actor;
			if (!applyActor->GetPrototype())
				continue;

			while (applyActor && !applyActor->GetPrototypeDirectly().IsValid())
				applyActor = applyActor->GetParent();

			if (applyActor)
				applyActors.Add(applyActor);
		}

		return applyActors;
	}

	void DefaultActorHeaderViewer::OnPropertyChanged(const String& path, const Vector<DataDocument>& prevValue, 
													 const Vector<DataDocument>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultActorHeaderViewer);
