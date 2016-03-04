#include "ActorPropertiesViewer.h"

#include "PropertiesWindow/ActorsViewer/DefaultEditorActorAnimationViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultEditorActorComponentViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultEditorActorHeaderViewer.h"
#include "PropertiesWindow/ActorsViewer/DefaultEditorActorTransformViewer.h"
#include "PropertiesWindow/ActorsViewer/IEditorActorAnimationViewer.h"
#include "PropertiesWindow/ActorsViewer/IEditorActorComponentViewer.h"
#include "PropertiesWindow/ActorsViewer/IEditorActorHeaderViewer.h"
#include "PropertiesWindow/ActorsViewer/IEditorActorTransformViewer.h"
#include "Scene/Actor.h"
#include "UI/ScrollArea.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"

EditorActorPropertiesViewer::EditorActorPropertiesViewer()
{
	mHeaderViewer = mnew DefaultEditorActorHeaderViewer();
	mTransformViewer = mnew DefaultEditorActorTransformViewer();
	mAnimationViewer = mnew DefaultEditorActorAnimationViewer();
	mDefaultComponentViewer = mnew DefaultEditorActorComponentViewer();

	auto componentsViewersTypes = TypeOf(IEditorActorComponentViewer).DerivedTypes();
	for (auto type : componentsViewersTypes)
		mAvailableComponentsViewers.Add((IEditorActorComponentViewer*)type->CreateSample());

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

EditorActorPropertiesViewer::~EditorActorPropertiesViewer()
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

const Type* EditorActorPropertiesViewer::GetViewingObjectType() const
{
	return &TypeOf(Actor);
}

void EditorActorPropertiesViewer::SetActorHeaderViewer(IEditorActorHeaderViewer* viewer)
{
	delete mHeaderViewer;
	mHeaderViewer = viewer;
}

void EditorActorPropertiesViewer::SetActorTransformViewer(IEditorActorTransformViewer* viewer)
{
	delete mTransformViewer;
	mTransformViewer = viewer;
}

void EditorActorPropertiesViewer::SetActorAnimationViewer(IEditorActorAnimationViewer* viewer)
{
	delete mAnimationViewer;
	mAnimationViewer = viewer;
}

void EditorActorPropertiesViewer::AddComponentViewerType(IEditorActorComponentViewer* viewer)
{
	mAvailableComponentsViewers.Add(viewer);
}

void EditorActorPropertiesViewer::SetTargets(const Vector<IObject*> targets)
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
		auto viewerSample = mAvailableComponentsViewers.FindMatch([&](IEditorActorComponentViewer* x) {
			return x->GetComponentType() == type; });

		if (!viewerSample)
			viewerSample = mDefaultComponentViewer;

		auto availableViewerType = viewerSample->GetComponentType();

		if (!mComponentViewersPool.ContainsKey(availableViewerType) || 
			mComponentViewersPool[availableViewerType].IsEmpty())
		{
			if (!mComponentViewersPool.ContainsKey(availableViewerType))
				mComponentViewersPool.Add(availableViewerType, Vector<IEditorActorComponentViewer*>());
			
			mComponentViewersPool[availableViewerType].Add(
				(IEditorActorComponentViewer*)(viewerSample->GetType().CreateSample()));
		}

		auto componentViewer = mComponentViewersPool[availableViewerType].PopBack();

		viewersWidgets.Add(componentViewer->GetWidget());
		mComponentsViewers.Add(componentViewer);

		componentViewer->SetTargetComponents(mTargetActors.Select<Component*>([&](Actor* x) { 
			return x->GetComponent(type); }));
	}

	mViewersLayout->AddChilds(viewersWidgets);
}

void EditorActorPropertiesViewer::OnEnabled()
{

}

void EditorActorPropertiesViewer::OnDisabled()
{

}

void EditorActorPropertiesViewer::Update(float dt)
{

}

void EditorActorPropertiesViewer::Draw()
{

}
