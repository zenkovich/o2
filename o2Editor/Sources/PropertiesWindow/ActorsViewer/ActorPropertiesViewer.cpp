#include "stdafx.h"
#include "ActorPropertiesViewer.h"

#include "PropertiesWindow/ActorsViewer/DefaultActorPropertiesViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultActorComponentViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultActorHeaderViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultActorTransformViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorPropertiesViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorTransformViewer.h"
#include "Scene/Actor.h"
#include "Scene/Component.h"
#include "UI/ScrollArea.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	ActorPropertiesViewer::ActorPropertiesViewer()
	{
		mHeaderViewer = mnew DefaultActorHeaderViewer();
		mTransformViewer = mnew DefaultActorTransformViewer();
		mDefaultComponentViewer = mnew DefaultActorComponentViewer();
		mDefaultActorPropertiesViewer=mnew DefaultActorPropertiesViewer();

		auto componentsViewersTypes = TypeOf(IActorComponentViewer).GetDerivedTypes();
		for (auto type : componentsViewersTypes)
			mAvailableComponentsViewers.Add((IActorComponentViewer*)type->CreateSample());

		auto actorPropertiessViewersTypes = TypeOf(IActorPropertiesViewer).GetDerivedTypes();
		for (auto type : actorPropertiessViewersTypes)
			mAvailableActorPropertiesViewers.Add((IActorPropertiesViewer*)type->CreateSample());

		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch());
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->name = "actors scroll area";
		mContentWidget = scrollArea;

		mViewersLayout = o2UI.CreateVerLayout();
		mViewersLayout->name          = "viewers layout";
		mViewersLayout->spacing       = 0.0f;
		mViewersLayout->border        = RectF();
		mViewersLayout->expandHeight  = false;
		mViewersLayout->expandWidth   = true;
		mViewersLayout->fitByChildren = true;
		mViewersLayout->baseCorner    = BaseCorner::Top;
		*mViewersLayout->layout       = UIWidgetLayout::BothStretch();
		mContentWidget->AddChild(mViewersLayout);

		o2Scene.onChanged += [&](Vector<Actor*>) { Refresh(); };
	}

	ActorPropertiesViewer::~ActorPropertiesViewer()
	{
		for (auto kv : mComponentViewersPool)
		{
			for (auto x : kv.Value())
				delete x;
		}

		for (auto x : mAvailableComponentsViewers)
			delete x;

		if (mDefaultComponentViewer)
			delete mDefaultComponentViewer;

		for (auto kv : mActorPropertiesViewersPool)
			delete kv.Value();

		for (auto x : mAvailableActorPropertiesViewers)
			delete x;

		if (mDefaultActorPropertiesViewer)
			delete mDefaultActorPropertiesViewer;

		if (mHeaderViewer)
			delete mHeaderViewer;

		if (mTransformViewer)
			delete mTransformViewer;
	}

	const Type* ActorPropertiesViewer::GetViewingObjectType() const
	{
		return &TypeOf(Actor);
	}

	void ActorPropertiesViewer::SetActorHeaderViewer(IActorHeaderViewer* viewer)
	{
		delete mHeaderViewer;
		mHeaderViewer = viewer;
	}

	void ActorPropertiesViewer::SetActorTransformViewer(IActorTransformViewer* viewer)
	{
		delete mTransformViewer;
		mTransformViewer = viewer;
	}

	void ActorPropertiesViewer::AddComponentViewerType(IActorComponentViewer* viewer)
	{
		mAvailableComponentsViewers.Add(viewer);
	}

	void ActorPropertiesViewer::AddActorPropertiesViewerType(IActorPropertiesViewer* viewer)
	{
		mAvailableActorPropertiesViewers.Add(viewer);
	}

	void ActorPropertiesViewer::Refresh()
	{
		if (mTargetActors.IsEmpty())
			return;

		for (auto viewer : mComponentsViewers)
			viewer->Refresh();

		if (mActorPropertiesViewer)
			mActorPropertiesViewer->Refresh();

		mTransformViewer->Refresh();
		mHeaderViewer->Refresh();
	}

	void ActorPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		mTargetActors = targets.Select<Actor*>([](auto x) { return dynamic_cast<Actor*>(x); });

		// clear 
		mViewersLayout->RemoveAllChildren(false);

		for (auto viewer : mComponentsViewers)
			mComponentViewersPool[viewer->GetComponentType()].Add(viewer);

		mComponentsViewers.Clear();

		// rebuild
		Vector<UIWidget*> viewersWidgets;
		viewersWidgets.Add(mHeaderViewer->GetWidget());
		mHeaderViewer->SetTargetActors(mTargetActors);

		viewersWidgets.Add(mTransformViewer->GetWidget());
		mTransformViewer->SetTargetActors(mTargetActors);

		SetTargetsActorProperties(targets, viewersWidgets);
		SetTargetsComponents(targets, viewersWidgets);		

		mViewersLayout->AddChildren(viewersWidgets.Cast<Actor*>());
	}

	void ActorPropertiesViewer::SetTargetsActorProperties(const Vector<IObject*> targets, Vector<UIWidget*>& viewersWidgets)
	{
		const Type* type = &mTargetActors[0]->GetType();
		bool isAllSameType = mTargetActors.All([&](Actor* x) { return &x->GetType() == type; });

		if (!isAllSameType)
			return;

		if (type == &TypeOf(Actor) || type == &TypeOf(UIWidget))
			return;

		bool usingDefaultViewer = false;

		auto viewerSample = mAvailableActorPropertiesViewers.FindMatch([&](IActorPropertiesViewer* x) {
			return x->GetActorType() == type; });

		if (!viewerSample)
		{
			viewerSample = mDefaultActorPropertiesViewer;
			usingDefaultViewer = true;
		}

		if (!mActorPropertiesViewersPool.ContainsKey(type))
		{
			auto newViewer = (IActorPropertiesViewer*)(viewerSample->GetType().CreateSample());

			if (usingDefaultViewer)
				((DefaultActorPropertiesViewer*)newViewer)->SpecializeActorType(type);

			mActorPropertiesViewersPool.Add(type, newViewer);
		}

		auto propertiesViewer = mActorPropertiesViewersPool[type];

		viewersWidgets.Add(propertiesViewer->GetWidget());
		mActorPropertiesViewer = propertiesViewer;

		propertiesViewer->SetTargetActors(mTargetActors);
	}

	void ActorPropertiesViewer::SetTargetsComponents(const Vector<IObject*> targets, Vector<UIWidget*>& viewersWidgets)
	{
		auto commonComponentsTypes = mTargetActors[0]->GetComponents().Select<const Type*>([](auto x) {
			return &x->GetType(); });

		for (int i = 1; i < mTargetActors.Count(); i++)
		{
			auto actorComponentsTypes = mTargetActors[i]->GetComponents().Select<const Type*>([](auto x) {
				return &x->GetType(); });

			auto commTypesTemp = commonComponentsTypes;
			for (auto type : commTypesTemp)
			{
				if (!actorComponentsTypes.Contains(type))
					commonComponentsTypes.Remove(type);
			}
		}

		for (const Type* type : commonComponentsTypes)
		{
			bool usingDefaultComponentViewer = false;

			auto viewerSample = mAvailableComponentsViewers.FindMatch([&](IActorComponentViewer* x) {
				return x->GetComponentType() == type; });

			if (!viewerSample)
			{
				viewerSample = mDefaultComponentViewer;
				usingDefaultComponentViewer = true;
			}

			if (!mComponentViewersPool.ContainsKey(type) || mComponentViewersPool[type].IsEmpty())
			{
				if (!mComponentViewersPool.ContainsKey(type))
					mComponentViewersPool.Add(type, Vector<IActorComponentViewer*>());

				auto newViewer = (IActorComponentViewer*)(viewerSample->GetType().CreateSample());

				if (usingDefaultComponentViewer)
					((DefaultActorComponentViewer*)newViewer)->SepcializeComponentType(type);

				mComponentViewersPool[type].Add(newViewer);
			}

			auto componentViewer = mComponentViewersPool[type].PopBack();

			viewersWidgets.Add(componentViewer->GetWidget());
			mComponentsViewers.Add(componentViewer);

			componentViewer->SetTargetComponents(
				mTargetActors.Select<Component*>([&](Actor* x) { return x->GetComponent(type); }));
		}
	}

	void ActorPropertiesViewer::OnEnabled()
	{}

	void ActorPropertiesViewer::OnDisabled()
	{
		mTargetActors.Clear();
	}

	void ActorPropertiesViewer::Update(float dt)
	{}

	void ActorPropertiesViewer::Draw()
	{}

}

DECLARE_CLASS(Editor::ActorPropertiesViewer);
