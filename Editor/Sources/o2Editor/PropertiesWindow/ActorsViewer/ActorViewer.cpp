#include "o2Editor/stdafx.h"
#include "ActorViewer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/AddComponentPanel.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorComponentViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorHeaderViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorTransformViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorTransformViewer.h"

namespace Editor
{
	ActorViewer::ActorViewer()
	{
		PushEditorScopeOnStack scope;

		// Create viewers
		mHeaderViewer = mnew DefaultActorHeaderViewer();
		mTransformViewer = mnew DefaultActorTransformViewer();
		mDefaultComponentViewer = mnew DefaultActorComponentViewer();
		mDefaultActorPropertiesViewer = mnew DefaultActorPropertiesViewer();

		// Create available component and actors viewers
		auto componentsViewersTypes = TypeOf(IActorComponentViewer).GetDerivedTypes();
		for (auto type : componentsViewersTypes)
		{
			if (type->GetName().Contains("TActorComponentViewer"))
				continue;

			mAvailableComponentsViewers.Add((IActorComponentViewer*)type->CreateSample());
		}

		auto actorPropertiessViewersTypes = TypeOf(IActorPropertiesViewer).GetDerivedTypes();
		for (auto type : actorPropertiessViewersTypes)
			mAvailableActorPropertiesViewers.Add((IActorPropertiesViewer*)type->CreateSample());

		// Initialize content widget and viewers layout
		mContentWidget = mnew Widget();
		mContentWidget->name = "content";
		*mContentWidget->layout = WidgetLayout::BothStretch();

		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch(0, 0, 15, 0));
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->SetEnableScrollsHiding(false);
		scrollArea->name = "actors scroll area";
		*scrollArea->layout = WidgetLayout::BothStretch(0, 40, 0, 0);
		mContentWidget->AddChild(scrollArea);

		mViewersLayout = o2UI.CreateVerLayout();
		mViewersLayout->name = "viewers layout";
		mViewersLayout->spacing = 0.0f;
		mViewersLayout->border = BorderF();
		mViewersLayout->expandHeight = false;
		mViewersLayout->expandWidth = true;
		mViewersLayout->fitByChildren = true;
		mViewersLayout->baseCorner = BaseCorner::Top;
		*mViewersLayout->layout = WidgetLayout::BothStretch();
		scrollArea->AddChild(mViewersLayout);

		// Initialize add component panel
		mAddComponentPanel = mnew AddComponentPanel(this);
		mAddComponentPanel->name = "add component";
		*mAddComponentPanel->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 40, 0);
		mContentWidget->AddChild(mAddComponentPanel);

		AnimationClip addComponentAnim = AnimationClip::EaseInOut<float>("child/add component/layout/offsetTop",
																		 40, 200, 0.4f);

		*addComponentAnim.AddTrack<float>("child/actors scroll area/layout/offsetBottom") =
			AnimationTrack<float>::EaseInOut(40, 200, 0.4f);

		mContentWidget->AddState("add component", addComponentAnim);

		mAddComponentPanel->onCursorReleased = [&](auto curs) { mContentWidget->SetState("add component", true); };
		mAddComponentPanel->onCursorPressedOutside = [&](auto curs) { mContentWidget->SetState("add component", false); };

		o2Scene.onObjectsChanged += THIS_FUNC(OnSceneObjectsChanged);
	}

	ActorViewer::~ActorViewer()
	{
		o2Scene.onObjectsChanged -= THIS_FUNC(OnSceneObjectsChanged);

		for (auto& kv : mComponentViewersPool)
		{
			for (auto x : kv.second)
				delete x;
		}

		for (auto x : mAvailableComponentsViewers)
			delete x;

		if (mDefaultComponentViewer)
			delete mDefaultComponentViewer;

		for (auto& kv : mActorPropertiesViewersPool)
			delete kv.second;

		for (auto x : mAvailableActorPropertiesViewers)
			delete x;

		if (mDefaultActorPropertiesViewer)
			delete mDefaultActorPropertiesViewer;

		if (mHeaderViewer)
			delete mHeaderViewer;

		if (mTransformViewer)
			delete mTransformViewer;
	}

	const Type* ActorViewer::GetViewingObjectType() const
	{
		return &TypeOf(Actor);
	}

	void ActorViewer::SetActorHeaderViewer(IActorHeaderViewer* viewer)
	{
		delete mHeaderViewer;
		mHeaderViewer = viewer;
	}

	void ActorViewer::SetActorTransformViewer(IActorTransformViewer* viewer)
	{
		delete mTransformViewer;
		mTransformViewer = viewer;
	}

	void ActorViewer::AddComponentViewerType(IActorComponentViewer* viewer)
	{
		mAvailableComponentsViewers.Add(viewer);
	}

	void ActorViewer::AddActorPropertiesViewerType(IActorPropertiesViewer* viewer)
	{
		mAvailableActorPropertiesViewers.Add(viewer);
	}

	void ActorViewer::Refresh()
	{
		PushEditorScopeOnStack scope;

		if (mTargetActors.IsEmpty())
			return;

		auto currentComponentGroups = GetGroupedComponents();
		if (mComponentGroupsTypes != currentComponentGroups)
		{
			SetTargets(mTargetActors.DynamicCast<IObject*>());
			return;
		}

		for (auto viewer : mComponentsViewers)
			viewer->Refresh();

		if (mActorPropertiesViewer)
			mActorPropertiesViewer->Refresh();

		mTransformViewer->Refresh();
		mHeaderViewer->Refresh();
	}

	void ActorViewer::OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects)
	{
		Refresh();
	}

	void ActorViewer::SetTargets(const Vector<IObject*> targets)
	{
		PushEditorScopeOnStack scope;

		mTargetActors = targets.Convert<Actor*>([](auto x) { return dynamic_cast<Actor*>(x); });

		// clear 
		mViewersLayout->RemoveAllChildren(false);

		// rebuild
		Vector<Widget*> viewersWidgets;
		viewersWidgets.Add(mHeaderViewer->GetWidget());
		mHeaderViewer->SetTargetActors(mTargetActors);

		viewersWidgets.Add(mTransformViewer->GetWidget());
		mTransformViewer->SetTargetActors(mTargetActors);

		SetTargetsActorProperties(targets, viewersWidgets);
		SetTargetsComponents(targets, viewersWidgets);

		mViewersLayout->AddChildren(viewersWidgets.Cast<Actor*>());
	}

	void ActorViewer::SetTargetsActorProperties(const Vector<IObject*> targets, Vector<Widget*>& viewersWidgets)
	{
		PushEditorScopeOnStack scope;

		const Type* type = &mTargetActors[0]->GetType();
		bool isAllSameType = mTargetActors.All([&](Actor* x) { return &x->GetType() == type; });

		if (!isAllSameType)
			return;

		bool usingDefaultViewer = false;

		auto viewerSample = mAvailableActorPropertiesViewers.FindOrDefault([&](IActorPropertiesViewer* x) {
			return x->GetActorType() == type; });

		if (!viewerSample)
		{
			viewerSample = mDefaultActorPropertiesViewer;
			usingDefaultViewer = true;
		}

		if (!mActorPropertiesViewersPool.ContainsKey(type))
		{
			PushEditorScopeOnStack scope;

			auto newViewer = (IActorPropertiesViewer*)(viewerSample->GetType().CreateSample());

			if (usingDefaultViewer)
				((DefaultActorPropertiesViewer*)newViewer)->SpecializeActorType(type);

			mActorPropertiesViewersPool.Add(type, newViewer);
		}

		auto propertiesViewer = mActorPropertiesViewersPool[type];

		propertiesViewer->SetTargetActors(mTargetActors);

		if (!propertiesViewer->IsEmpty())
			viewersWidgets.Add(propertiesViewer->GetWidget());

		mActorPropertiesViewer = propertiesViewer;
	}

	Vector<Pair<const Type*, Vector<Component*>>> ActorViewer::GetGroupedComponents() const
	{
		Vector<Pair<const Type*, Vector<Component*>>> res;

		for (auto actor : mTargetActors)
		{
			Map<const Type*, int> offsets;
			for (auto component : actor->GetComponents())
			{
				auto type = &component->GetType();
				auto offset = offsets[type];
				offsets[type]++;

				Vector<Component*>* list = nullptr;
				for (int i = 0; i < res.Count(); i++)
				{
					if (res[i].first == type)
					{
						if (offset == 0)
						{
							list = &res[i].second;
							break;
						}
						else
							offset--;
					}
				}

				if (!list)
				{
					res.Add({ type, {} });
					list = &res.Last().second;
				}

				list->Add(component);
			}
		}

		return res;
	}

	void ActorViewer::SetTargetsComponents(const Vector<IObject*> targets, Vector<Widget*>& viewersWidgets)
	{
		PushEditorScopeOnStack scope;

		auto lastComponentViewers = mComponentsViewers;

		for (auto viewer : mComponentsViewers)
			mComponentViewersPool[viewer->GetComponentType()].Add(viewer);

		mComponentsViewers.Clear();

		mComponentGroupsTypes = GetGroupedComponents();
		for (auto pair : mComponentGroupsTypes)
		{
			auto viewerSample = mAvailableComponentsViewers.FindOrDefault([&](IActorComponentViewer* x) {
				return x->GetComponentType() == pair.first; });

			if (!viewerSample)
				viewerSample = mDefaultComponentViewer;

			if (!mComponentViewersPool.ContainsKey(type) || mComponentViewersPool[type].IsEmpty())
			{
				if (!mComponentViewersPool.ContainsKey(type))
					mComponentViewersPool.Add(type, {});

				auto newViewer = (IActorComponentViewer*)(viewerSample->GetType().CreateSample());

				mComponentViewersPool[type].Add(newViewer);
			}

			auto componentViewer = mComponentViewersPool[type].PopBack();

			viewersWidgets.Add(componentViewer->GetWidget());
			mComponentsViewers.Add(componentViewer);

			componentViewer->SetTargetComponents(pair.second);

			if (lastComponentViewers.Contains(componentViewer))
				lastComponentViewers.Remove(componentViewer);
			else
				componentViewer->SetEnabled(true);
		}

		for (auto viewer : lastComponentViewers)
			viewer->SetEnabled(false);
	}

	void ActorViewer::OnEnabled()
	{
		mHeaderViewer->SetEnabled(true);
		mTransformViewer->SetEnabled(true);

		if (mActorPropertiesViewer)
			mActorPropertiesViewer->SetEnabled(true);

		mComponentsViewers.ForEach([](auto x) { x->SetEnabled(true); });
	}

	void ActorViewer::OnDisabled()
	{
		mHeaderViewer->SetEnabled(false);
		mTransformViewer->SetEnabled(false);

		if (mActorPropertiesViewer)
			mActorPropertiesViewer->SetEnabled(false);

		mComponentsViewers.ForEach([](auto x) { x->SetEnabled(false); });

		mTargetActors.Clear();
	}

}

DECLARE_CLASS(Editor::ActorViewer);
