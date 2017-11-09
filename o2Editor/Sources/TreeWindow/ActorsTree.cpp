#include "ActorsTree.h"

#include "Assets/ActorAsset.h"
#include "Assets/Assets.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "AssetsWindow/UIAssetIcon.h"
#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/Actions/EnableActors.h"
#include "Core/Actions/LockActors.h"
#include "Core/Actions/ReparentActors.h"
#include "Core/EditorApplication.h"
#include "Events/EventSystem.h"
#include "PropertiesWindow/Properties/ActorProperty.h"
#include "PropertiesWindow/Properties/ComponentProperty.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/Toggle.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	UIActorsTree::UIActorsTree():
		UITree(), mAttackedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		delete mNodeWidgetSample;
		mNodeWidgetSample = mnew UIActorsTreeNode();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		Initialize();
	}

	UIActorsTree::UIActorsTree(const UIActorsTree& other):
		UITree(other), mAttackedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	UIActorsTree::~UIActorsTree()
	{
		DeattachFromSceneEvents();
	}

	UIActorsTree& UIActorsTree::operator=(const UIActorsTree& other)
	{
		UITree::operator=(other);
		return *this;
	}

	void UIActorsTree::AttachToSceneEvents()
	{
		o2Scene.onChanged += THIS_FUNC(OnActorsChanged);

		o2Scene.onActorCreated    += THIS_FUNC(OnActorCreated);
		o2Scene.onActorDestroying += THIS_FUNC(OnActorDestroyed);

		auto updateActorTreeNode = THIS_FUNC(OnActorChanged);
		o2Scene.onActorEnableChanged          += updateActorTreeNode;
		o2Scene.onActorLockChanged            += updateActorTreeNode;
		o2Scene.onActorNameChanged            += updateActorTreeNode;
		o2Scene.onActorChildsHierarchyChanged += updateActorTreeNode;

		mAttackedToSceneEvents = true;
	}

	void UIActorsTree::DeattachFromSceneEvents()
	{
		if (Scene::IsSingletonInitialzed() && mAttackedToSceneEvents)
		{
			o2Scene.onChanged -= THIS_FUNC(OnActorsChanged);

			o2Scene.onActorCreated    -= THIS_FUNC(OnActorCreated);
			o2Scene.onActorDestroying -= THIS_FUNC(OnActorDestroyed);

			auto updateActorTreeNode = THIS_FUNC(OnActorChanged);
			o2Scene.onActorEnableChanged          -= updateActorTreeNode;
			o2Scene.onActorLockChanged            -= updateActorTreeNode;
			o2Scene.onActorNameChanged            -= updateActorTreeNode;
			o2Scene.onActorChildsHierarchyChanged -= updateActorTreeNode;
		}
	}

	UITreeNode* UIActorsTree::GetNode(Actor* object)
	{
		return UITree::GetNode((UnknownPtr)(void*)object);
	}

	UIActorsTree::ActorsVec UIActorsTree::GetSelectedActors() const
	{
		return UITree::GetSelectedObjects().Select<Actor*>([](auto x) { return (Actor*)(void*)x; });
	}

	void UIActorsTree::SetSelectedActors(const ActorsVec& actors)
	{
		UITree::SetSelectedObjects(actors.Select<UnknownPtr>([](auto x) { return (UnknownPtr)(void*)x; }));
	}

	void UIActorsTree::SelectActor(Actor* object)
	{
		UITree::SelectObject((UnknownPtr)(void*)object);
	}

	void UIActorsTree::SelectAndHightlightActor(Actor* object)
	{
		UITree::SelectAndHightlightObject((UnknownPtr)(void*)object);
	}

	void UIActorsTree::ScrollToAndHightlight(Actor* object)
	{
		UITree::ScrollToAndHightlight(object);
	}

	void UIActorsTree::DeselectActor(Actor* object)
	{
		UITree::DeselectObject((UnknownPtr)(void*)object);
	}

	void UIActorsTree::DeselectAllActors()
	{
		UITree::DeselectAllObjects();
	}

	void UIActorsTree::ScrollTo(Actor* object)
	{
		UITree::ScrollTo((UnknownPtr)(void*)object);
	}

	void UIActorsTree::Initialize()
	{
		mEnableActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
		mEnableActorsTogglesGroup->onReleased = THIS_FUNC(EnableActorsGroupReleased);

		mLockActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
		mLockActorsTogglesGroup->onReleased = THIS_FUNC(LockActorsGroupReleased);

		UIActorsTreeNode* actorNodeWidgetSample = (UIActorsTreeNode*)mNodeWidgetSample;
		actorNodeWidgetSample->InitializeControls();
	}

	UnknownPtr UIActorsTree::GetObjectParent(UnknownPtr object)
	{
		Actor* actor = object;
		return actor->GetParent();
	}

	Vector<UnknownPtr> UIActorsTree::GetObjectChilds(UnknownPtr object)
	{
		if (object)
		{
			Actor* parent = object;
			return parent->GetChildren().Cast<UnknownPtr>();
		}

		return o2Scene.GetRootActors().Cast<UnknownPtr>();
	}

	String UIActorsTree::GetObjectDebug(UnknownPtr object)
	{
		return object ? ((Actor*)object)->GetName() : "null";
	}

	void UIActorsTree::FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object)
	{
		UIActorsTreeNode* actorNode = (UIActorsTreeNode*)nodeWidget;
		actorNode->SetActor(object);
		actorNode->mLockToggle->SetToggleGroup(mLockActorsTogglesGroup);
		actorNode->mEnableToggle->SetToggleGroup(mEnableActorsTogglesGroup);
	}

	void UIActorsTree::OnNodeDblClick(UITreeNode* nodeWidget)
	{
		((UIActorsTreeNode*)nodeWidget)->EnableEditName();
	}

	void UIActorsTree::OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject)
	{
		Actor* parent = newParent;
		Actor* prevActor = prevObject;
		Vector<Actor*> actors = objects.Cast<Actor*>();

		auto action = mnew ReparentActorsAction(actors);

		o2Scene.ReparentActors(actors, parent, prevActor);

		action->ActorsReparented(parent, prevActor);
		o2EditorApplication.DoneAction(action);

		UITree::OnDraggedObjects(objects, newParent, prevObject);
	}

	void UIActorsTree::EnableActorsGroupPressed(bool value)
	{}

	void UIActorsTree::EnableActorsGroupReleased(bool value)
	{
		ActorsVec actors = mEnableActorsTogglesGroup->GetToggled().Select<Actor*>(
			[](UIToggle* x) { return ((UITreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew EnableActorsAction(actors, value);
		o2EditorApplication.DoneAction(action);
	}

	void UIActorsTree::LockActorsGroupPressed(bool value)
	{}

	void UIActorsTree::LockActorsGroupReleased(bool value)
	{
		ActorsVec actors = mLockActorsTogglesGroup->GetToggled().Select<Actor*>(
			[](UIToggle* x) { return ((UITreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew LockActorsAction(actors, value);
		o2EditorApplication.DoneAction(action);
	}

	void UIActorsTree::OnNodesSelectionChanged(UnknownPtrsVec objects)
	{
		onObjectsSelectionChanged(objects.Cast<Actor*>());
		UITree::OnNodesSelectionChanged(objects);
	}

	void UIActorsTree::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			assetsScroll->InstantiateDraggingAssets();

			if (!assetsScroll->mInstSceneDragActors.IsEmpty())
			{
				UpdateNodesView(true);
				UpdateLayout(true);

				for (auto instActor : assetsScroll->mInstSceneDragActors)
				{
					int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == instActor; });
					CreateVisibleNodeWidget(mAllNodes[idx], idx);
				}

				Focus();
				SetSelectedActors(assetsScroll->mInstSceneDragActors);
				BeginDragging(GetNode(assetsScroll->mInstSceneDragActors.Last()));
				mDragOffset = Vec2F();
			}
		}
		else UITree::OnDragEnter(group);
	}

	void UIActorsTree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			DeselectAllActors();
			EndDragging();
			assetsScroll->ClearInstantiatedDraggingAssets();
			assetsScroll->Focus();
		}
		else UITree::OnDragExit(group);
	}

	void UIActorsTree::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			UpdateDraggingGraphics();
			UITree::OnDraggedAbove(this);
		}
		else UITree::OnDraggedAbove(group);
	}

	void UIActorsTree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			UITree::OnDropped(this);
			
			assetsScroll->RegActorsCreationAction();
			assetsScroll->mInstSceneDragActors.Clear();
		}
		else UITree::OnDropped(group);
	}

	void UIActorsTree::OnActorCreated(Actor* actor)
	{
		UITree::OnObjectCreated(actor, actor->GetParent());
	}

	void UIActorsTree::OnActorDestroyed(Actor* actor)
	{
		UITree::OnObjectRemoved(actor);
	}

	void UIActorsTree::OnActorsChanged(const ActorsVec& actors)
	{
		UITree::OnObjectsChanged(actors.Cast<UnknownPtr>());
	}

	void UIActorsTree::OnActorChanged(Actor* actor)
	{
		UITree::OnObjectsChanged({ actor });
	}

	UIActorsTreeNode::UIActorsTreeNode():
		UITreeNode()
	{}

	UIActorsTreeNode::UIActorsTreeNode(const UIActorsTreeNode& other) :
		UITreeNode(other)
	{
		InitializeControls();
	}

	Editor::UIActorsTreeNode& UIActorsTreeNode::operator=(const UIActorsTreeNode& other)
	{
		CopyData(other);
		return *this;
	}

	void UIActorsTreeNode::CopyData(const Actor& otherActor)
	{
		const UIActorsTreeNode& other = dynamic_cast<const UIActorsTreeNode&>(otherActor);

		UITreeNode::CopyData(other);
		InitializeControls();
	}

	void UIActorsTreeNode::InitializeControls()
	{
		mNameDrawable = GetLayerDrawable<Text>("name");
		mLockToggle   = (UIToggle*)GetChild("lockToggle");
		mEnableToggle = (UIToggle*)GetChild("enableToggle");
		mLinkBtn      = (UIButton*)GetChild("linkBtn");
		mNameEditBox  = (UIEditBox*)GetChild("nameEditBox");
		mEditState    = GetStateObject("edit");

		if (mLockToggle)
			mLockToggle->onClick = THIS_FUNC(OnLockClicked);

		if (mEnableToggle)
			mEnableToggle->onClick = THIS_FUNC(OnEnableCkicked);

		if (mNameEditBox)
			mNameEditBox->onChangeCompleted = THIS_FUNC(OnActorNameChanged);
	}

	void UIActorsTreeNode::SetActor(Actor* actor)
	{
		mTargetActor = actor;
		mName = actor->GetName();

		float alpha = actor->IsEnabledInHierarchy() ? 1.0f : 0.5f;

		mNameDrawable->SetText(mName);
		if (!Math::Equals(alpha, mNameDrawable->GetTransparency()))
		{
			mNameDrawable->SetTransparency(alpha);
			mEnableToggle->SetTransparency(alpha);
			mLinkBtn->SetTransparency(alpha);
		}

		mLinkBtn->SetVisible(actor->GetPrototype());
		mLinkBtn->SetStateForcible("halfHide", !actor->GetPrototypeDirectly().IsValid());

		if (actor->GetPrototype())
		{
			mLinkBtn->onClick = [=]() {
				auto proto = actor->GetPrototype();
				o2EditorAssets.ShowAssetIcon(proto->GetPath());
			};
		}

		mEnableToggle->SetValue(actor->IsEnabled());

		mLockToggle->SetValue(actor->IsLocked());
		mLockToggle->SetStateForcible("locked", actor->IsLockedInHierarchy());
		mLockToggle->SetStateForcible("halfHide", actor->IsLockedInHierarchy() && !actor->IsLocked());
	}

	void UIActorsTreeNode::EnableEditName()
	{
		mEditState->SetState(true);

		mNameEditBox->text = (String)mTargetActor->name;
		mNameEditBox->SelectAll();
		mNameEditBox->UIWidget::Focus();
		mNameEditBox->ResetScroll();
	}

	void UIActorsTreeNode::OnLockClicked()
	{
		mTargetActor->SetLocked(mLockToggle->GetValue());
	}

	void UIActorsTreeNode::OnEnableCkicked()
	{
		mTargetActor->SetEnabled(mEnableToggle->GetValue());
	}

	void UIActorsTreeNode::OnActorNameChanged(const WString& text)
	{
		String prevName = mTargetActor->GetName();

		mTargetActor->SetName(text);
		mEditState->SetState(false);
		((UIActorsTree*)mOwnerTree)->OnActorChanged(mTargetActor);


		DataNode prevData; prevData = prevName;
		DataNode newData; newData = mTargetActor->GetName();

		auto action = mnew ActorsPropertyChangeAction({ mTargetActor }, nullptr, "name", { prevData }, { newData });
		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::UIActorsTree);

DECLARE_CLASS(Editor::UIActorsTreeNode);
