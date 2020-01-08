#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Tree.h"

using namespace o2;

namespace o2
{
	class EditBox;
	class Button;
}

namespace Editor
{
	class ActorViewer;
	class ComponentsTree;

	// --------------------------------------------------------------------------------
	// Add component to actor panel. Shows filter input, add button and components tree
	// --------------------------------------------------------------------------------
	class AddComponentPanel : public Widget, public CursorEventsArea, public KeyboardEventsListener
	{
	public:
		// Default constructor
		AddComponentPanel();

		// Constructor
		AddComponentPanel(ActorViewer* viewer);

		// Draws widget, calls OnDrawn for CursorEventsListener
		void Draw();

		// Returns filter widget
		EditBox* GetFilter() const;

		// Returns tree widget
		ComponentsTree* GetTree() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		SERIALIZABLE(AddComponentPanel);

	private:
		ActorViewer* mViewer = nullptr; // Owner actors viewer

		EditBox*        mFilterBox = nullptr; // Components names filter edit box, updates list of component when edit
		Button*         mAddButton = nullptr; // Add button
		ComponentsTree* mTree = nullptr;      // Components tree

	private:
		// It is called when add button pressed. Adds selected component to target actors from viewer
		void OnAddPressed();

		// It is called when tree node was double clicked
		void OnNodeDblClick(TreeNode* nodeWidget);

		// It is called when key was released. When returns has pressed, component is creating
		void OnKeyReleased(const Input::Key& key);
	};
	
	// ---------------------------------------------------------------------
	// Components tree. Builds data tree by available components with filter
	// ---------------------------------------------------------------------
	class ComponentsTree : public Tree
	{
	public:
		struct NodeData
		{
			NodeData*         parent = nullptr;
			Vector<NodeData*> children;

			String name;
			String path;
			String icon = "ui/UI4_component_icon.png";

			const Type* type = nullptr;

		public:
			~NodeData();

			void Clear();
			NodeData* AddChild(const String& name, const Type* type);
		};

	public:
		// Default constructor
		ComponentsTree();

		// Copy-constructor
		ComponentsTree(const ComponentsTree& other);

		// Copy operator
		ComponentsTree& operator=(const ComponentsTree& other);

		// Refreshes components list
		void Refresh();

		// Sets filter and refreshes tree
		void SetFilter(const WString& filter);

		SERIALIZABLE(ComponentsTree);

	private:
		WString  mFilterStr; // Filtering string
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

		friend class ComponentsTreeNode;
	};

	// --------------------------------------------------------------------------
	// Components tree node. Can bea  group of components or the component itself
	// --------------------------------------------------------------------------
	class ComponentsTreeNode : public TreeNode
	{
	public:
		ComponentsTree::NodeData* data = nullptr;

	public:
		// Default constructor
		ComponentsTreeNode();

		// Copy-constructor
		ComponentsTreeNode(const ComponentsTreeNode& other);

		// Copy operator
		ComponentsTreeNode& operator=(const ComponentsTreeNode& other);

		// Initializes node by data
		void Setup(ComponentsTree::NodeData* data, ComponentsTree* tree);

		SERIALIZABLE(ComponentsTreeNode);

	private:
		Text*   mName;
		Sprite* mIcon;

		ComponentsTree* mTree = nullptr;

	private:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();

		friend class ComponentsTree;
	};
}

CLASS_BASES_META(Editor::AddComponentPanel)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::CursorEventsArea);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::AddComponentPanel)
{
	PRIVATE_FIELD(mViewer);
	PRIVATE_FIELD(mFilterBox);
	PRIVATE_FIELD(mAddButton);
	PRIVATE_FIELD(mTree);
}
END_META;
CLASS_METHODS_META(Editor::AddComponentPanel)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(EditBox*, GetFilter);
	PUBLIC_FUNCTION(ComponentsTree*, GetTree);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsInputTransparent);
	PRIVATE_FUNCTION(void, OnAddPressed);
	PRIVATE_FUNCTION(void, OnNodeDblClick, TreeNode*);
	PRIVATE_FUNCTION(void, OnKeyReleased, const Input::Key&);
}
END_META;

CLASS_BASES_META(Editor::ComponentsTree)
{
	BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::ComponentsTree)
{
	PRIVATE_FIELD(mFilterStr);
	PRIVATE_FIELD(mRoot);
}
END_META;
CLASS_METHODS_META(Editor::ComponentsTree)
{

	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetFilter, const WString&);
	PRIVATE_FUNCTION(void, UpdateVisibleNodes);
	PRIVATE_FUNCTION(TreeNode*, CreateTreeNodeWidget);
	PRIVATE_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PRIVATE_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PRIVATE_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PRIVATE_FUNCTION(void, FillNodeDataByObject, TreeNode*, UnknownPtr);
}
END_META;

CLASS_BASES_META(Editor::ComponentsTreeNode)
{
	BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::ComponentsTreeNode)
{
	PUBLIC_FIELD(data);
	PRIVATE_FIELD(mName);
	PRIVATE_FIELD(mIcon);
	PRIVATE_FIELD(mTree);
}
END_META;
CLASS_METHODS_META(Editor::ComponentsTreeNode)
{

	PUBLIC_FUNCTION(void, Setup, ComponentsTree::NodeData*, ComponentsTree*);
	PRIVATE_FUNCTION(void, CopyData, const Actor&);
	PRIVATE_FUNCTION(void, OnDeserialized, const DataNode&);
	PRIVATE_FUNCTION(void, InitializeControls);
}
END_META;
