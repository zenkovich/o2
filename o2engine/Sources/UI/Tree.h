#pragma once

#include "Events/CursorEventsListener.h"
#include "UI/ScrollArea.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	class Sprite;
	class UITree;

	// ------------
	// UI Tree node
	// ------------
	class UITreeNode: public UIWidget
	{
	public:
		// Default constructor
		UITreeNode();

		// Copy-constructor
		UITreeNode(const UITreeNode& other);

		// Destructor
		~UITreeNode();

		// Copy-operator
		UITreeNode& operator=(const UITreeNode& other);

		// Draws widget
		void Draw();

		// Sets expanding
		void SetExpanded(bool expanded);

		// Is node expanded
		bool IsExpanded() const;

		// Expands node
		void Expand();

		// Collapses node
		void Collapse();

		// Expands all nodes
		void ExpandAll();

		// Collapses all nodes
		void CollapseAll();

		// Searches child node with object and returns him
		Ptr<UITreeNode> GetNode(Ptr<char> object);

		// Rebuilds this node
		void Rebuild(bool withChilds = true);

		// Sets children offset
		void SetChildrenOffset(float offset);

		// Returns children offset
		float GetChildrenOffset() const;

		SERIALIZABLE(UITreeNode);

	protected:
		Ptr<UIWidgetState> mExpandedState; // Expanded state. Changes mExpandCoef
		float              mExpandCoef;    // Expand coefficient (0...1)
		Ptr<UnknownType>   mObject;        // Object pointer
		Ptr<UITree>        mTree;          // Owner tree
		float              mChildsOffset;  // Children nodes offset @SERIALIZABLE

	protected:
		// Updates expanding
		void UpdateExpanding(float dt);

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Returns current height with childrens
		float GetCurrentHeight() const;

		friend class UITree;
	};

	// -------
	// UI Tree
	// -------
	class UITree: public UIScrollArea, public CursorEventsListener
	{
	public:
		typedef Vector<Ptr<UnknownType>> UnknownObjectsVec;

	public:
		// Default constructor
		UITree();

		// Copy-constructor
		UITree(const UITree& other);

		// Destructor
		~UITree();

		// Copy-operator
		UITree& operator=(const UITree& other);

		// Sets source processing functions
		void SetSourceFunctions(const Function<Vector<Ptr<UnknownType>>(Ptr<UnknownType>)> getChildsFunc,
								const Function<void(Ptr<UITreeNode>, Ptr<UnknownType>)> setupNodeFunc);

		// Rebuilds all tree
		void RebuildTree();

		// Updates tree node for object
		void UpdateTreeNode(Ptr<UnknownType> object);

		// Returns ui node for object
		Ptr<UITreeNode> GetNode(Ptr<UnknownType> object);

		// Expands all nodes
		void ExpandAll();

		// Collapses all nodes
		void CollapseAll();

		// Returns items vertical layout
		Ptr<UIVerticalLayout> GetItemsLayout() const;

		// Returns item sample
		Ptr<UITreeNode> GetNodeSample() const;

		// Returns selection drawable
		Ptr<Sprite> GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE(UITree);

	protected:
		Function<Vector<Ptr<UnknownType>>(Ptr<UnknownType>)> mGetChildsFunc;
		Function<void(Ptr<UITreeNode>, Ptr<UnknownType>)>    mSetupNodeFunc;

		Ptr<UIVerticalLayout> mLayout;                 // Root items layout
		Ptr<UITreeNode>       mNodeSample;             // Item sample @SERIALIZABLE
		Ptr<Sprite>           mSelectionDrawable;      // Selection sprite @SERIALIZABLE
		Layout                mSelectionLayout;        // Selection layout, result selection area depends on selected item @SERIALIZABLE

		RectF                 mCurrentSelectionRect;   // Current selection rectangle (for smoothing)
		RectF                 mTargetSelectionRect;    // Target selection rectangle (over selected item)
		Vec2F                 mLastSelectCheckCursor;  // Last cursor position on selection check

		int                   mSelectedItem;           // Index of selected item

	protected:

		friend class UITreeNode;
	};
}
