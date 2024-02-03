#pragma once

#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/Singleton.h"
#include "o2/O2.h"

namespace o2
{
    class Button;
    class EditBox;
    class Sprite;
    class Text;
    class Window;
}

namespace Editor
{
    class AnimationPropertiesTree;

    // ---------------------------------------------------------------------------------------
    // Animation properties list dialog. Shows properties tree with filter for specified actor
    // ---------------------------------------------------------------------------------------
    class PropertiesListDlg : public Singleton<PropertiesListDlg>
    {
    public:
        PropertiesListDlg();
        ~PropertiesListDlg();

        // Shows animation properties window for actor and animation
        static void Show(AnimationClip* animation, const Ref<Actor>& actor);

    private:
        Ref<o2::Window> mWindow;
        Ref<EditBox>    mFilter;

        Ref<AnimationPropertiesTree> mPropertiesTree;    
    private:
        // Initializes window and controls
        void InitializeWindow();
    };

    // ------------------------------------------------------------------------------------
    // Animation properties tree. Builds data tree by actor's properties, showing by filter
    // Can add or remove animation tracks
    // ------------------------------------------------------------------------------------
    class AnimationPropertiesTree : public Tree
    {
    public:
        struct NodeData
        {
            NodeData* parent = nullptr;
            Vector<NodeData*> children;

            String name;
            String path;

            const Type* type = nullptr;

            bool used = false;

            ~NodeData();

            void Clear();
            NodeData* AddChild(const String& name, const Type* type);
        };

    public:
        // Default constructor
        AnimationPropertiesTree();

        // Copy-constructor
        AnimationPropertiesTree(const AnimationPropertiesTree& other);

        // Copy operator
        AnimationPropertiesTree& operator=(const AnimationPropertiesTree& other);

        // Initializes properties
        void Initialize(AnimationClip* animation, const Ref<Actor>& actor);

        // Sets filter and refreshes tree
        void SetFilter(const WString& filter);

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(AnimationPropertiesTree);

    private:
        WString mFilterStr; // Filtering string

        AnimationClip* mAnimation = nullptr; // Looking animation
        Ref<Actor> mActor;                     // Looking actor

        NodeData mRoot;                 // Root properties data node
        Vector<IObject*> mPassedObject; // Tree processing passed objects

    private:
        // Initializes parameters tree node by object properties
        void InitializeTreeNode(NodeData* node, IObject* object);

        // Processes object base types and fields
        void ProcessObject(Ref<void> object, const ObjectType* type, NodeData* node);

        // Processes tree node property. Checks type
        void ProcessTreeNode(Ref<void> object, const Type* type, const String& name, NodeData* node);

        // initializes single property node
        void InitializePropertyNode(NodeData* node, const String& name, const Type* type);

        // Initializes sub tree for object
        void InitializeObjectTreeNode(const ObjectType* fieldObjectType, Ref<void> object, const String& name, NodeData* node);

        // Updates visible nodes (calculates range and initializes nodes), enables editor mode
        void UpdateVisibleNodes() override;

        // Gets tree node from pool or creates new, enables editor mode
        Ref<TreeNode> CreateTreeNodeWidget() override;

        // Returns object's parent
        Ref<void> GetObjectParent(Ref<void> object) override;

        // Returns object's children
        Vector<Ref<void>> GetObjectChilds(Ref<void> object) override;

        // Returns debugging string for object
        String GetObjectDebug(Ref<void> object) override;

        // Sets nodeWidget data by object
        void FillNodeDataByObject(Ref<TreeNode> nodeWidget, Ref<void> object) override;

        // Called when tree node was double clicked
        void OnNodeDblClick(Ref<TreeNode> nodeWidget) override;

        // Called when list of selected objects was changed
        void OnNodesSelectionChanged(Vector<Ref<void>> objects) override;

        friend class AnimationPropertiesTreeNode;
    };

    // -------------------------------------------------------------------------
    // Animation properties tree node. Shows icon, name and add or remove button
    // -------------------------------------------------------------------------
    class AnimationPropertiesTreeNode : public TreeNode
    {
    public:
        // Default constructor
        AnimationPropertiesTreeNode();

        // Copy-constructor
        AnimationPropertiesTreeNode(const AnimationPropertiesTreeNode& other);

        // Copy operator
        AnimationPropertiesTreeNode& operator=(const AnimationPropertiesTreeNode& other);

        // Initializes node by data
        void Setup(Ref<AnimationPropertiesTree::NodeData> data, Ref<AnimationPropertiesTree> tree);

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(AnimationPropertiesTreeNode);

    private:
        Ref<Text> mName;         // Name of property
        Ref<Sprite> mIcon;       // Property icon. Used only for finite properties
        Ref<Button> mAddButton;  // Add button

        // Handles button click
        void HandleAddButtonClicked(Object* sender, const EventArgs& e);
    };

    // ----------------------------------------------------------------------------
    // Casts Ref to type*
    // ----------------------------------------------------------------------------
    template <typename T>
    T* DynamicCast(const Ref<void>& obj)
    {
        return dynamic_cast<T*>(obj.Get());
    }

    // ----------------------------------------------------------------------------
    // Creates Ref
    // ----------------------------------------------------------------------------
    template <typename T, typename... Args>
    Ref<T> mmake(Args&&... args)
    {
        return MakeRef<T>(std::forward<Args>(args)...);
    }
}// Include necessary headers for Ref<> and WeakRef<>
#include <memory>

// Create Ref<> and WeakRef<> aliases
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

// Add button, it is enabled when animation track isn't added to animation, adds this value to animation
Button* mRemoveButton;

// Remove button, it is enabled when animation track is added to animation, removes this value to animation
Button* mAddButton;

// Data node pointer
AnimationPropertiesTree::NodeData* mData = nullptr;

// Owner tree
AnimationPropertiesTree* mTree = nullptr;

private:
// Called on deserialization, initializes controls
void OnDeserialized(const DataValue& node) override;

// initializes controls and widgets
void InitializeControls();

friend class AnimationPropertiesTree;
};

// --- META ---

CLASS_BASES_META(Editor::AnimationPropertiesTree)
{
    BASE_CLASS(o2::Tree);
}
END_META;

CLASS_FIELDS_META(Editor::AnimationPropertiesTree)
{
    FIELD().PRIVATE().NAME(mFilterStr),
    FIELD().PRIVATE().NAME(mAnimation),
    FIELD().PRIVATE().NAME(mActor),
    FIELD().PRIVATE().NAME(mRoot),
    FIELD().PRIVATE().NAME(mPassedObject)
}
END_META;

CLASS_METHODS_META(Editor::AnimationPropertiesTree)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(),
    FUNCTION().PUBLIC().CONSTRUCTOR(const AnimationPropertiesTree&),
    FUNCTION().PUBLIC().SIGNATURE(void, Initialize, Ref<AnimationClip>, Ref<Actor>),
    FUNCTION().PUBLIC().SIGNATURE(void, SetFilter, const WString&),
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory),
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeTreeNode, Ref<AnimationPropertiesTree::NodeData>, Ref<IObject>),
    FUNCTION().PRIVATE().SIGNATURE(void, ProcessObject, void*, const ObjectType*, Ref<AnimationPropertiesTree::NodeData>),
    FUNCTION().PRIVATE().SIGNATURE(void, ProcessTreeNode, void*, const Type*, const String&, Ref<AnimationPropertiesTree::NodeData>),
    FUNCTION().PRIVATE().SIGNATURE(void, InitializePropertyNode, Ref<AnimationPropertiesTree::NodeData>, const String&, const Type*),
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeObjectTreeNode, const ObjectType*, void*, const String&, Ref<AnimationPropertiesTree::NodeData>),
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateVisibleNodes),
    FUNCTION().PRIVATE().SIGNATURE(TreeNode*, CreateTreeNodeWidget),
    FUNCTION().PRIVATE().SIGNATURE(void*, GetObjectParent, void*),
    FUNCTION().PRIVATE().SIGNATURE(Vector<void*>, GetObjectChilds, void*),
    FUNCTION().PRIVATE().SIGNATURE(String, GetObjectDebug, void*),
    FUNCTION().PRIVATE().SIGNATURE(void, FillNodeDataByObject, Ref<TreeNode>, void*),
    FUNCTION().PRIVATE().SIGNATURE(void, OnNodeDblClick, Ref<TreeNode>),
    FUNCTION().PRIVATE().SIGNATURE(void, OnNodesSelectionChanged, Vector<void*>)
}
END_META;

CLASS_BASES_META(Editor::AnimationPropertiesTreeNode)
{
    BASE_CLASS(o2::TreeNode);
}
END_META;

CLASS_FIELDS_META(Editor::AnimationPropertiesTreeNode)
{
    FIELD().PRIVATE().NAME(mName),
    FIELD().PRIVATE().NAME(mIcon),
    FIELD().PRIVATE().NAME(mAddButton),
    FIELD().PRIVATE().NAME(mRemoveButton),
    FIELD().PRIVATE().NAME(mData),
    FIELD().PRIVATE().NAME(mTree)
}
END_META;

CLASS_METHODS_META(Editor::AnimationPropertiesTreeNode)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(),
    FUNCTION().PUBLIC().CONSTRUCTOR(const AnimationPropertiesTreeNode&),
    FUNCTION().PUBLIC().SIGNATURE(void, Setup, Ref<AnimationPropertiesTree::NodeData>, Ref<AnimationPropertiesTree>),
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory),
    FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&),
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls)
}
END_META;
// --- END META ---