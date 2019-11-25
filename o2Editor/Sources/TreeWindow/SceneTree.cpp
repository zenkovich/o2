#include "stdafx.h"
#include "SceneTree.h"

#include "Assets/ActorAsset.h"
#include "Assets/Assets.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "AssetsWindow/AssetIcon.h"
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
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/Toggle.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"

namespace Editor
{
	SceneTree::SceneTree() :
		Tree(), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		delete mNodeWidgetSample;
		mNodeWidgetSample = mnew SceneTreeNode();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		Initialize();
	}

	SceneTree::SceneTree(const SceneTree& other) :
		Tree(other), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	SceneTree::~SceneTree()
	{
		DeattachFromSceneEvents();
	}

	SceneTree& SceneTree::operator=(const SceneTree& other)
	{
		Tree::operator=(other);
		return *this;
	}

	void SceneTree::AttachToSceneEvents()
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

	void SceneTree::DeattachFromSceneEvents()
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

	TreeNode* SceneTree::GetNode(SceneEditableObject* object)
	{
		return Tree::GetNode((UnknownPtr)(void*)object);
	}

	SceneTree::SceneEditableObjsVec SceneTree::GetSelectedObjects() const
	{
		return Tree::GetSelectedObjects().Select<SceneEditableObject*>([](auto x) { return (SceneEditableObject*)(void*)x; });
	}

	void SceneTree::SetSelectedObjects(const SceneEditableObjsVec& objects)
	{
		Tree::SetSelectedObjects(objects.Select<UnknownPtr>([](auto x) { return (UnknownPtr)(void*)x; }));
	}

	void SceneTree::SelectObject(SceneEditableObject* object)
	{
		Tree::SelectObject((UnknownPtr)(void*)object);
	}

	void SceneTree::SelectAndHightlightObject(SceneEditableObject* object)
	{
		Tree::SelectAndHightlightObject((UnknownPtr)(void*)object);
	}

	void SceneTree::ScrollToAndHightlight(SceneEditableObject* object)
	{
		Tree::ScrollToAndHightlight(object);
	}

	void SceneTree::DeselectObject(SceneEditableObject* object)
	{
		Tree::DeselectObject((UnknownPtr)(void*)object);
	}

	void SceneTree::DeselectAllObjects()
	{
		Tree::DeselectAllObjects();
	}

	void SceneTree::ScrollTo(SceneEditableObject* object)
	{
		Tree::ScrollTo((UnknownPtr)(void*)object);
	}

	void SceneTree::SetEditorWatching(bool watching)
	{
		mWatchEditor = watching;

		UpdateNodesView();
	}

	void SceneTree::Initialize()
	{
		mEnableTogglesGroup = mnew ToggleGroup(ToggleGroup::Type::VerOneClick);
		mEnableTogglesGroup->onReleased = THIS_FUNC(EnableObjectsGroupReleased);

		mLockTogglesGroup = mnew ToggleGroup(ToggleGroup::Type::VerOneClick);
		mLockTogglesGroup->onReleased = THIS_FUNC(LockObjectsGroupReleased);

		SceneTreeNode* objectNodeWidgetSample = (SceneTreeNode*)mNodeWidgetSample;
		objectNodeWidgetSample->InitializeControls();
	}

	void SceneTree::UpdateVisibleNodes()
	{
		PushEditorScopeOnStack scope;
		Tree::UpdateVisibleNodes();
	}

	TreeNode* SceneTree::CreateTreeNodeWidget()
	{
		PushEditorScopeOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	UnknownPtr SceneTree::GetObjectParent(UnknownPtr object)
	{
		SceneEditableObject* sceneObject = object;
		return sceneObject->GetEditableParent();
	}

	Vector<UnknownPtr> SceneTree::GetObjectChilds(UnknownPtr object)
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

	String SceneTree::GetObjectDebug(UnknownPtr object)
	{
		return object ? ((SceneEditableObject*)object)->GetName() : "null";
	}

	void SceneTree::FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object)
	{
		SceneTreeNode* node = (SceneTreeNode*)nodeWidget;
		node->SetSceneObject(object);
		node->mLockToggle->SetToggleGroup(mLockTogglesGroup);
		node->mEnableToggle->SetToggleGroup(mEnableTogglesGroup);
	}

	void SceneTree::OnNodeDblClick(TreeNode* nodeWidget)
	{
		((SceneTreeNode*)nodeWidget)->EnableEditName();
	}

	void SceneTree::OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject)
	{
		SceneEditableObject* newParentEditableObject = (SceneEditableObject*)newParent;
		SceneEditableObject* prevEditableObject = prevObject;
		Vector<SceneEditableObject*> editableObjects = objects.Cast<SceneEditableObject*>();

		auto action = mnew ReparentAction(editableObjects);

		o2Scene.ReparentEditableObjects(editableObjects, newParentEditableObject, prevEditableObject);

		action->ObjectsReparented(mParent, prevEditableObject);
		o2EditorApplication.DoneAction(action);

		Tree::OnDraggedObjects(objects, newParent, prevObject);
	}

	void SceneTree::EnableObjectsGroupPressed(bool value)
	{}

	void SceneTree::EnableObjectsGroupReleased(bool value)
	{
		SceneEditableObjsVec objects = mEnableTogglesGroup->GetToggled().Select<SceneEditableObject*>(
			[](Toggle* x) { return ((TreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew EnableAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void SceneTree::LockObjectsGroupPressed(bool value)
	{}

	void SceneTree::LockObjectsGroupReleased(bool value)
	{
		SceneEditableObjsVec objects = mLockTogglesGroup->GetToggled().Select<SceneEditableObject*>(
			[](Toggle* x) { return ((TreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew LockAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void SceneTree::OnNodesSelectionChanged(UnknownPtrsVec objects)
	{
		onObjectsSelectionChanged(objects.Cast<SceneEditableObject*>());
		Tree::OnNodesSelectionChanged(objects);
	}

	void SceneTree::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
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
		else Tree::OnDragEnter(group);
	}

	void SceneTree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			DeselectAllObjects();
			EndDragging();
			assetsScroll->ClearInstantiatedDraggingAssets();
			assetsScroll->Focus();
		}
		else Tree::OnDragExit(group);
	}

	void SceneTree::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			UpdateDraggingGraphics();
			Tree::OnDraggedAbove(this);
		}
		else Tree::OnDraggedAbove(group);
	}

	void SceneTree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			Tree::OnDropped(this);

			assetsScroll->RegObjectsCreationAction();
			assetsScroll->mInstSceneDragObjects.Clear();
		}
		else Tree::OnDropped(group);
	}

	void SceneTree::OnObjectCreated(SceneEditableObject* object)
	{
		Tree::OnObjectCreated(object, object->GetEditableParent());
	}

	void SceneTree::OnObjectDestroing(SceneEditableObject* object)
	{
		Tree::OnObjectRemoved(object);
	}

	void SceneTree::OnObjectsChanged(const SceneEditableObjsVec& objects)
	{
		Tree::OnObjectsChanged(objects.Cast<UnknownPtr>());
	}

	void SceneTree::OnObjectChanged(SceneEditableObject* object)
	{
		Tree::OnObjectsChanged({ object });
	}

	SceneTreeNode::SceneTreeNode() :
		TreeNode()
	{}

	SceneTreeNode::SceneTreeNode(const SceneTreeNode& other) :
		TreeNode(other)
	{
		CopyData(other);
		InitializeControls();
	}

	Editor::SceneTreeNode& SceneTreeNode::operator=(const SceneTreeNode& other)
	{
		CopyData(other);
		return *this;
	}

	void SceneTreeNode::CopyData(const Actor& otherActor)
	{
		TreeNode::CopyData(otherActor);
		InitializeControls();
	}

	void SceneTreeNode::OnDeserialized(const DataNode& node)
	{
		TreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void SceneTreeNode::InitializeControls()
	{
		mNameDrawable = GetLayerDrawable<Text>("name");
		mLockToggle = (Toggle*)GetChild("lockToggle");
		mEnableToggle = (Toggle*)GetChild("enableToggle");
		mLinkBtn = (Button*)GetChild("linkBtn");
		mNameEditBox = (EditBox*)GetChild("nameEditBox");
		mEditState = GetStateObject("edit");

		if (mLockToggle)
			mLockToggle->onClick = THIS_FUNC(OnLockClicked);

		if (mEnableToggle)
			mEnableToggle->onClick = THIS_FUNC(OnEnableCkicked);

		if (mNameEditBox)
			mNameEditBox->onChangeCompleted = THIS_FUNC(OnObjectNameChanged);
	}

	void SceneTreeNode::SetSceneObject(SceneEditableObject* object)
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

	void SceneTreeNode::EnableEditName()
	{
		mEditState->SetState(true);

		mNameEditBox->text = mTargetObject->GetName();
		mNameEditBox->SelectAll();
		mNameEditBox->Widget::Focus();
		mNameEditBox->ResetScroll();
	}

	void SceneTreeNode::OnLockClicked()
	{
		mTargetObject->SetLocked(mLockToggle->GetValue());
	}

	void SceneTreeNode::OnEnableCkicked()
	{
		mTargetObject->SetEnabled(mEnableToggle->GetValue());
	}

	void SceneTreeNode::OnObjectNameChanged(const WString& text)
	{
		String prevName = mTargetObject->GetName();

		mTargetObject->SetName(text);
		mEditState->SetState(false);
		((SceneTree*)mOwnerTree)->OnObjectChanged(mTargetObject);


		DataNode prevData; prevData = prevName;
		DataNode newData; newData = mTargetObject->GetName();

		auto action = mnew PropertyChangeAction({ mTargetObject }, "name", { prevData }, { newData });
		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::SceneTree);

DECLARE_CLASS(Editor::SceneTreeNode);
