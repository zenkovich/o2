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
		mNodeWidgetSample = mmake<DrawOrderTreeNode>();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", Ref<WidgetLayer>());

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
		o2Scene.onObjectsChanged += &DrawOrderTree::OnObjectsChanged;

		o2Scene.onAddedToScene += &DrawOrderTree::OnObjectCreated;
		o2Scene.onRemovedFromScene += &DrawOrderTree::OnObjectDestroing;

		auto updateTreeNode = &DrawOrderTree::OnObjectChanged;
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
			o2Scene.onObjectsChanged -= &DrawOrderTree::OnObjectsChanged;

			o2Scene.onAddedToScene -= &DrawOrderTree::OnObjectCreated;
			o2Scene.onRemovedFromScene -= &DrawOrderTree::OnObjectDestroing;

			auto updateTreeNode = &DrawOrderTree::OnObjectChanged;
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
		mEnableTogglesGroup = mmake<ToggleGroup>(ToggleGroup::Type::VerOneClick);
		mEnableTogglesGroup->onReleased = &DrawOrderTree::EnableObjectsGroupReleased;

		mLockTogglesGroup = mmake<ToggleGroup>(ToggleGroup::Type::VerOneClick);
		mLockTogglesGroup->onReleased = &DrawOrderTree::LockObjectsGroupReleased;

		auto objectNodeWidgetSample = mnode_cast<DrawOrderTreeNode>(mNodeWidgetSample);
		objectNodeWidgetSample->InitializeControls();
	}

	void DrawOrderTree::SetSelectedObjects(const Vector<Ref<SceneEditableObject>>& objects)
	{
		Vector<OrderTreeNode*> nodes;
		for (auto object : objects)
		{
			OrderTreeNode* node = nullptr;
			if (mObjectToNodeMap.TryGetValue(object, node))
				nodes.Add(node);
		}

		Tree::SetSelectedObjects(nodes.Convert<void*>([](auto x) { return x; }));
	}

	void DrawOrderTree::ScrollToAndHighlight(Ref<SceneEditableObject> object)
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
			Ref<OrderTreeNode> cameraNode = mmake<OrderTreeNode>();
			cameraNode->name = camera->GetName();
			cameraNode->type = OrderTreeNode::Type::Camera;
			cameraNode->object = camera;
			mObjectToNodeMap[cameraNode->object] = cameraNode;
			mRootOrderNodes.Add(cameraNode);

			for (auto layer : camera->drawLayers.GetLayers())
			{
				Ref<OrderTreeNode> layerNode = mmake<OrderTreeNode>();
				layerNode->name = layer->GetName();
				layerNode->type = OrderTreeNode::Type::Layer;
				layerNode->parent = Ref<OrderTreeNode>(*cameraNode);
				cameraNode->AddChild(layerNode);

				for (auto drawable : layer->GetDrawables())
					ProcessDrawableTreeNode(*layerNode, drawable);
			}
		}
	}

	void DrawOrderTree::ProcessDrawableTreeNode(Ref<OrderTreeNode> parent, ISceneDrawable* drawable)
	{
		auto drawableNode = mmake<Ref<OrderTreeNode>>();
		drawableNode = mmake<OrderTreeNode>();

		OrderTreeNode* parentPtr = *&parent;
		*drawableNode = *parentPtr;
		mRootOrderNodes.Add(dr);
	}
}replace raw pointers with Ref<>, except void* pointers. Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<> objects.

```cpp
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

struct OrderTreeNode {
    enum class Type {
        Actor,
        Root,
        Drawable,
        EndOfBatch
    };

    Type type;
    int batchIdx;
    std::string name;
    Ref<void> object;
    WeakRef<OrderTreeNode> parent;
    std::vector<Ref<OrderTreeNode>> children;
};

class DrawOrderTree {
public:
    void ProcessDrawableTreeNode(const Ref<OrderTreeNode>& parent, const Ref<Drawable>& drawable) {
        Ref<OrderTreeNode> drawableNode = mmake<OrderTreeNode>();

        drawableNode->inheritedOrderFromParent = drawable->IsDrawingDepthInheritedFromParent();
        drawableNode->customOrder = drawable->GetDrawingDepth();
        drawableNode->batchIdx = drawable->drawCallIdx;

        if (drawableNode->batchIdx < mStartBatchIdx)
            mStartBatchIdx = drawableNode->batchIdx;

        if (Ref<Actor> actor = DynamicCast<Actor>(drawable)) {
            drawableNode->name = actor->name;
            drawableNode->type = OrderTreeNode::Type::Actor;
            drawableNode->object = actor;

            mObjectToNodeMap[drawableNode->object] = drawableNode.get();

            CheckBatchEnd(drawableNode.get());
            parent->AddChild(drawableNode);
        }
        else if (Ref<SceneLayer::RootDrawablesContainer> root = DynamicCast<SceneLayer::RootDrawablesContainer>(drawable)) {
            drawableNode->type = OrderTreeNode::Type::Root;
            drawableNode->name = "Layer roots";

            CheckBatchEnd(drawableNode.get());
            parent->AddChild(drawableNode);
        }
        else {
            drawableNode->name = drawable->GetType().GetName();
            drawableNode->type = OrderTreeNode::Type::Drawable;

            CheckBatchEnd(drawableNode.get());
            parent->AddChild(drawableNode);
        }

        for (const auto& inherited : drawable->GetChildrenInheritedDepth())
            ProcessDrawableTreeNode(drawableNode, inherited);
    }

    void DrawOrderTree::CheckBatchEnd(const Ref<OrderTreeNode>& node) {
        static Ref<OrderTreeNode> prevDrawableNode = nullptr;

        if (prevDrawableNode && node->object != nullptr && node->batchIdx != prevDrawableNode->batchIdx) {
            Ref<OrderTreeNode> endNode = mmake<OrderTreeNode>();
            endNode->type = OrderTreeNode::Type::EndOfBatch;
            endNode->batchIdx = prevDrawableNode->batchIdx;

            endNode->name = "End of batch #" + std::to_string(endNode->batchIdx - mStartBatchIdx);

            auto parent = prevDrawableNode->GetParent();
            parent.lock()->AddChild(endNode);
        }

        if (node->object && node->batchIdx >= 0)
            prevDrawableNode = node;
    }

    void UpdateNodesStructure() {
        RebuildOrderTree();
        Tree::UpdateNodesStructure();
    }

    void UpdateVisibleNodes() {
        PushEditorScopeOnStack scope;
        Tree::UpdateVisibleNodes();
    }

    TreeNode* CreateTreeNodeWidget() {
        PushEditorScopeOnStack scope;
        return Tree::CreateTreeNodeWidget();
    }

    void* GetObjectParent(void* object) {
        if (!object)
            return nullptr;

        OrderTreeNode* treeNode = static_cast<OrderTreeNode*>(object);
        return treeNode->GetParent().lock().get();
    }

    std::vector<void*> GetObjectChilds(void* object) {
        OrderTreeNode* treeNode = static_cast<OrderTreeNode*>(object);
        if (treeNode) {
            std::vector<void*> children;
            for (const auto& child : treeNode->GetChildren())
                children.push_back(child.get());
            return children;
        }

        std::vector<void*> children;
        for (const auto& root : mRootOrderNodes)
            children.push_back(root.get());
        return children;
    }

    void FillNodeDataByObject(TreeNode* nodeWidget, void* object) {
        DrawOrderTreeNode* node = static_cast<DrawOrderTreeNode*>(nodeWidget);
        node->Setup(static_cast<OrderTreeNode*>(object));
        node->mLockToggle->SetToggleGroup(mLockTogglesGroup);
        node->mEnableToggle->SetToggleGroup(mEnableTogglesGroup);
    }

    void OnNodeDblClick(TreeNode* nodeWidget) {
        static_cast<DrawOrderTreeNode*>(nodeWidget)->EnableEditName();
    }

    void OnDraggedObjects(std::vector<void*> objects, void* newParent, void* prevObject) {
        SceneEditableObject* newParentEditableObject = static_cast<SceneEditableObject*>(newParent);
        SceneEditableObject* prevEditableObject = static_cast<SceneEditableObject*>(prevObject);
        std::vector<SceneEditableObject*> editableObjects;
        for (const auto& object : objects)
            editableObjects.push_back(static_cast<SceneEditableObject*>(object));

        auto action = std::make_unique<ReparentAction>(editableObjects);

        o2Scene.ReparentEditableObjects(editableObjects, newParentEditableObject, prevEditableObject);

        action->ObjectsReparented(mParent, prevEditableObject);
        o2EditorApplication.DoneAction(action.get());

        Tree::OnDraggedObjects(objects, newParent, prevObject);
    }

    void EnableObjectsGroupPressed(bool value) {}

    void EnableObjectsGroupReleased(bool value) {
        std::vector<SceneEditableObject*> objects;
        for (const auto& toggle : mEnableTogglesGroup->GetToggled())
            objects.push_back(static_cast<SceneEditableObject*>(static_cast<TreeNode*>(toggle->GetParent())->GetObject()));

        auto action = std::make_unique<EnableAction>(objects, value);
        o2EditorApplication.DoneAction(action.get());
    }

    void LockObjectsGroupPressed(bool value) {}

    void LockObjectsGroupReleased(bool value) {
        std::vector<SceneEditableObject*> objects;
        for (const auto& toggle : mLockTogglesGroup->GetToggled())
            objects.push_back(static_cast<SceneEditableObject*>(static_cast<TreeNode*>(toggle->GetParent())->GetObject()));

        auto action = std::make_unique<LockAction>(objects, value);
        o2EditorApplication.DoneAction(action.get());
    }

    void OnNodesSelectionChanged(const std::vector<void*>& objects) {
        std::vector<SceneEditableObject*> editableObjects;
        for (const auto& object : objects) {
            if (OrderTreeNode* treeNode = static_cast<OrderTreeNode*>(object); treeNode->object)
                editableObjects.push_back(static_cast<SceneEditableObject*>(treeNode->object.get()));
        }

        onObjectsSelectionChanged(editableObjects);
        Tree::OnNodesSelectionChanged(objects);
    }

    void* mParent;
    std::map<Ref<void>, OrderTreeNode*> mObjectToNodeMap;
    std::vector<Ref<OrderTreeNode>> mRootOrderNodes;
    int mStartBatchIdx;
    //
};

```#include <memory>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename ... Args>
Ref<T> mmake(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
struct DynamicCast
{
    template<typename U>
    static Ref<U> cast(const Ref<T>& ptr)
    {
        return std::dynamic_pointer_cast<U>(ptr);
    }
};

class ISelectableDragableObjectsGroup {};

class SceneEditableObject {};

template<typename T>
class Vector
{
public:
    void push_back(const T& value)
    {
        values.push_back(value);
    }
    
    std::vector<T> Cast() const
    {
        return values;
    }
    
private:
    std::vector<T> values;
};

template<typename T>
class DataValue {};

class Color4
{
public:
    static Color4 SomeColor(float batchIdx, float alpha)
    {
        return Color4();
    }
};

template<typename T>
class Drawable {};

class Text : public Drawable<Text>
{
public:
    Text(const std::string& name) {}
    void SetText(const std::string& text) {}
};

class Sprite : public Drawable<Sprite>
{
public:
    Sprite(const std::string& name) {}
    void SetImageAsset(const std::string& image) {}
    void SetImageName(const std::string& image) {}
    void SetTransparency(float alpha) {}
    void SetEnabled(bool enabled) {}
    void SetColor(const Color4& color) {}
    const Color4& GetColor() const {}
};

class Toggle
{
public:
    using Callback = void (*)();
    
    void SetEnabled(bool enabled) {}
    void SetValue(bool value) {}
    void SetTransparency(float alpha) {}
    void SetColor(const Color4& color) {}
    void onClick() {}
    
    Callback onClick;
};

class Button
{
public:
    using Callback = void (*)();
    
    void SetEnabled(bool enabled) {}
    void SetTransparency(float alpha) {}
    void SetColor(const Color4& color) {}
    void onClick() {}
    
    Callback onClick;
};

class EditBox
{
public:
    using Callback = void (*)();
    
    void SetEnabled(bool enabled) {}
    void SetTransparency(float alpha) {}
    void SetColor(const Color4& color) {}
    void onChangeCompleted() {}
    
    Callback onChangeCompleted;
};

class BaseTree
{
public:
    virtual void OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group) = 0;
    virtual void OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group) = 0;
    virtual void OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group) = 0;
    virtual void OnDropped(const Ref<ISelectableDragableObjectsGroup>& group) = 0;
    virtual void OnObjectCreated(const Ref<SceneEditableObject>& object) = 0;
    virtual void OnObjectDestroing(const Ref<SceneEditableObject>& object) = 0;
    virtual void OnObjectsChanged(const Vector<Ref<SceneEditableObject>>& objects) = 0;
    virtual void OnObjectChanged(const Ref<SceneEditableObject>& object) = 0;
};

class DrawOrderTree : public BaseTree
{
public:
    void OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group) override
    {
        Tree::OnDragEnter(group);
    }
    void OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group) override
    {
        Tree::OnDragExit(group);
    }
    void OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group) override
    {
        Tree::OnDraggedAbove(group);
    }
    void OnDropped(const Ref<ISelectableDragableObjectsGroup>& group) override
    {
        Tree::OnDropped(group);
    }
    void OnObjectCreated(const Ref<SceneEditableObject>& object) override
    {
        RebuildOrderTree();
        Tree::OnObjectCreated(object, object->GetEditableParent());
    }
    void OnObjectDestroing(const Ref<SceneEditableObject>& object) override
    {
        RebuildOrderTree();
        Tree::OnObjectRemoved(object);
    }
    void OnObjectsChanged(const Vector<Ref<SceneEditableObject>>& objects) override
    {
        RebuildOrderTree();
        Tree::OnObjectsChanged(objects.Cast());
    }
    void OnObjectChanged(const Ref<SceneEditableObject>& object) override
    {
        Tree::OnObjectsChanged({ object });
    }
    void RebuildOrderTree() {}
};

class TreeNode {};

class DrawOrderTreeNode : public TreeNode
{
public:
    DrawOrderTreeNode()
    {
        InitializeControls();
    }

    DrawOrderTreeNode(const DrawOrderTreeNode& other) : TreeNode(other)
    {
        InitializeControls();
    }
    
    DrawOrderTreeNode& operator=(const DrawOrderTreeNode& other)
    {
        TreeNode::operator=(other);
        InitializeControls();
        return *this;
    }
    
    void OnDeserialized(const DataValue<DrawOrderTreeNode>& node)
    {
        TreeNode::OnDeserialized(node);
        InitializeControls();
    }
    
    void InitializeControls()
    {
        mNameDrawable = mmake<Text>("name");
        mOrderDrawable = mmake<Text>("batch");
        mIconSprite = mmake<Sprite>("icon");
        mBackSprite = mmake<Sprite>("back");
        mLockToggle = mmake<Toggle>();
        mEnableToggle = mmake<Toggle>();
        mLinkBtn = mmake<Button>();
        mNameEditBox = mmake<EditBox>();
        mEditState = GetStateObject("edit");
        
        if (mLockToggle)
            mLockToggle->onClick = &DrawOrderTreeNode::OnLockClicked;
        
        if (mEnableToggle)
            mEnableToggle->onClick = &DrawOrderTreeNode::OnEnableCkicked;
        
        if (mNameEditBox)
            mNameEditBox->onChangeCompleted = &DrawOrderTreeNode::OnObjectNameChanged;
    }
    
    template<typename T>
    T* GetLayerDrawable(const std::string& name) {}
    
    template<typename T>
    T* GetChild(const std::string& name) {}
    
    void OnLockClicked()
    {
        
    }
    
    void OnEnableCkicked()
    {
        
    }
    
    void OnObjectNameChanged()
    {
        
    }

private:
    Ref<Text> mNameDrawable;
    Ref<Text> mOrderDrawable;
    Ref<Sprite> mIconSprite;
    Ref<Sprite> mBackSprite;
    Ref<Toggle> mLockToggle;
    Ref<Toggle> mEnableToggle;
    Ref<Button> mLinkBtn;
    Ref<EditBox> mNameEditBox;
    Ref<StateObject> mEditState;
    Ref<StateObject> mLinkBtnHalfHideState;
    Ref<StateObject> mLockToggleHalfHideState;
    Ref<StateObject> mLockToggleLockedState;
};

int main()
{
    return 0;
}#include <ref.h>

using namespace Zeus::Core;

namespace Editor
{
	class DrawOrderTreeNode : public BaseNode
	{
	public:
		using Ref = Ref<DrawOrderTreeNode>;
		using WeakRef = RefWeak<DrawOrderTreeNode>;

		DrawOrderTreeNode();

		const Ref<DrawOrderTreeNode>& GetParentNode() const { return mParentNode; }
		const Ref<DrawOrderTreeNode>& GetFirstChild() const { return mFirstChild; }
		const Ref<DrawOrderTreeNode>& GetNextSibling() const { return mNextSibling; }
		void AddChild(const Ref<DrawOrderTreeNode>& child);

		virtual void Initialize() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;

	protected:
		WeakRef<DrawOrderTreeNode> mParentNode;
		Ref<DrawOrderTreeNode> mFirstChild;
		Ref<DrawOrderTreeNode> mNextSibling;

		// Other member variables...

	};

	class DrawOrderTree
	{
	public:
		DrawOrderTree();

		Ref<DrawOrderTreeNode> GetRootNode() const { return mRootNode; }

		// Other member variables and functions...
	
	private:
		Ref<DrawOrderTreeNode> mRootNode;

		// Other member variables...

	};

}

void DrawOrderTreeNode::AddChild(const Ref<DrawOrderTreeNode>& child)
{
	if (!child)
		return;

	child->mParentNode = this;

	if (!mFirstChild)
	{
		mFirstChild = child;
	}
	else
	{
		auto sibling = mFirstChild;
		while (sibling->mNextSibling)
		{
			sibling = sibling->mNextSibling;
		}
		sibling->mNextSibling = child;
	}
}

void DrawOrderTree::Initialize()
{
	// Code here...
}

void DrawOrderTree::Update(float deltaTime)
{
	// Code here...
}

void DrawOrderTree::Render()
{
	// Code here...
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

DECLARE_CLASS(Editor::DrawOrderTree, Editor__DrawOrderTree);

DECLARE_CLASS(Editor::DrawOrderTreeNode, Editor__DrawOrderTreeNode);
// --- END META ---