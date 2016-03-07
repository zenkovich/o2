#include "ActorPropertiesViewer.h"

#include "PropertiesWindow/ActorsViewer/DefaultActorAnimationViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultActorComponentViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultActorHeaderViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultActorTransformViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorAnimationViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"
#include "PropertiesWindow/ActorsViewer/IActorTransformViewer.h"
#include "Scene/Actor.h"
#include "UI/ScrollArea.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"

namespace Editor
{
	ActorPropertiesViewer::ActorPropertiesViewer()
	{
		mHeaderViewer = mnew DefaultActorHeaderViewer();
		mTransformViewer = mnew DefaultActorTransformViewer();
		mAnimationViewer = mnew DefaultActorAnimationViewer();
		mDefaultComponentViewer = mnew DefaultActorComponentViewer();

		auto componentsViewersTypes = TypeOf(IActorComponentViewer).DerivedTypes();
		for (auto type : componentsViewersTypes)
			mAvailableComponentsViewers.Add((IActorComponentViewer*)type->CreateSample());

		mContentWidget = o2UI.CreateScrollArea("backless");

		mViewersLayout = o2UI.CreateVerLayout();
		mViewersLayout->spacing = 0.0f;
		mViewersLayout->border = RectF();
		mViewersLayout->expandHeight = false;
		mViewersLayout->expandWidth = true;
		mViewersLayout->fitByChildren = true;
		mViewersLayout->baseCorner = BaseCorner::Top;
		mViewersLayout->layout = UIWidgetLayout::BothStretch();
		mContentWidget->AddChild(mViewersLayout);
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

		delete mDefaultComponentViewer;
		delete mHeaderViewer;
		delete mTransformViewer;
		delete mAnimationViewer;
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

	void ActorPropertiesViewer::SetActorAnimationViewer(IActorAnimationViewer* viewer)
	{
		delete mAnimationViewer;
		mAnimationViewer = viewer;
	}

	void ActorPropertiesViewer::AddComponentViewerType(IActorComponentViewer* viewer)
	{
		mAvailableComponentsViewers.Add(viewer);
	}

	void ActorPropertiesViewer::SetTargets(const Vector<IObject*> targets)
	{
		mTargetActors = targets.Select<Actor*>([](auto x) { return (Actor*)x; });

		// clear 
		mViewersLayout->RemoveAllChilds(false);

		for (auto viewer : mComponentsViewers)
			mComponentViewersPool[viewer->GetComponentType()].Add(viewer);

		mComponentsViewers.Clear();

		// rebuild
		Vector<UIWidget*> viewersWidgets;
		viewersWidgets.Add(mHeaderViewer->GetWidget());
		mHeaderViewer->SetTargetActors(mTargetActors);

		viewersWidgets.Add(mTransformViewer->GetWidget());
		mTransformViewer->SetTargetActors(mTargetActors);

		viewersWidgets.Add(mAnimationViewer->GetWidget());
		mAnimationViewer->SetTargetActors(mTargetActors);

		// components
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
			auto viewerSample = mAvailableComponentsViewers.FindMatch([&](IActorComponentViewer* x) {
				return x->GetComponentType() == type; });

			if (!viewerSample)
				viewerSample = mDefaultComponentViewer;

			auto availableViewerType = viewerSample->GetComponentType();

			if (!mComponentViewersPool.ContainsKey(availableViewerType) ||
				mComponentViewersPool[availableViewerType].IsEmpty())
			{
				if (!mComponentViewersPool.ContainsKey(availableViewerType))
					mComponentViewersPool.Add(availableViewerType, Vector<IActorComponentViewer*>());

				mComponentViewersPool[availableViewerType].Add(
					(IActorComponentViewer*)(viewerSample->GetType().CreateSample()));
			}

			auto componentViewer = mComponentViewersPool[availableViewerType].PopBack();

			viewersWidgets.Add(componentViewer->GetWidget());
			mComponentsViewers.Add(componentViewer);

			componentViewer->SetTargetComponents(mTargetActors.Select<Component*>([&](Actor* x) {
				return x->GetComponent(type); }));
		}

		mViewersLayout->AddChilds(viewersWidgets);
	}

	void ActorPropertiesViewer::OnEnabled()
	{

	}

	void ActorPropertiesViewer::OnDisabled()
	{

	}

	void ActorPropertiesViewer::Update(float dt)
	{

	}

	void ActorPropertiesViewer::Draw()
	{

	}

}