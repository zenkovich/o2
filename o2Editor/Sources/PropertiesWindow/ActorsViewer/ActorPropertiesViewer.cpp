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

	auto componentsViewersTypes = IEditorActorComponentViewer::type.DerivedTypes();
	for (auto type : componentsViewersTypes)
		mAvailableComponentsViewers.Add((IEditorActorComponentViewer*)type->CreateSample());

	mContentWidget = o2UI.CreateScrollArea();

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

Type* EditorActorPropertiesViewer::GetViewingObjectType() const
{
	return &Actor::type;
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
	auto commonComponentsTypes = mTargetActors[0]->GetComponents().Select<Type*>([](auto x) { return &x->GetType(); });
	for (int i = 1; i < mTargetActors.Count() - 1; i++)
	{
		auto actorComponentsTypes = mTargetActors[i]->GetComponents().Select<Type*>([](auto x) { return &x->GetType(); });

		auto commTypesTemp = commonComponentsTypes;
		for (auto type : commTypesTemp)
		{
			if (!actorComponentsTypes.Contains(type))
				commonComponentsTypes.Remove(type);
		}
	}

	for (Type* type : commonComponentsTypes)
	{
		if (!mComponentViewersPool.ContainsKey(type) || mComponentViewersPool[type].IsEmpty())
		{
			auto viewerSample = mAvailableComponentsViewers.FindMatch([&](IEditorActorComponentViewer* x) {
				return x->GetComponentType() == type; });

			if (!viewerSample)
				viewerSample = mDefaultComponentViewer;

			if (!mComponentViewersPool.ContainsKey(type))
				mComponentViewersPool.Add(type, Vector<IEditorActorComponentViewer*>());
			
			mComponentViewersPool[type].Add(viewerSample->Clone());
		}

		auto componentViewer = mComponentViewersPool[type].PopBack();
		viewersWidgets.Add(componentViewer->GetWidget());
		componentViewer->SetTargetActors(mTargetActors);
		mComponentsViewers.Add(componentViewer);
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
