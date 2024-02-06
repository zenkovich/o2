#include "o2Editor/stdafx.h"
#include "SceneHierarchyTree.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Events/EventSystem.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/CameraActor.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/AssetsWindow/AssetIcon.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Actions/Enable.h"
#include "o2Editor/Core/Actions/Lock.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/Actions/Reparent.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Properties/Basic/ActorProperty.h"
#include "o2Editor/Core/Properties/Basic/ComponentProperty.h"
#include "o2Editor/Core/UIRoot.h"

namespace Editor
{
	SceneHierarchyTree::SceneHierarchyTree() :
		Tree(), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		delete mNodeWidgetSample;
		mNodeWidgetSample = mnew SceneHierarchyTreeNode();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		Initialize();
	}

	SceneHierarchyTree::SceneHierarchyTree(const SceneHierarchyTree& other) :
		Tree(other), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	SceneHierarchyTree::~SceneHierarchyTree()
	{
		DeattachFromSceneEvents();
	}

	SceneHierarchyTree& SceneHierarchyTree::operator=(const SceneHierarchyTree& other)
	{
		Tree::operator=(other);
		return *this;
	}

	void SceneHierarchyTree::AttachToSceneEvents()
	{
		o2Scene.onObjectsChanged += THIS_FUNC(OnObjectsChanged);

		o2Scene.onAddedToScene += THIS_FUNC(OnObjectCreated);
		o2Scene.onRemovedFromScene += THIS_FUNC(OnObjectDestroing);

		auto updateTreeNode = THIS_FUNC(OnObjectChanged);
		o2Scene.onEnableChanged += updateTreeNode;
		o2Scene.onLockChanged += updateTreeNode;
		o2Scene.onNameChanged += updateTreeNode;
		o2Scene.onChildrenHierarchyChanged += updateTreeNode;

		mAttachedToSceneEvents = true;
	}

	void SceneHierarchyTree::DeattachFromSceneEvents()
	{
		if (Scene::IsSingletonInitialzed() && mAttachedToSceneEvents)
		{
			o2Scene.onObjectsChanged -= THIS_FUNC(OnObjectsChanged);

			o2Scene.onAddedToScene -= THIS_FUNC(OnObjectCreated);
			o2Scene.onRemovedFromScene -= THIS_FUNC(OnObjectDestroing);

			auto updateTreeNode = THIS_FUNC(OnObjectChanged);
			o2Scene.onEnableChanged -= updateTreeNode;
			o2Scene.onLockChanged -= updateTreeNode;
			o2Scene.onNameChanged -= updateTreeNode;
			o2Scene.onChildrenHierarchyChanged -= updateTreeNode;
		}
	}

	TreeNode* SceneHierarchyTree::GetNode(SceneEditableObject* object)
	{
		return Tree::GetNode((void*)(void*)object);
	}

	Vector<SceneEditableObject*> SceneHierarchyTree::GetSelectedObjects() const
	{
		return Tree::GetSelectedObjects().Convert<SceneEditableObject*>([](auto x) { return (SceneEditableObject*)(void*)x; });
	}

	void SceneHierarchyTree::SetSelectedObjects(const Vector<SceneEditableObject*>& objects)
	{
		Tree::SetSelectedObjects(objects.Convert<void*>([](auto x) { return (void*)(void*)x; }));
	}

	void SceneHierarchyTree::SelectObject(SceneEditableObject* object)
	{
		Tree::SelectObject((void*)(void*)object);
	}

	void SceneHierarchyTree::SelectAndHighlightObject(SceneEditableObject* object)
	{
		Tree::SelectAndHighlightObject((void*)(void*)object);
	}

	void SceneHierarchyTree::ScrollToAndHighlight(SceneEditableObject* object)
	{
		Tree::ScrollToAndHighlight(object);
	}

	void SceneHierarchyTree::DeselectObject(SceneEditableObject* object)
	{
		Tree::DeselectObject((void*)(void*)object);
	}

	void SceneHierarchyTree::DeselectAllObjects()
	{
		Tree::DeselectAllObjects();
	}

	void SceneHierarchyTree::ScrollTo(SceneEditableObject* object)
	{
		Tree::ScrollTo((void*)(void*)object);
	}

	void SceneHierarchyTree::SetEditorWatching(bool watching)
	{
		mWatchEditor = watching;
		UpdateNodesView();
	}

	bool SceneHierarchyTree::IsEditorWatching() const
	{
		return mWatchEditor;
	}

	String SceneHierarchyTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void SceneHierarchyTree::Initialize()
	{
		mEnableTogglesGroup = mnew ToggleGroup(ToggleGroup::Type::VerOneClick);
		mEnableTogglesGroup->onReleased = THIS_FUNC(EnableObjectsGroupReleased);

		mLockTogglesGroup = mnew ToggleGroup(ToggleGroup::Type::VerOneClick);
		mLockTogglesGroup->onReleased = THIS_FUNC(LockObjectsGroupReleased);

		SceneHierarchyTreeNode* objectNodeWidgetSample = (SceneHierarchyTreeNode*)mNodeWidgetSample;
		objectNodeWidgetSample->InitializeControls();
	}

	void SceneHierarchyTree::UpdateVisibleNodes()
	{
		PushEditorScopeOnStack scope;
		Tree::UpdateVisibleNodes();
	}

	TreeNode* SceneHierarchyTree::CreateTreeNodeWidget()
	{
		PushEditorScopeOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	void* SceneHierarchyTree::GetObjectParent(void* object)
	{
		SceneEditableObject* sceneObject = (SceneEditableObject*)object;
		return sceneObject->GetEditableParent();
	}

	Vector<void*> SceneHierarchyTree::GetObjectChilds(void* object)
	{
		if (object)
		{
			SceneEditableObject* parent = (SceneEditableObject*)object;
			return parent->GetEditableChildren().Cast<void*>();
		}

		if (mWatchEditor)
			return EditorUIRoot.GetRootWidget()->GetEditableChildren().Cast<void*>();

		return o2Scene.GetRootActors().Convert<void*>([](Actor* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	String SceneHierarchyTree::GetObjectDebug(void* object)
	{
		return object ? ((SceneEditableObject*)object)->GetName() : String("null");
	}

	void SceneHierarchyTree::FillNodeDataByObject(TreeNode* nodeWidget, void* object)
	{
		SceneHierarchyTreeNode* node = (SceneHierarchyTreeNode*)nodeWidget;
		node->SetSceneObject((SceneEditableObject*)object);
		node->mLockToggle->SetToggleGroup(mLockTogglesGroup);
		node->mEnableToggle->SetToggleGroup(mEnableTogglesGroup);
	}

	void SceneHierarchyTree::OnNodeDblClick(const Ref<TreeNode>& nodeWidget)
	{
		((SceneHierarchyTreeNode*)nodeWidget)->EnableEditName();
	}

	void SceneHierarchyTree::OnDraggedObjects(Vector<void*> objects, void* newParent, void* prevObject)
	{
		SceneEditableObject* newParentEditableObject = (SceneEditableObject*)newParent;
		SceneEditableObject* prevEditableObject = (SceneEditableObject*)prevObject;
		Vector<SceneEditableObject*> editableObjects = objects.Cast<SceneEditableObject*>();

		auto action = mnew ReparentAction(editableObjects);

		o2Scene.ReparentEditableObjects(editableObjects, newParentEditableObject, prevEditableObject);

		action->ObjectsReparented(mParent, prevEditableObject);
		o2EditorApplication.DoneAction(action);

		Tree::OnDraggedObjects(objects, newParent, prevObject);
	}

	void SceneHierarchyTree::EnableObjectsGroupPressed(bool value)
	{}

	void SceneHierarchyTree::EnableObjectsGroupReleased(bool value)
	{
		Vector<SceneEditableObject*> objects = mEnableTogglesGroup->GetToggled().Convert<SceneEditableObject*>(
			[](Toggle* x) { return (SceneEditableObject*)((TreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew EnableAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void SceneHierarchyTree::LockObjectsGroupPressed(bool value)
	{}

	void SceneHierarchyTree::LockObjectsGroupReleased(bool value)
	{
		Vector<SceneEditableObject*> objects = mLockTogglesGroup->GetToggled().Convert<SceneEditableObject*>(
			[](Toggle* x) { return (SceneEditableObject*)((TreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew LockAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void SceneHierarchyTree::OnNodesSelectionChanged(Vector<void*> objects)
	{
		onObjectsSelectionChanged(objects.Cast<SceneEditableObject*>());
		Tree::OnNodesSelectionChanged(objects);
	}

	void SceneHierarchyTree::OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			assetsScroll->InstantiateDraggingAssets();

			if (!assetsScroll->mInstantiatedSceneDragObjects.IsEmpty())
			{
				o2Scene.UpdateAddedEntities();

				UpdateNodesView(true);
				SetLayoutDirty();

				for (auto& object : assetsScroll->mInstantiatedSceneDragObjects)
				{
					int idx = mAllNodes.IndexOf([=](Node* x) { return x->object == object; });
					CreateVisibleNodeWidget(mAllNodes[idx], idx);
				}

				Focus();
				SetSelectedObjects(assetsScroll->mInstantiatedSceneDragObjects);
				BeginDragging(GetNode(assetsScroll->mInstantiatedSceneDragObjects.Last()));
				mDragOffset = Vec2F();
			}
		}
		else Tree::OnDragEnter(group);
	}

	void SceneHierarchyTree::OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group)
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

	void SceneHierarchyTree::OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			UpdateDraggingGraphics();
			Tree::OnDraggedAbove(this);
		}
		else Tree::OnDraggedAbove(group);
	}

	void SceneHierarchyTree::OnDropped(const Ref<ISelectableDragableObjectsGroup>& group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (assetsScroll)
		{
			Tree::OnDropped(this);

			assetsScroll->RegObjectsCreationAction();
			assetsScroll->mInstantiatedSceneDragObjects.Clear();
		}
		else Tree::OnDropped(group);
	}

	void SceneHierarchyTree::OnObjectCreated(SceneEditableObject* object)
	{
		Tree::OnObjectCreated(object, object->GetEditableParent());
	}

	void SceneHierarchyTree::OnObjectDestroing(SceneEditableObject* object)
	{
		Tree::OnObjectRemoved(object);
	}

	void SceneHierarchyTree::OnObjectsChanged(const Vector<SceneEditableObject*>& objects)
	{
		Tree::OnObjectsChanged(objects.Cast<void*>());
	}

	void SceneHierarchyTree::OnObjectChanged(SceneEditableObject* object)
	{
		Tree::OnObjectsChanged({ object });
	}

	SceneHierarchyTreeNode::SceneHierarchyTreeNode() :
		TreeNode()
	{}

	SceneHierarchyTreeNode::SceneHierarchyTreeNode(const SceneHierarchyTreeNode& other) :
		TreeNode(other)
	{
		InitializeControls();
	}

	Editor::SceneHierarchyTreeNode& SceneHierarchyTreeNode::operator=(const SceneHierarchyTreeNode& other)
	{
		TreeNode::operator=(other);
		InitializeControls();
		return *this;
	}

	void SceneHierarchyTreeNode::OnDeserialized(const DataValue& node)
	{
		TreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void SceneHierarchyTreeNode::InitializeControls()
	{
		mNameDrawable = GetLayerDrawable<Text>("name");
		mLockToggle = (Toggle*)GetChild("lockToggle");
		mEnableToggle = (Toggle*)GetChild("enableToggle");
		mLinkBtn = (Button*)GetChild("linkBtn");
		mNameEditBox = (EditBox*)GetChild("nameEditBox");
		mEditState = GetStateObject("edit");

		if (mLinkBtn)
			mLinkBtnHalfHideState = mLinkBtn->GetStateObject("halfHide");

		if (mLockToggle)
		{
			mLockToggleHalfHideState = mLockToggle->GetStateObject("halfHide");
			mLockToggleLockedState = mLockToggle->GetStateObject("locked");

			mLockToggle->onClick = THIS_FUNC(OnLockClicked);
		}

		if (mEnableToggle)
			mEnableToggle->onClick = THIS_FUNC(OnEnableCkicked);

		if (mNameEditBox)
			mNameEditBox->onChangeCompleted = THIS_FUNC(OnObjectNameChanged);
	}

	void SceneHierarchyTreeNode::SetSceneObject(SceneEditableObject* object)
	{
		mTargetObject = object;

		if (mName != object->GetName())
		{
			mName = object->GetName();
			mNameDrawable->SetText(mName);
		}

		float alpha = object->IsEnabledInHierarchy() ? 1.0f : 0.5f;
		if (!Math::Equals(alpha, mNameDrawable->GetTransparency()))
		{
			mNameDrawable->SetTransparency(alpha);
			mEnableToggle->SetTransparency(alpha);
			mLinkBtn->SetTransparency(alpha);
		}

		if (Actor* actor = dynamic_cast<Actor*>(object))
		{
			mLinkBtn->SetEnabled(actor->GetPrototype());
			mLinkBtnHalfHideState->SetState(!actor->GetPrototypeDirectly().IsValid());

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
		mLockToggleLockedState->SetState(object->IsLockedInHierarchy());
		mLockToggleHalfHideState->SetState(object->IsLockedInHierarchy() && !object->IsLocked());
	}

	void SceneHierarchyTreeNode::EnableEditName()
	{
		mEditState->SetState(true);

		mNameEditBox->text = mTargetObject->GetName();
		mNameEditBox->SelectAll();
		mNameEditBox->Widget::Focus();
		mNameEditBox->ResetScroll();
	}

	String SceneHierarchyTreeNode::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void SceneHierarchyTreeNode::OnLockClicked()
	{
		mTargetObject->SetLocked(mLockToggle->GetValue());
	}

	void SceneHierarchyTreeNode::OnEnableCkicked()
	{
		mTargetObject->SetEnabled(mEnableToggle->GetValue());
	}

	void SceneHierarchyTreeNode::OnObjectNameChanged(const WString& text)
	{
		String prevName = mTargetObject->GetName();

		mTargetObject->SetName(text);
		mEditState->SetState(false);
		((SceneHierarchyTree*)mOwnerTree)->OnObjectChanged(mTargetObject);


		DataDocument prevData; prevData = prevName;
		DataDocument newData; newData = mTargetObject->GetName();

		auto action = mnew PropertyChangeAction({ mTargetObject }, "name", { prevData }, { newData });
		o2EditorApplication.DoneAction(action);
	}

}
// --- META ---

DECLARE_CLASS(Editor::SceneHierarchyTree, Editor__SceneHierarchyTree);

DECLARE_CLASS(Editor::SceneHierarchyTreeNode, Editor__SceneHierarchyTreeNode);
// --- END META ---
