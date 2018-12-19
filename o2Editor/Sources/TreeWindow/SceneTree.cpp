#include "stdafx.h"
#include "SceneTree.h"

#include "Assets/ActorAsset.h"
#include "Assets/Assets.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "AssetsWindow/UIAssetIcon.h"
#include "Core/Actions/Enable.h"
#include "Core/Actions/Lock.h"
#include "Core/Actions/PropertyChange.h"
#include "Core/Actions/Reparent.h"
#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Basic/ActorProperty.h"
#include "Core/Properties/Basic/ComponentProperty.h"
#include "Core/UIRoot.h"
#include "Events/EventSystem.h"
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
	UISceneTree::UISceneTree() :
		UITree(), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		delete mNodeWidgetSample;
		mNodeWidgetSample = mnew UISceneTreeNode();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		Initialize();
	}

	UISceneTree::UISceneTree(const UISceneTree& other) :
		UITree(other), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	UISceneTree::~UISceneTree()
	{
		DeattachFromSceneEvents();
	}

	UISceneTree& UISceneTree::operator=(const UISceneTree& other)
	{
		UITree::operator=(other);
		return *this;
	}

	void UISceneTree::AttachToSceneEvents()
	{
		o2Scene.onObjectsChanged += THIS_FUNC(OnObjectsChanged);

		o2Scene.onCreated += THIS_FUNC(OnObjectCreated);
		o2Scene.onDestroying += THIS_FUNC(OnObjectDestroing);

		auto updateTreeNode = THIS_FUNC(OnObjectChanged);
		o2Scene.onEnableChanged += updateTreeNode;
		o2Scene.onLockChanged += updateTreeNode;
		o2Scene.onNameChanged += updateTreeNode;
		o2Scene.onChildrenHierarchyChanged += updateTreeNode;

		mAttachedToSceneEvents = true;
	}

	void UISceneTree::DeattachFromSceneEvents()
	{
		if (Scene::IsSingletonInitialzed() && mAttachedToSceneEvents)
		{
			o2Scene.onObjectsChanged -= THIS_FUNC(OnObjectsChanged);

			o2Scene.onCreated -= THIS_FUNC(OnObjectCreated);
			o2Scene.onDestroying -= THIS_FUNC(OnObjectDestroing);

			auto updateTreeNode = THIS_FUNC(OnObjectChanged);
			o2Scene.onEnableChanged -= updateTreeNode;
			o2Scene.onLockChanged -= updateTreeNode;
			o2Scene.onNameChanged -= updateTreeNode;
			o2Scene.onChildrenHierarchyChanged -= updateTreeNode;
		}
	}

	UITreeNode* UISceneTree::GetNode(SceneEditableObject* object)
	{
		return UITree::GetNode((UnknownPtr)(void*)object);
	}

	UISceneTree::SceneEditableObjsVec UISceneTree::GetSelectedObjects() const
	{
		return UITree::GetSelectedObjects().Select<SceneEditableObject*>([](auto x) { return (SceneEditableObject*)(void*)x; });
	}

	void UISceneTree::SetSelectedObjects(const SceneEditableObjsVec& objects)
	{
		UITree::SetSelectedObjects(objects.Select<UnknownPtr>([](auto x) { return (UnknownPtr)(void*)x; }));
	}

	void UISceneTree::SelectObject(SceneEditableObject* object)
	{
		UITree::SelectObject((UnknownPtr)(void*)object);
	}

	void UISceneTree::SelectAndHightlightObject(SceneEditableObject* object)
	{
		UITree::SelectAndHightlightObject((UnknownPtr)(void*)object);
	}

	void UISceneTree::ScrollToAndHightlight(SceneEditableObject* object)
	{
		UITree::ScrollToAndHightlight(object);
	}

	void UISceneTree::DeselectObject(SceneEditableObject* object)
	{
		UITree::DeselectObject((UnknownPtr)(void*)object);
	}

	void UISceneTree::DeselectAllObjects()
	{
		UITree::DeselectAllObjects();
	}

	void UISceneTree::ScrollTo(SceneEditableObject* object)
	{
		UITree::ScrollTo((UnknownPtr)(void*)object);
	}

	void UISceneTree::SetEditorWatching(bool watching)
	{
		mWatchEditor = watching;

		UpdateNodesView();
	}

	void UISceneTree::Initialize()
	{
		mEnableTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
		mEnableTogglesGroup->onReleased = THIS_FUNC(EnableObjectsGroupReleased);

		mLockTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
		mLockTogglesGroup->onReleased = THIS_FUNC(LockObjectsGroupReleased);

		UISceneTreeNode* objectNodeWidgetSample = (UISceneTreeNode*)mNodeWidgetSample;
		objectNodeWidgetSample->InitializeControls();
	}

	void UISceneTree::UpdateVisibleNodes()
	{
		PushScopeEnterOnStack scope;
		UITree::UpdateVisibleNodes();
	}

	UnknownPtr UISceneTree::GetObjectParent(UnknownPtr object)
	{
		SceneEditableObject* sceneObject = object;
		return sceneObject->GetEditableParent();
	}

	Vector<UnknownPtr> UISceneTree::GetObjectChilds(UnknownPtr object)
	{
		if (object)
		{
			SceneEditableObject* parent = object;
			return parent->GetEditablesChildren().Cast<UnknownPtr>();
		}

		if (mWatchEditor)
			return EditorUIRoot.GetRootWidget()->GetEditablesChildren().Cast<UnknownPtr>();

		return o2Scene.GetRootActors().Select<UnknownPtr>([](Actor* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	String UISceneTree::GetObjectDebug(UnknownPtr object)
	{
		return object ? ((SceneEditableObject*)object)->GetName() : "null";
	}

	void UISceneTree::FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object)
	{
		UISceneTreeNode* node = (UISceneTreeNode*)nodeWidget;
		node->SetSceneObject(object);
		node->mLockToggle->SetToggleGroup(mLockTogglesGroup);
		node->mEnableToggle->SetToggleGroup(mEnableTogglesGroup);
	}

	void UISceneTree::OnNodeDblClick(UITreeNode* nodeWidget)
	{
		((UISceneTreeNode*)nodeWidget)->EnableEditName();
	}

	void UISceneTree::OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject)
	{
		SceneEditableObject* newParentEditableObject = (SceneEditableObject*)newParent;
		SceneEditableObject* prevEditableObject = prevObject;
		Vector<SceneEditableObject*> editableObjects = objects.Cast<SceneEditableObject*>();

		auto action = mnew ReparentAction(editableObjects);

		o2Scene.ReparentEditableObjects(editableObjects, newParentEditableObject, prevEditableObject);

		action->ObjectsReparented(parent, prevEditableObject);
		o2EditorApplication.DoneAction(action);

		UITree::OnDraggedObjects(objects, newParent, prevObject);
	}

	void UISceneTree::EnableObjectsGroupPressed(bool value)
	{}

	void UISceneTree::EnableObjectsGroupReleased(bool value)
	{
		SceneEditableObjsVec objects = mEnableTogglesGroup->GetToggled().Select<SceneEditableObject*>(
			[](UIToggle* x) { return ((UITreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew EnableAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void UISceneTree::LockObjectsGroupPressed(bool value)
	{}

	void UISceneTree::LockObjectsGroupReleased(bool value)
	{
		SceneEditableObjsVec objects = mLockTogglesGroup->GetToggled().Select<SceneEditableObject*>(
			[](UIToggle* x) { return ((UITreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew LockAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void UISceneTree::OnNodesSelectionChanged(UnknownPtrsVec objects)
	{
		onObjectsSelectionChanged(objects.Cast<SceneEditableObject*>());
		UITree::OnNodesSelectionChanged(objects);
	}

	void UISceneTree::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			assetsScroll->InstantiateDraggingAssets();

			if (!assetsScroll->mInstSceneDragObjects.IsEmpty())
			{
				UpdateNodesView(true);
				SetLayoutDirty();

				for (auto object : assetsScroll->mInstSceneDragObjects)
				{
					int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });
					CreateVisibleNodeWidget(mAllNodes[idx], idx);
				}

				Focus();
				SetSelectedObjects(assetsScroll->mInstSceneDragObjects);
				BeginDragging(GetNode(assetsScroll->mInstSceneDragObjects.Last()));
				mDragOffset = Vec2F();
			}
		}
		else UITree::OnDragEnter(group);
	}

	void UISceneTree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			DeselectAllObjects();
			EndDragging();
			assetsScroll->ClearInstantiatedDraggingAssets();
			assetsScroll->Focus();
		}
		else UITree::OnDragExit(group);
	}

	void UISceneTree::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			UpdateDraggingGraphics();
			UITree::OnDraggedAbove(this);
		}
		else UITree::OnDraggedAbove(group);
	}

	void UISceneTree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			UITree::OnDropped(this);

			assetsScroll->RegObjectssCreationAction();
			assetsScroll->mInstSceneDragObjects.Clear();
		}
		else UITree::OnDropped(group);
	}

	void UISceneTree::OnObjectCreated(SceneEditableObject* object)
	{
		UITree::OnObjectCreated(object, object->GetEditableParent());
	}

	void UISceneTree::OnObjectDestroing(SceneEditableObject* object)
	{
		UITree::OnObjectRemoved(object);
	}

	void UISceneTree::OnObjectsChanged(const SceneEditableObjsVec& objects)
	{
		UITree::OnObjectsChanged(objects.Cast<UnknownPtr>());
	}

	void UISceneTree::OnObjectChanged(SceneEditableObject* object)
	{
		UITree::OnObjectsChanged({ object });
	}

	UISceneTreeNode::UISceneTreeNode() :
		UITreeNode()
	{}

	UISceneTreeNode::UISceneTreeNode(const UISceneTreeNode& other) :
		UITreeNode(other)
	{
		CopyData(other);
		InitializeControls();
	}

	Editor::UISceneTreeNode& UISceneTreeNode::operator=(const UISceneTreeNode& other)
	{
		CopyData(other);
		return *this;
	}

	void UISceneTreeNode::CopyData(const Actor& otherActor)
	{
		const UISceneTreeNode& other = dynamic_cast<const UISceneTreeNode&>(otherActor);

		UITreeNode::CopyData(other);

		InitializeControls();
	}

	void UISceneTreeNode::OnDeserialized(const DataNode& node)
	{
		UITreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void UISceneTreeNode::InitializeControls()
	{
		mNameDrawable = GetLayerDrawable<Text>("name");
		mLockToggle = (UIToggle*)GetChild("lockToggle");
		mEnableToggle = (UIToggle*)GetChild("enableToggle");
		mLinkBtn = (UIButton*)GetChild("linkBtn");
		mNameEditBox = (UIEditBox*)GetChild("nameEditBox");
		mEditState = GetStateObject("edit");

		if (mLockToggle)
			mLockToggle->onClick = THIS_FUNC(OnLockClicked);

		if (mEnableToggle)
			mEnableToggle->onClick = THIS_FUNC(OnEnableCkicked);

		if (mNameEditBox)
			mNameEditBox->onChangeCompleted = THIS_FUNC(OnObjectNameChanged);
	}

	void UISceneTreeNode::SetSceneObject(SceneEditableObject* object)
	{
		mTargetObject = object;
		mName = object->GetName();

		float alpha = object->IsEnabledInHierarchy() ? 1.0f : 0.5f;

		mNameDrawable->SetText(mName);
		if (!Math::Equals(alpha, mNameDrawable->GetTransparency()))
		{
			mNameDrawable->SetTransparency(alpha);
			mEnableToggle->SetTransparency(alpha);
			mLinkBtn->SetTransparency(alpha);
		}

		if (Actor* actor = dynamic_cast<Actor*>(object))
		{
			mLinkBtn->SetEnabled(actor->GetPrototype());
			mLinkBtn->SetStateForcible("halfHide", !actor->GetPrototypeDirectly().IsValid());

			if (actor->GetPrototype())
			{
				mLinkBtn->onClick = [=]() {
					auto proto = actor->GetPrototype();
					o2EditorAssets.ShowAssetIcon(proto->GetPath());
				};
			}
		}
		else mLinkBtn->SetEnabled(false);

		if (object->IsSupportsDisabling())
		{
			mEnableToggle->SetValue(object->IsEnabled());
			mEnableToggle->SetEnabled(true);
		}
		else mEnableToggle->SetEnabled(false);

		mLockToggle->SetValue(object->IsLocked());
		mLockToggle->SetStateForcible("locked", object->IsLockedInHierarchy());
		mLockToggle->SetStateForcible("halfHide", object->IsLockedInHierarchy() && !object->IsLocked());
	}

	void UISceneTreeNode::EnableEditName()
	{
		mEditState->SetState(true);

		mNameEditBox->text = mTargetObject->GetName();
		mNameEditBox->SelectAll();
		mNameEditBox->UIWidget::Focus();
		mNameEditBox->ResetScroll();
	}

	void UISceneTreeNode::OnLockClicked()
	{
		mTargetObject->SetLocked(mLockToggle->GetValue());
	}

	void UISceneTreeNode::OnEnableCkicked()
	{
		mTargetObject->SetEnabled(mEnableToggle->GetValue());
	}

	void UISceneTreeNode::OnObjectNameChanged(const WString& text)
	{
		String prevName = mTargetObject->GetName();

		mTargetObject->SetName(text);
		mEditState->SetState(false);
		((UISceneTree*)mOwnerTree)->OnObjectChanged(mTargetObject);


		DataNode prevData; prevData = prevName;
		DataNode newData; newData = mTargetObject->GetName();

		auto action = mnew PropertyChangeAction({ mTargetObject }, "name", { prevData }, { newData });
		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::UISceneTree);

DECLARE_CLASS(Editor::UISceneTreeNode);
