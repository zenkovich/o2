#pragma once

#include "Utils/Singleton.h"
#include "Scene/UI/Widgets/Tree.h"

namespace o2
{
	class Window;
	class EditBox;
}

using namespace o2;

namespace Editor
{
	class AnimationPropertiesTree;

	class PropertiesListDlg : public Singleton<PropertiesListDlg>
	{
	public:
		void Show(Animation* animation, ActorRef actor);

	private:
		Window* mWindow = nullptr;
		EditBox* mFilter = nullptr;
		AnimationPropertiesTree* mPropertiesTree = nullptr;
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
		};

	public:
		AnimationPropertiesTree();
		AnimationPropertiesTree(const AnimationPropertiesTree& other);

		AnimationPropertiesTree& operator=(const AnimationPropertiesTree& other);

		void Initialize(ActorRef actor);

	private:
		NodeData mRoot; // Root properties data node

	private:
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

	private:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();
	};
}
