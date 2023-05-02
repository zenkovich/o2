#pragma once

#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/Singleton.h"

namespace o2
{
	class Button;
	class EditBox;
	class Sprite;
	class Text;
	class Window;
}

using namespace o2;

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
		static void Show(AnimationClip* animation, ActorRef actor);

	private:
		o2::Window* mWindow = nullptr;
		EditBox*    mFilter = nullptr;

		AnimationPropertiesTree* mPropertiesTree = nullptr;

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
			NodeData*         parent = nullptr;
			Vector<NodeData*> children;

			String name;
			String path;

			const Type* type = nullptr;

			bool used = false;

		public:
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
		void Initialize(AnimationClip* animation, ActorRef actor);

		// Sets filter and refreshes tree
		void SetFilter(const WString& filter);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AnimationPropertiesTree);

	private:
		WString mFilterStr; // Filtering string

		AnimationClip* mAnimation = nullptr; // Looking animation
		ActorRef       mActor;               // Looking actor

		NodeData         mRoot;         // Root properties data node
		Vector<IObject*> mPassedObject; // Tree processing passed objects

	private:
		// Initializes parameters tree node by object properties
		void InitializeTreeNode(NodeData* node, IObject* object);

		// Processes object base types and fields
		void ProcessObject(void* object, const ObjectType* type, NodeData* node);

		// Processes tree node property. Checks type
		void ProcessTreeNode(void* object, const Type* type, const String& name, NodeData* node);

		// initializes single property node
		void InitializePropertyNode(NodeData* node, const String& name, const Type* type);

		// Initializes sub tree for object
		void InitializeObjectTreeNode(const ObjectType* fieldObjectType, void* object, const String& name, NodeData* node);

		// Updates visible nodes (calculates range and initializes nodes), enables editor mode
		void UpdateVisibleNodes() override;

		// Gets tree node from pool or creates new, enables editor mode
		TreeNode* CreateTreeNodeWidget() override;

		// Returns object's parent
		void* GetObjectParent(void* object) override;

		// Returns object's children
		Vector<void*> GetObjectChilds(void* object) override;

		// Returns debugging string for object
		String GetObjectDebug(void* object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, void* object) override;

		// Called when tree node was double clicked
		void OnNodeDblClick(TreeNode* nodeWidget) override;

		// Called when list of selected objects was changed
		void OnNodesSelectionChanged(Vector<void*> objects) override;

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
		void Setup(AnimationPropertiesTree::NodeData* data, AnimationPropertiesTree* tree);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AnimationPropertiesTreeNode);

	private:
		Text*   mName;         // Name of property
		Sprite* mIcon;         // Property icon. Used only for finite properties
		Button* mAddButton;    // Add button, it is enabled when animation track isn't added to animation, adds this value to animation
		Button* mRemoveButton; // Remove button, it is enabled when animation track is added to animation, removes this value to animation

		AnimationPropertiesTree::NodeData* mData = nullptr; // Data node pointer

		AnimationPropertiesTree* mTree = nullptr; // Owner tree

	private:
		// Called on deserialization, initializes controls
		void OnDeserialized(const DataValue& node) override;

		// initializes controls and widgets
		void InitializeControls();

		friend class AnimationPropertiesTree;
	};
}

CLASS_BASES_META(Editor::AnimationPropertiesTree)
{
	BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationPropertiesTree)
{
	FIELD().PRIVATE().NAME(mFilterStr);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAnimation);
	FIELD().PRIVATE().NAME(mActor);
	FIELD().PRIVATE().NAME(mRoot);
	FIELD().PRIVATE().NAME(mPassedObject);
}
END_META;
CLASS_METHODS_META(Editor::AnimationPropertiesTree)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const AnimationPropertiesTree&);
	FUNCTION().PUBLIC().SIGNATURE(void, Initialize, AnimationClip*, ActorRef);
	FUNCTION().PUBLIC().SIGNATURE(void, SetFilter, const WString&);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeTreeNode, NodeData*, IObject*);
	FUNCTION().PRIVATE().SIGNATURE(void, ProcessObject, void*, const ObjectType*, NodeData*);
	FUNCTION().PRIVATE().SIGNATURE(void, ProcessTreeNode, void*, const Type*, const String&, NodeData*);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializePropertyNode, NodeData*, const String&, const Type*);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeObjectTreeNode, const ObjectType*, void*, const String&, NodeData*);
	FUNCTION().PRIVATE().SIGNATURE(void, UpdateVisibleNodes);
	FUNCTION().PRIVATE().SIGNATURE(TreeNode*, CreateTreeNodeWidget);
	FUNCTION().PRIVATE().SIGNATURE(void*, GetObjectParent, void*);
	FUNCTION().PRIVATE().SIGNATURE(Vector<void*>, GetObjectChilds, void*);
	FUNCTION().PRIVATE().SIGNATURE(String, GetObjectDebug, void*);
	FUNCTION().PRIVATE().SIGNATURE(void, FillNodeDataByObject, TreeNode*, void*);
	FUNCTION().PRIVATE().SIGNATURE(void, OnNodeDblClick, TreeNode*);
	FUNCTION().PRIVATE().SIGNATURE(void, OnNodesSelectionChanged, Vector<void*>);
}
END_META;

CLASS_BASES_META(Editor::AnimationPropertiesTreeNode)
{
	BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationPropertiesTreeNode)
{
	FIELD().PRIVATE().NAME(mName);
	FIELD().PRIVATE().NAME(mIcon);
	FIELD().PRIVATE().NAME(mAddButton);
	FIELD().PRIVATE().NAME(mRemoveButton);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mData);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mTree);
}
END_META;
CLASS_METHODS_META(Editor::AnimationPropertiesTreeNode)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const AnimationPropertiesTreeNode&);
	FUNCTION().PUBLIC().SIGNATURE(void, Setup, AnimationPropertiesTree::NodeData*, AnimationPropertiesTree*);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
}
END_META;
