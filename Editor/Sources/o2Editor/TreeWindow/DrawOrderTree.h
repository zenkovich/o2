#pragma once

#include "o2/Scene/UI/Widgets/Tree.h"

namespace o2
{
    class Actor;
    class SceneEditableObject;
    class Button;
    class EditBox;
    class Toggle;
    class ToggleGroup;
}

using namespace o2;

namespace Editor
{
    class ActorProperty;
    class ComponentProperty;

    // -----------------
    // Scene tree widget
    // -----------------
    class DrawOrderTree : public Tree
    {
    public:
        Function<void(Vector<Ref<SceneEditableObject>>)> onObjectsSelectionChanged; // Callback on objects selection changed

    public:
        // Default constructor
        DrawOrderTree();

        // Copy-constructor
        DrawOrderTree(const DrawOrderTree& other);

        // Destructor
        ~DrawOrderTree();

        // Copy-operator
        DrawOrderTree& operator=(const DrawOrderTree& other);

        // Sets selected objects
        void SetSelectedObjects(const Vector<Ref<SceneEditableObject>>& objects);

        // Scrolls view to object and highlights
        void ScrollToAndHighlight(const Ref<SceneEditableObject>& object);

        // Rebuilds order tree mRootOrderNode
        void RebuildOrderTree();

        // Attaches to scene events
        void AttachToSceneEvents();

        // Deattaches from scene events
        void DetachFromSceneEvents();

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(DrawOrderTree);

    public:
        struct OrderTreeNode : public ITreeNode<OrderTreeNode>, public RefCounterable
        {
            enum class Type { Camera, Layer, Root, Drawable, Actor, EndOfBatch };

            Type   type;
            String name;

            Ref<SceneEditableObject> object;
            bool                     objectEnabled = false;

            int   batchIdx = -1;

            bool  inheritedOrderFromParent = false;
            float customOrder = 0.0f;
        };

    protected:
        Vector<Ref<OrderTreeNode>> mRootOrderNodes; // Root nodes for draw order hierarchy data

        int mStartBatchIdx = 0; // Index of first batch

        Map<Ref<SceneEditableObject>, OrderTreeNode*> mObjectToNodeMap; // Map from object to node

        Ref<ToggleGroup>   mEnableTogglesGroup; // Enable objects toggles group
        Ref<ToggleGroup>   mLockTogglesGroup;   // Lock objects toggles group
        bool               mAttachedToSceneEvents; // Is tree attached to scene events
        
        Ref<ActorProperty>     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
        Ref<ComponentProperty> mDragComponentPropertyField; // Component property field under cursor when dragging actor

        bool mWatchEditor = false;

    protected:
        // Initializes widget logic
        void Initialize();

        // Processes drawable node recursively
        void ProcessDrawableTreeNode(OrderTreeNode* parent, ISceneDrawable* drawable);

        // Checks that previous object was changed batch
        void CheckBatchEnd(OrderTreeNode* node);

        // Updates draw order hierarchy, Updates root nodes and their childs if need
        void UpdateNodesStructure() override;

        // Updates visible nodes (calculates range and initializes nodes), enables editor mode
        void UpdateVisibleNodes() override;

        // Gets tree node from pool or creates new, enables editor mode
        TreeNode* CreateTreeNodeWidget() override;

        // Returns object's parent
        void* GetObjectParent(void* object) override;

        // Returns object's children
        Vector<void*> GetObjectChilds(void* object) override;

        // Sets nodeWidget data by object
        void FillNodeDataByObject(TreeNode* nodeWidget, void* object) override;

        // Called when tree node was double clicked
        void OnNodeDblClick(TreeNode* nodeWidget) override;

        // Called when objects was dragged in new parent in position next of prevObject
        void OnDraggedObjects(Vector<void*> objects, void* newParent, void* prevObject) override;

        // Called when object was created
        void OnObjectCreated(const Ref<SceneEditableObject>& object);

        // Called when object was destroyed
        void OnObjectDestroing(const Ref<SceneEditableObject>& object);

        // Called when some objects were changed
        void OnObjectsChanged(const Vector<Ref<SceneEditableObject>>& objects);

        // Called when object was changed
        void OnObjectChanged(const Ref<SceneEditableObject>& object);

        // Called when enable objects toggle group pressed
        void EnableObjectsGroupPressed(bool value);

        // Called when enable objects toggle group released
        void EnableObjectsGroupReleased(bool value);

        // Called when lock objects toggle group pressed
        void LockObjectsGroupPressed(bool value);

        // Called when lock objects toggle group released
        void LockObjectsGroupReleased(bool value);

        // Called when list of selected objects was changed
        void OnNodesSelectionChanged(Vector<void*> objects) override;

        // Called when some drag listeners was entered to this area
        void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

        // Called when some drag listeners was exited from this area
        void OnDragExit(ISelectableDragableObjectsGroup* group) override;

        // Called when some drag listeners was dragged above this area
        void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

        // Called when some selectable listeners was dropped to this
        void OnDropped(ISelereplace raw pointers with Ref<>, except void* pointers.#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
inline Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename Derived, typename Base>
inline Ref<Derived> DynamicCast(const Ref<Base>& ref)
{
    return std::dynamic_pointer_cast<Derived>(ref);
}

void LockObjectsGroupReleased(bool)
{
}

void OnNodesSelectionChanged(const Vector<void*>&)
{
}

void OnDragEnter(ISelectableDragableObjectsGroup*)
{
}

void OnDragExit(ISelectableDragableObjectsGroup*)
{
}

void OnDraggedAbove(ISelectableDragableObjectsGroup*)
{
}

void OnDropped(ISelectableDragableObjectsGroup*)
{
}

class Editor::DrawOrderTreeNode : public o2::TreeNode
{
public:
    Ref<DrawOrderTree::OrderTreeNode> mTarget;
    Ref<ISelectableDragableObjectsGroup> mLockToggle;
    Ref<bool> mLockToggleLockedState;
    Ref<bool> mLockToggleHalfHideState;
    Ref<bool> mEnableToggle;
    Ref<ISelectableDragableObjectsGroup> mLinkBtn;
    Ref<bool> mLinkBtnHalfHideState;
    Ref<Drawable> mNameDrawable;
    Ref<Drawable> mOrderDrawable;
    Ref<Sprite> mBackSprite;
    Ref<Sprite> mIconSprite;
    Ref<EditBox> mNameEditBox;
    Ref<size_t> mEditState;

    DrawOrderTreeNode() {}

    DrawOrderTreeNode(const DrawOrderTreeNode& other) {}

    void Setup(DrawOrderTree::OrderTreeNode*) {}

    void EnableEditName() {}

    static String GetCreateMenuCategory() {}

protected:
    void OnDeserialized(const DataValue&) {}

    void InitializeControls() {}

    void OnLockClicked() {}

    void OnEnableCkicked() {}

    void OnObjectNameChanged(const WString&) {}
};