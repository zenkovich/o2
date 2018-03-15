#include "stdafx.h"
#include "DefaultActorHeaderViewer.h"

#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/EditorApplication.h"
#include "PropertiesWindow/Properties/AssetProperty.h"
#include "PropertiesWindow/Properties/BooleanProperty.h"
#include "PropertiesWindow/Properties/LayerProperty.h"
#include "PropertiesWindow/Properties/StringProperty.h"
#include "PropertiesWindow/Properties/TagProperty.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Actor.h"
#include "UI/Button.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/Image.h"
#include "UI/Toggle.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	DefaultActorHeaderViewer::DefaultActorHeaderViewer()
	{
		mDataView = mnew UIWidget();
		mDataView->name = "actor head";
		mDataView->layout->minHeight = 42;

		mEnableProperty = mnew BooleanProperty(o2UI.CreateWidget<UIToggle>("actor head enable"));
		*mEnableProperty->GetWidget()->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, 0));
		mEnableProperty->SetValuePath("enabled");
		mEnableProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mEnableProperty->GetWidget());

		mNameProperty = mnew StringProperty(o2UI.CreateWidget<UIEditBox>("actor head name"));
		*mNameProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 21, 15, 17, 2);
		mNameProperty->SetValuePath("name");
		mNameProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mNameProperty->GetWidget());

		mLockProperty = mnew BooleanProperty(o2UI.CreateWidget<UIToggle>("actor head lock"));
		*mLockProperty->GetWidget()->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, -1));
		mLockProperty->SetValuePath("locked");
		mLockProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mLockProperty->GetWidget());

		auto prototypeRoot = mDataView->AddChildWidget(mnew UIWidget());
		prototypeRoot->name = "prototype";
		*prototypeRoot->layout = UIWidgetLayout::BothStretch();
		prototypeRoot->AddState("visible", Animation::EaseInOut(prototypeRoot, &prototypeRoot->transparency, 0.0f, 1.0f, 0.1f));

		auto linkImg = o2UI.CreateImage("ui/UI2_prefab_link_big.png");
		*linkImg->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, -20));
		prototypeRoot->AddChild(linkImg);

		mPrototypeProperty = mnew AssetPROPERTY(ActorAssetRef>(o2UI.CreateWidget<UIWidget>("actor head asset property"));
		*mPrototypeProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 21, 65, 17, 22);
		prototypeRoot->AddChild(mPrototypeProperty->GetWidget());

		mPrototypeApplyBtn = o2UI.CreateWidget<UIButton>("accept prototype");
		*mPrototypeApplyBtn->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(-40, -18));
		mPrototypeApplyBtn->onClick = THIS_FUNC(OnApplyPrototypePressed);
		prototypeRoot->AddChild(mPrototypeApplyBtn);

		mPrototypeRevertBtn = o2UI.CreateWidget<UIButton>("revert prototype");
		*mPrototypeRevertBtn->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(-20, -18));
		mPrototypeRevertBtn->onClick = THIS_FUNC(OnRevertPrototypePressed);
		prototypeRoot->AddChild(mPrototypeRevertBtn);

		mPrototypeBreakBtn = o2UI.CreateWidget<UIButton>("break prototype");
		*mPrototypeBreakBtn->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(25, 25), Vec2F(0, -18));
		mPrototypeBreakBtn->onClick = THIS_FUNC(OnBreakPrototypePressed);
		prototypeRoot->AddChild(mPrototypeBreakBtn);

		auto tagsImg = o2UI.CreateImage("ui/UI2_tag_big.png");
		*tagsImg->layout = UIWidgetLayout::Based(BaseCorner::LeftBottom, Vec2F(20, 20), Vec2F(1, 1));
		mDataView->AddChild(tagsImg);

		mTagsProperty = mnew TagsProperty(o2UI.CreateWidget<UIEditBox>("actor head tags"));
		*mTagsProperty->GetWidget()->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 21, 129, 17, 3);
		mTagsProperty->SetValuePath("tags");
		mTagsProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mTagsProperty->GetWidget());

		auto layerImg = o2UI.CreateImage("ui/UI2_layer_big.png");
		*layerImg->layout = UIWidgetLayout::Based(BaseCorner::RightBottom, Vec2F(20, 20), Vec2F(-109, 1));
		mDataView->AddChild(layerImg);

		mLayerProperty = mnew LayerProperty(o2UI.CreateWidget<UIDropDown>("actor head layer"));
		*mLayerProperty->GetWidget()->layout = UIWidgetLayout::Based(BaseCorner::RightBottom, Vec2F(106, 17), Vec2F(-4, 3));
		mLayerProperty->SetValuePath("layer");
		mLayerProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mLayerProperty->GetWidget());

		Animation protoStateAnim = Animation::EaseInOut(mDataView, &mDataView->layout->minHeight, 42.0f, 62.0f, 0.1f);
		*protoStateAnim.AddAnimationValue(&prototypeRoot->visible) = AnimatedValue<bool>::Linear(false, true, 0.1f);
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

		auto getTargetsPair = [&](Actor* x, auto getter) { 
			return Pair<void*, void*>(getter(x), x->GetPrototypeLink() ? getter(x->GetPrototypeLink().Get()) : nullptr);
		};

		auto getTargets = [&](auto getter) {
			return actors.Select<Pair<void*, void*>>([&](Actor* x) { return getTargetsPair(x, getter); });
		};
		
		mEnableProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->enabled; }), true);
		mNameProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->name; }), true);
		mLockProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->locked; }), true);

 		Vector<void*> prototypeTargets = actors.Select<void*>([](Actor* x) { return &x->prototype; });
 		mPrototypeProperty->SetValuePtr(prototypeTargets, true);

		*mDataView->state["prototype"] = mPrototypeProperty->GetCommonValue().IsValid();

		mTagsProperty->SetValueAndPrototypePtr(getTargets([](Actor* x) { return &x->tags; }), false);

		Vector<void*> layersTargets = actors.Select<void*>([](Actor* x) { return &x->layer; });
		//mLayerProperty->Setup(tagsTargets, true);
	}

	UIWidget* DefaultActorHeaderViewer::GetWidget() const
	{
		return mDataView;
	}

	void DefaultActorHeaderViewer::Refresh()
	{
		mEnableProperty->Refresh();
		mNameProperty->Refresh();
		mLockProperty->Refresh();
		mPrototypeProperty->Refresh();
		mTagsProperty->Refresh();

		*mDataView->state["prototype"] = mPrototypeProperty->GetCommonValue().IsValid();
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
		o2EditorProperties.SetTarget(nullptr);

		Vector<Actor*> revertingActors = GetRootApplyActors();

		for (auto actor : revertingActors)
			actor->RevertToPrototype();

		mActors.Clear();
		if (areViewActorsAssets)
		{
			Vector<UID> viewActors = mActors.Select<UID>([](Actor* x) { return x->GetAssetID(); });
			for (auto id : viewActors)
				mActors.Add(ActorAssetRef(id)->GetActor());
		}
		else
		{
			Vector<UInt64> viewActors = mActors.Select<UInt64>([](Actor* x) { return x->GetID(); });
			for (auto id : viewActors)
				mActors.Add(o2Scene.GetActorByID(id));
		}

		o2EditorProperties.SetTargets(mActors.Cast<IObject*>());
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

	void DefaultActorHeaderViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue)
	{
		ActorsPropertyChangeAction* action = mnew ActorsPropertyChangeAction(
			o2EditorSceneScreen.GetSelectedActors(), nullptr, path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultActorHeaderViewer);
