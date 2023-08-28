#include "o2Editor/stdafx.h"
#include "DrawOrderTree.h"

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
	DrawOrderTree::DrawOrderTree() :
		Tree(), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		delete mNodeWidgetSample;
		mNodeWidgetSample = mnew DrawOrderTreeNode();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		Initialize();
	}

	DrawOrderTree::DrawOrderTree(const DrawOrderTree& other) :
		Tree(other), mAttachedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	DrawOrderTree::~DrawOrderTree()
	{
		DeattachFromSceneEvents();
	}

	DrawOrderTree& DrawOrderTree::operator=(const DrawOrderTree& other)
	{
		Tree::operator=(other);
		return *this;
	}

	void DrawOrderTree::AttachToSceneEvents()
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

	void DrawOrderTree::DeattachFromSceneEvents()
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

	String DrawOrderTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void DrawOrderTree::Initialize()
	{
		mEnableTogglesGroup = mnew ToggleGroup(ToggleGroup::Type::VerOneClick);
		mEnableTogglesGroup->onReleased = THIS_FUNC(EnableObjectsGroupReleased);

		mLockTogglesGroup = mnew ToggleGroup(ToggleGroup::Type::VerOneClick);
		mLockTogglesGroup->onReleased = THIS_FUNC(LockObjectsGroupReleased);

		DrawOrderTreeNode* objectNodeWidgetSample = (DrawOrderTreeNode*)mNodeWidgetSample;
		objectNodeWidgetSample->InitializeControls();
	}

	void DrawOrderTree::SetSelectedObjects(const Vector<SceneEditableObject*>& objects)
	{
		Vector<OrderTreeNode*> nodes;
		for (auto object : objects)
		{
			OrderTreeNode* node = nullptr;
			if (mObjectToNodeMap.TryGetValue(object, node))
				nodes.Add(node);
		}

		Tree::SetSelectedObjects(nodes.Convert<void*>([](auto x) { return (void*)x; }));
	}

	void DrawOrderTree::ScrollToAndHighlight(SceneEditableObject* object)
	{
		OrderTreeNode* node = nullptr;
		if (mObjectToNodeMap.TryGetValue(object, node))
			Tree::ScrollToAndHighlight(node);
	}

	void DrawOrderTree::RebuildOrderTree()
	{
		mRootOrderNodes.Clear();
		mObjectToNodeMap.Clear();

		mStartBatchIdx = INT_MAX;

		for (auto camera : o2Scene.GetCameras())
		{
			OrderTreeNode* cameraNode = mnew OrderTreeNode();
			cameraNode->name = camera->GetName();
			cameraNode->type = OrderTreeNode::Type::Camera;
			cameraNode->object = camera;
			mObjectToNodeMap[cameraNode->object] = cameraNode;
			mRootOrderNodes.Add(cameraNode);

			for (auto layer : camera->drawLayers.GetLayers())
			{
				OrderTreeNode* layerNode = mnew OrderTreeNode();
				layerNode->name = layer->GetName();
				layerNode->type = OrderTreeNode::Type::Layer;
				cameraNode->AddChild(layerNode);

				for (auto drawable : layer->GetDrawables())
					ProcessDrawableTreeNode(layerNode, drawable);
			}
		}
	}

	void DrawOrderTree::ProcessDrawableTreeNode(OrderTreeNode* parent, ISceneDrawable* drawable)
	{
		OrderTreeNode* drawableNode = mnew OrderTreeNode();

		drawableNode->inheritedOrderFromParent = drawable->IsDrawingDepthInheritedFromParent();
		drawableNode->customOrder = drawable->GetDrawingDepth();
		drawableNode->batchIdx = drawable->drawCallIdx;

		if (drawableNode->batchIdx < mStartBatchIdx)
			mStartBatchIdx = drawableNode->batchIdx;

		if (auto actor = dynamic_cast<Actor*>(drawable))
		{
			drawableNode->name = actor->name;
			drawableNode->type = OrderTreeNode::Type::Actor;
			drawableNode->object = actor;

			mObjectToNodeMap[drawableNode->object] = drawableNode; 

			CheckBatchEnd(drawableNode);
			parent->AddChild(drawableNode);
		}
		else if (auto root = dynamic_cast<SceneLayer::RootDrawablesContainer*>(drawable))
		{
			drawableNode->type = OrderTreeNode::Type::Root;
			drawableNode->name = "Layer roots";

			CheckBatchEnd(drawableNode);
			parent->AddChild(drawableNode);
		}
		else
		{
			drawableNode->name = drawable->GetType().GetName();
			drawableNode->type = OrderTreeNode::Type::Drawable;

			CheckBatchEnd(drawableNode);
			parent->AddChild(drawableNode);
		}

		for (auto inherited : drawable->GetChildrenInheritedDepth())
			ProcessDrawableTreeNode(drawableNode, inherited);
	}

	void DrawOrderTree::CheckBatchEnd(OrderTreeNode* node)
	{
		static OrderTreeNode* prevDrawableNode = nullptr;

		if (prevDrawableNode && node->object != nullptr && node->batchIdx != prevDrawableNode->batchIdx)
		{
			OrderTreeNode* endNode = mnew OrderTreeNode();
			endNode->type = OrderTreeNode::Type::EndOfBatch;
			endNode->batchIdx = prevDrawableNode->batchIdx;

			endNode->name = "End of batch #" + (String)(endNode->batchIdx - mStartBatchIdx);

			auto parent = prevDrawableNode->GetParent();
			parent->AddChild(endNode);
		}

		if (node->object && node->batchIdx >= 0)
			prevDrawableNode = node;
	}

	void DrawOrderTree::UpdateNodesStructure()
	{
		RebuildOrderTree();
		Tree::UpdateNodesStructure();
	}

	void DrawOrderTree::UpdateVisibleNodes()
	{
		PushEditorScopeOnStack scope;
		Tree::UpdateVisibleNodes();
	}

	TreeNode* DrawOrderTree::CreateTreeNodeWidget()
	{
		PushEditorScopeOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	void* DrawOrderTree::GetObjectParent(void* object)
	{
		if (!object)
			return nullptr;

		OrderTreeNode* treeNode = (OrderTreeNode*)(void*)object;
		return (void*)(treeNode->GetParent());
	}

	Vector<void*> DrawOrderTree::GetObjectChilds(void* object)
	{
		OrderTreeNode* treeNode = (OrderTreeNode*)object;
		if (OrderTreeNode* treeNode = (OrderTreeNode*)object)
			return treeNode->GetChildren().Convert<void*>([](auto x) { return (void*)x; });

		return mRootOrderNodes.Convert<void*>([](auto x) { return (void*)x; });
	}

	void DrawOrderTree::FillNodeDataByObject(TreeNode* nodeWidget, void* object)
	{
		DrawOrderTreeNode* node = (DrawOrderTreeNode*)nodeWidget;
		node->Setup((OrderTreeNode*)object);
		node->mLockToggle->SetToggleGroup(mLockTogglesGroup);
		node->mEnableToggle->SetToggleGroup(mEnableTogglesGroup);
	}

	void DrawOrderTree::OnNodeDblClick(TreeNode* nodeWidget)
	{
		((DrawOrderTreeNode*)nodeWidget)->EnableEditName();
	}

	void DrawOrderTree::OnDraggedObjects(Vector<void*> objects, void* newParent, void* prevObject)
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

	void DrawOrderTree::EnableObjectsGroupPressed(bool value)
	{}

	void DrawOrderTree::EnableObjectsGroupReleased(bool value)
	{
		Vector<SceneEditableObject*> objects = mEnableTogglesGroup->GetToggled().Convert<SceneEditableObject*>(
			[](Toggle* x) { return (SceneEditableObject*)((TreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew EnableAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void DrawOrderTree::LockObjectsGroupPressed(bool value)
	{}

	void DrawOrderTree::LockObjectsGroupReleased(bool value)
	{
		Vector<SceneEditableObject*> objects = mLockTogglesGroup->GetToggled().Convert<SceneEditableObject*>(
			[](Toggle* x) { return (SceneEditableObject*)((TreeNode*)x->GetParent())->GetObject(); });

		auto action = mnew LockAction(objects, value);
		o2EditorApplication.DoneAction(action);
	}

	void DrawOrderTree::OnNodesSelectionChanged(Vector<void*> objects)
	{
		Vector<SceneEditableObject*> editableObjects = objects.Cast<OrderTreeNode*>()
			.FindAll([](auto x) { return x->object; })
			.Convert<SceneEditableObject*>([](auto x) { return x->object; });

		onObjectsSelectionChanged(editableObjects);
		Tree::OnNodesSelectionChanged(objects);
	}

	void DrawOrderTree::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		Tree::OnDragEnter(group);
	}

	void DrawOrderTree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		Tree::OnDragExit(group);
	}

	void DrawOrderTree::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		Tree::OnDraggedAbove(group);
	}

	void DrawOrderTree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		Tree::OnDropped(group);
	}

	void DrawOrderTree::OnObjectCreated(SceneEditableObject* object)
	{
		RebuildOrderTree();
		Tree::OnObjectCreated(object, object->GetEditableParent());
	}

	void DrawOrderTree::OnObjectDestroing(SceneEditableObject* object)
	{
		RebuildOrderTree();
		Tree::OnObjectRemoved(object);
	}

	void DrawOrderTree::OnObjectsChanged(const Vector<SceneEditableObject*>& objects)
	{
		RebuildOrderTree();
		Tree::OnObjectsChanged(objects.Cast<void*>());
	}

	void DrawOrderTree::OnObjectChanged(SceneEditableObject* object)
	{
		Tree::OnObjectsChanged({ object });
	}

	DrawOrderTreeNode::DrawOrderTreeNode() :
		TreeNode()
	{}

	DrawOrderTreeNode::DrawOrderTreeNode(const DrawOrderTreeNode& other) :
		TreeNode(other)
	{
		InitializeControls();
	}

	Editor::DrawOrderTreeNode& DrawOrderTreeNode::operator=(const DrawOrderTreeNode& other)
	{
		TreeNode::operator=(other);
		InitializeControls();
		return *this;
	}

	void DrawOrderTreeNode::OnDeserialized(const DataValue& node)
	{
		TreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void DrawOrderTreeNode::InitializeControls()
	{
		mNameDrawable = GetLayerDrawable<Text>("name");
		mOrderDrawable = GetLayerDrawable<Text>("batch");
		mIconSprite = GetLayerDrawable<Sprite>("icon");
		mBackSprite = GetLayerDrawable<Sprite>("back");
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

	void DrawOrderTreeNode::Setup(DrawOrderTree::OrderTreeNode* object)
	{
		mTarget = object;

		Map<DrawOrderTree::OrderTreeNode::Type, String> icons =
		{
			{ DrawOrderTree::OrderTreeNode::Type::Camera, "ui/UI4_camera_icn.png" },
			{ DrawOrderTree::OrderTreeNode::Type::Layer, "ui/UI4_layer_big.png" },
			{ DrawOrderTree::OrderTreeNode::Type::Drawable, "ui/UI4_image_icn.png" },
			{ DrawOrderTree::OrderTreeNode::Type::Actor, "ui/UI4_image_icn.png" },
			{ DrawOrderTree::OrderTreeNode::Type::Root, "ui/UI4_folder_icn.png" },
			{ DrawOrderTree::OrderTreeNode::Type::EndOfBatch, "ui/UI4_color_type.png" },
		};

		String name = mTarget->name;

		if (mName != name)
		{
			mName = name;
			mNameDrawable->SetText(mName);
		}

		WString orderText;
		if ((!mTarget->inheritedOrderFromParent && mTarget->object) || mTarget->type == DrawOrderTree::OrderTreeNode::Type::Root)
			orderText = (WString)mTarget->customOrder;
		else if (mTarget->inheritedOrderFromParent)
			orderText = "inherited";

		if (orderText != mOrderDrawable->GetText())
			mOrderDrawable->SetText(orderText);

		String icon = icons[mTarget->type];
		if (mIconSprite->GetImageName() != icon)
			mIconSprite->SetImageAsset(ImageAssetRef(icon));
		
		mBackSprite->color = Color4::SomeColor(mTarget->batchIdx, mBackSprite->GetColor().a);
		mBackSprite->enabled = mTarget->type != DrawOrderTree::OrderTreeNode::Type::Camera &&
			mTarget->type != DrawOrderTree::OrderTreeNode::Type::Layer &&
			mTarget->type != DrawOrderTree::OrderTreeNode::Type::Root;

		if (mTarget->object)
		{
			auto sceneObject = mTarget->object;
			float alpha = sceneObject->IsEnabledInHierarchy() ? 1.0f : 0.5f;
			if (!Math::Equals(alpha, mNameDrawable->GetTransparency()))
			{
				mNameDrawable->SetTransparency(alpha);
				mEnableToggle->SetTransparency(alpha);
				mLinkBtn->SetTransparency(alpha);
			}

			if (Actor* actor = dynamic_cast<Actor*>(sceneObject))
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
			else
				mLinkBtn->SetEnabled(false);

			if (sceneObject->IsSupportsDisabling())
			{
				mEnableToggle->SetValue(sceneObject->IsEnabled());
				mEnableToggle->SetEnabled(true);
			}
			else
				mEnableToggle->SetEnabled(false);

			mLockToggle->SetEnabled(true);
			mLockToggle->SetValue(sceneObject->IsLocked());
			mLockToggleLockedState->SetState(sceneObject->IsLockedInHierarchy());
			mLockToggleHalfHideState->SetState(sceneObject->IsLockedInHierarchy() && !sceneObject->IsLocked());
		}
		else
		{
			mEnableToggle->SetEnabled(false);
			mLockToggle->SetEnabled(false);
			mLinkBtn->SetEnabled(false);
		}
	}

	void DrawOrderTreeNode::EnableEditName()
	{
		if (mTarget->object)
		{
			mEditState->SetState(true);

			mNameEditBox->text = mTarget->object->GetName();
			mNameEditBox->SelectAll();
			mNameEditBox->Widget::Focus();
			mNameEditBox->ResetScroll();
		}
	}

	String DrawOrderTreeNode::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void DrawOrderTreeNode::OnLockClicked()
	{
		if (mTarget->object)
			mTarget->object->SetLocked(mLockToggle->GetValue());
	}

	void DrawOrderTreeNode::OnEnableCkicked()
	{
		if (mTarget->object)
			mTarget->object->SetEnabled(mEnableToggle->GetValue());
	}

	void DrawOrderTreeNode::OnObjectNameChanged(const WString& text)
	{
		if (mTarget->object)
		{
			String prevName = mTarget->object->GetName();

			mTarget->object->SetName(text);
			mEditState->SetState(false);
			((DrawOrderTree*)mOwnerTree)->OnObjectChanged(mTarget->object);


			DataDocument prevData; prevData = prevName;
			DataDocument newData; newData = mTarget->object->GetName();

			auto action = mnew PropertyChangeAction({ mTarget->object }, "name", { prevData }, { newData });
			o2EditorApplication.DoneAction(action);
		}
	}

}
// --- META ---

ENUM_META(Editor::DrawOrderTree::OrderTreeNode::Type)
{
	ENUM_ENTRY(Actor);
	ENUM_ENTRY(Camera);
	ENUM_ENTRY(Drawable);
	ENUM_ENTRY(EndOfBatch);
	ENUM_ENTRY(Layer);
	ENUM_ENTRY(Root);
}
END_ENUM_META;

DECLARE_CLASS(Editor::DrawOrderTree);

DECLARE_CLASS(Editor::DrawOrderTreeNode);
// --- END META ---
