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
        Function<void(const Vector<Ref<SceneEditableObject>>&)> onObjectsSelectionChanged; // Callback on objects selection changed

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
        void DeattachFromSceneEvents();

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

            int    batchIdx = -1;

            bool   inheritedOrderFromParent = false;
            float  customOrder = 0.0f;
        };

    protected:
        Vector<Ref<OrderTreeNode>> mRootOrderNodes; // Root nodes for draw order hierarchy data

        int mStartBatchIdx = 0; // Index of first batch

        Map<Ref<SceneEditableObject>, Ref<OrderTreeNode>> mObjectToNodeMap; // Map from object to node

        Ref<ToggleGroup> mEnableTogglesGroup;    // Enable objects toggles group
        Ref<ToggleGroup> mLockTogglesGroup;      // Lock objects toggles group
        bool             mAttachedToSceneEvents; // Is tree attached to scene events

        Ref<ActorProperty>     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
        Ref<ComponentProperty> mDragComponentPropertyField; // Component property field under cursor when dragging actor

        bool mWatchEditor = false;

    protected:
        // Initializes widget logic
        void Initialize();

        // Processes drawable node recursively
        void ProcessDrawableTreeNode(const Ref<OrderTreeNode>& parent, const Ref<ISceneDrawable>& drawable);

        // Checks that previous object was changed batch
        void CheckBatchEnd(const Ref<OrderTreeNode>& node);

        // Updates draw order hierarchy, Updates root nodes and their childs if need
        void UpdateNodesStructure() override;

        // Updates visible nodes (calculates range and initializes nodes), enables editor mode
        void UpdateVisibleNodes() override;

        // Gets tree node from pool or creates new, enables editor mode
        Ref<TreeNode> CreateTreeNodeWidget() override;

        // Returns object's parent
        void* GetObjectParent(void* object) override;

        // Returns object's children
        Vector<void*> GetObjectChilds(void* object) override;

        // Sets nodeWidget data by object
        void FillNodeDataByObject(const Ref<TreeNode>& nodeWidget, void* object) override;

        // Called when tree node was double clicked
        void OnNodeDblClick(const Ref<TreeNode>& nodeWidget) override;

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
        void OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group) override;

        // Called when some drag listeners was exited from this area
        void OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group) override;

        // Called when some drag listeners was dragged above this area
        void OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group) override;

        // Called when some selectable listeners was dropped to this
        void OnDropped(const Ref<ISelectableDragableObjectsGroup>& group) override;

        friend class DrawOrderTreeNode;
    };

    // ---------------
    // Scene tree node
    // ---------------
    class DrawOrderTreeNode : public TreeNode
    {
    public:
        // Default constructor
        DrawOrderTreeNode();

        // Copy-constructor
        DrawOrderTreeNode(const DrawOrderTreeNode& other);

        // Copy operator
        DrawOrderTreeNode& operator=(const DrawOrderTreeNode& other);

        // Sets object and updates content
        void Setup(const Ref<DrawOrderTree::OrderTreeNode>& object);

        // Enables edit name edit box
        void EnableEditName();

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(DrawOrderTreeNode);

    protected:
        Ref<DrawOrderTree::OrderTreeNode> mTarget; // Target object

        Ref<Toggle>      mLockToggle;              // Lock toggle
        Ref<WidgetState> mLockToggleLockedState;   // Lock toggle locked state
        Ref<WidgetState> mLockToggleHalfHideState; // Lock toggle half hide state

        Ref<Toggle> mEnableToggle; // Enable toggle

        Ref<Button>      mLinkBtn;              // View link button
        Ref<WidgetState> mLinkBtnHalfHideState; // View link button half hide state

        Ref<Text>        mNameDrawable;  // Object name drawable
        Ref<Text>        mOrderDrawable; // Object order value drawable
        Ref<Sprite>      mBackSprite;    // Object back drawable
        Ref<Sprite>      mIconSprite;    // Object icon drawable
        Ref<EditBox>     mNameEditBox;   // Object's name edit box
        Ref<WidgetState> mEditState;     // Object's name edit state

    protected:
        // Called on deserialization, initializes controls
        void OnDeserialized(const DataValue& node) override;

        // initializes controls and widgets
        void InitializeControls();

        // Called when lock toggle was clicked and changes target object's lock state
        void OnLockClicked();

        // Called when enable toggle was clicked and changes target object's enable state
        void OnEnableCkicked();

        // Called when object name edit box changed
        void OnObjectNameChanged(const WString& text);

        friend class DrawOrderTree;
    };
}
// --- META ---

PRE_ENUM_META(Editor::DrawOrderTree::OrderTreeNode::Type);

CLASS_BASES_META(Editor::DrawOrderTree)
{
    BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::DrawOrderTree)
{
    FIELD().PUBLIC().NAME(onObjectsSelectionChanged);
    FIELD().PROTECTED().NAME(mRootOrderNodes);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mStartBatchIdx);
    FIELD().PROTECTED().NAME(mObjectToNodeMap);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEnableTogglesGroup);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockTogglesGroup);
    FIELD().PROTECTED().NAME(mAttachedToSceneEvents);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDragActorPropertyField);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDragComponentPropertyField);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWatchEditor);
}
END_META;
CLASS_METHODS_META(Editor::DrawOrderTree)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const DrawOrderTree&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSelectedObjects, const Vector<SceneEditableObject*>&);
    FUNCTION().PUBLIC().SIGNATURE(void, ScrollToAndHighlight, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildOrderTree);
    FUNCTION().PUBLIC().SIGNATURE(void, AttachToSceneEvents);
    FUNCTION().PUBLIC().SIGNATURE(void, DeattachFromSceneEvents);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, Initialize);
    FUNCTION().PROTECTED().SIGNATURE(void, ProcessDrawableTreeNode, OrderTreeNode*, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, CheckBatchEnd, OrderTreeNode*);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateNodesStructure);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateVisibleNodes);
    FUNCTION().PROTECTED().SIGNATURE(TreeNode*, CreateTreeNodeWidget);
    FUNCTION().PROTECTED().SIGNATURE(void*, GetObjectParent, void*);
    FUNCTION().PROTECTED().SIGNATURE(Vector<void*>, GetObjectChilds, void*);
    FUNCTION().PROTECTED().SIGNATURE(void, FillNodeDataByObject, TreeNode*, void*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnNodeDblClick, TreeNode*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDraggedObjects, Vector<void*>, void*, void*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnObjectCreated, SceneEditableObject*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnObjectDestroing, SceneEditableObject*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnObjectsChanged, const Vector<SceneEditableObject*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnObjectChanged, SceneEditableObject*);
    FUNCTION().PROTECTED().SIGNATURE(void, EnableObjectsGroupPressed, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, EnableObjectsGroupReleased, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, LockObjectsGroupPressed, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, LockObjectsGroupReleased, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnNodesSelectionChanged, Vector<void*>);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnter, ISelectableDragableObjectsGroup*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragExit, ISelectableDragableObjectsGroup*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, ISelectableDragableObjectsGroup*);
}
END_META;

CLASS_BASES_META(Editor::DrawOrderTreeNode)
{
    BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::DrawOrderTreeNode)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTarget);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockToggleLockedState);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockToggleHalfHideState);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEnableToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLinkBtn);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLinkBtnHalfHideState);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mNameDrawable);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOrderDrawable);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBackSprite);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mIconSprite);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mNameEditBox);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditState);
}
END_META;
CLASS_METHODS_META(Editor::DrawOrderTreeNode)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const DrawOrderTreeNode&);
    FUNCTION().PUBLIC().SIGNATURE(void, Setup, DrawOrderTree::OrderTreeNode*);
    FUNCTION().PUBLIC().SIGNATURE(void, EnableEditName);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnLockClicked);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnableCkicked);
    FUNCTION().PROTECTED().SIGNATURE(void, OnObjectNameChanged, const WString&);
}
END_META;
// --- END META ---
