#pragma once

#include "Utils/Singleton.h"
#include "Scene/UI/Widgets/Tree.h"

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

	class PropertiesListDlg : public Singleton<PropertiesListDlg>
	{
	public:
		PropertiesListDlg();
		~PropertiesListDlg();

		// Shows animation properties window for actor and animation
		void Show(Animation* animation, ActorRef actor);

	private:
		Window* mWindow = nullptr;
		EditBox* mFilter = nullptr;
		AnimationPropertiesTree* mPropertiesTree = nullptr;

	private:
		// Initializes window and controls
		void InitializeWindow();
	};

	class AnimationPropertiesTree : public Tree
	{
	public:
		struct NodeData
		{
			NodeData* parent = nullptr;
			Vector<NodeData*> children;

			String name;
			String path;
			const Type* type;
			bool used = false;

		public:
			~NodeData();

			void Clear();
			NodeData* AddChild(const String& name, const Type* type);
		};

	public:
		AnimationPropertiesTree();
		AnimationPropertiesTree(const AnimationPropertiesTree& other);

		AnimationPropertiesTree& operator=(const AnimationPropertiesTree& other);

		// Initializes properties
		void Initialize(Animation* animation, ActorRef actor);

		SERIALIZABLE(AnimationPropertiesTree);

	private:
		NodeData mRoot; // Root properties data node

	private:
		// Initializes parameters tree node by object properties
		void InitializeTreeNode(NodeData* node, IObject* object);

		// Initializes children node for object property
		void InitializeSubTreeNode(FieldInfo* fieldInfo, const ObjectType* type, IObject* object, NodeData* node);

		// Updates visible nodes (calculates range and initializes nodes), enables editor mode
		void UpdateVisibleNodes() override;

		// Gets tree node from pool or creates new, enables editor mode
		TreeNode* CreateTreeNodeWidget() override;

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object) override;

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object) override;

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object) override;

		// It is called when tree node was double clicked
		void OnNodeDblClick(TreeNode* nodeWidget) override;

		// It is called when list of selected objects was changed
		void OnNodesSelectionChanged(UnknownPtrsVec objects) override;
	};

	class AnimationPropertiesTreeNode : public TreeNode
	{
	public:
		AnimationPropertiesTreeNode();
		AnimationPropertiesTreeNode(const AnimationPropertiesTreeNode& other);

		AnimationPropertiesTreeNode& operator=(const AnimationPropertiesTreeNode& other);

		// Initializes node by data
		void Setup(const AnimationPropertiesTree::NodeData& data);

		SERIALIZABLE(AnimationPropertiesTreeNode);

	private:
		Text*   mName;
		Sprite* mIcon;
		Button* mAddButton;
		Button* mRemoveButton;

	private:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();
	};
}
