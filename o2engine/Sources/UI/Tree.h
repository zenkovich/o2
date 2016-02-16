#pragma once

#include "Events/DrawableCursorEventsListener.h"
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
		void SetExpanded(bool expanded, bool forcible = false);

		// Is node expanded
		bool IsExpanded() const;

		// Expands node
		void Expand(bool forcible = false);

		// Collapses node
		void Collapse(bool forcible = false);

		// Expands all nodes
		void ExpandAll();

		// Collapses all nodes
		void CollapseAll();

		// Searches child node with object and returns him
		UITreeNode* GetNode(UnknownType* object);

		// Rebuilds this node
		void Rebuild(bool withChilds = true);

		// Sets children offset
		void SetChildrenOffset(float offset);

		// Returns children offset
		float GetChildrenOffset() const;

		// Returns object pointer
		UnknownType* GetObject() const;

		SERIALIZABLE(UITreeNode);

	protected:
		UIWidgetState* mExpandedState;  // Expanded state. Changes mExpandCoef
		float          mExpandCoef;     // Expand coefficient (0...1)
		UnknownType*   mObject;         // Object pointer
		UITree*        mTree;           // Owner tree
		float          mChildsOffset;   // Children nodes offset @SERIALIZABLE
		float          mInsertSizeCoef; // Inserting as previous size coef 
		float          mDragSizeCoef;   // Dragging size coef 1 -> 0

	protected:
		// Creates children nodes
		void RebuildChildrenNodes(Vector<UnknownType*> objects);

		// Removes children nodes
		void RemoveChildrenNodes();

		// Updates expanding
		void UpdateTreeLayout(float dt);

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Returns current height with children
		float GetCurrentHeight() const;

		friend class UITree;
	};

	// -------
	// UI Tree
	// -------
	class UITree: public UIScrollArea, public DrawableCursorEventsListener
	{
	public:
		enum class RearrangeType { Disabled, Enabled, OnlyReparent };

		typedef Vector<UnknownType*> UnknownObjectsVec;
		typedef Vector<UITreeNode*> TreeNodesVec;

	public:
		Function<void(UnknownObjectsVec, UnknownType*, UnknownType*)> onDraggedObjects;
		Function<UnknownType*(UnknownType*)>                          getParentFunc;
		Function<Vector<UnknownType*>(UnknownType*)>                  getChildsFunc;
		Function<void(UITreeNode*, UnknownType*)>                     setupNodeFunc;
		Function<void(UITreeNode*)>                                   onItemClick;
		Function<void(UITreeNode*)>                                   onItemDblClick;
		Function<void(UITreeNode*)>                                   onItemRBClick;
		Function<void(UnknownObjectsVec)>                             onItemsSelectionChanged;

		// Default constructor
		UITree();

		// Copy-constructor
		UITree(const UITree& other);

		// Destructor
		~UITree();

		// Copy-operator
		UITree& operator=(const UITree& other);

		// Draws widget
		void Draw();

		// Updates widget
		void Update(float dt);

		// Rebuilds all tree
		void RebuildTree();

		// Updates tree node for object
		void UpdateTreeNode(UnknownType* object);

		// Returns ui node for object
		UITreeNode* GetNode(UnknownType* object);

		// Expands all nodes
		void ExpandAll();

		// Collapses all nodes
		void CollapseAll();

		// Returns selected objects vector
		Vector<UnknownType*> GetSelectedObjects() const;

		// Sets selected objects
		void SetSelectedObjects(const Vector<UnknownType*>& objects);

		// Selects object
		void SelectObject(UnknownType* object);

		// Deselects object
		void DeselectObject(UnknownType* object);

		// Deselects all objects
		void DeselectAllObjects();

		// Returns item sample
		UITreeNode* GetNodeSample() const;

		// Returns hover drawable
		Sprite* GetHoverDrawable() const;

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Creates tree node for object
		void OnObjectCreated(UnknownType* object, UnknownType* parent);

		// Removes tree node for object
		void OnObjectRemoved(UnknownType* object);

		// Returns is listener scrollable
		bool IsScrollable() const;

		// Sets nodes pool resize count
		void SetNodesPoolResizeCount(int count);

		// Sets selection sprites pool resize count
		void SetSelectionSpritesPoolResizeCount(int count);

		// Sets rearrange type
		void SetRearrangeType(RearrangeType type);

		// Returns available rearrange type 
		RearrangeType GetRearrangeType() const;

		// Sets selected nodes sprite color
		void SetSelectedColor(const Color4& color);

		// Returns selected nodes sprite color
		Color4 GetSelectedColor() const;

		// Sets not selected nodes sprite color
		void SetUnselectedColor(const Color4& color);

		// Returns not selected nodes sprite color
		Color4 GetUnselectedColor() const;

		// Sets hover nodes sprite color
		void SetHoverColor(const Color4& color);

		// Returns hover nodes sprite color
		Color4 GetHoverColor() const;

		// Returns is this widget can be selected
		bool IsSelectable() const;

		SERIALIZABLE(UITree);

	protected:
		struct SelectedNode
		{
			UnknownType* object = nullptr;
			UITreeNode*  node = nullptr;
			Sprite*      selectionSprite = nullptr;

			bool operator==(const SelectedNode& other) const;
		};
		typedef Vector<SelectedNode> SelectedNodesVec;
		typedef Vector<Sprite*>  SpritesVec;

	protected:
		int               mNodesPoolResizeCount = 20;            
		int               mSelectionSpritesPoolResizeCount = 10;
		Color4            mSelectedColor = Color4(175, 175, 255, 150);   // @SERIALIZABLE
		Color4            mUnselectedColor = Color4(100, 100, 100, 100); // @SERIALIZABLE
		Color4            mHoverColor = Color4(100, 100, 100, 100);      // @SERIALIZABLE
						  
		TreeNodesVec      mAllNodes;              // Array of all tree nodes
		UITreeNode*       mNodeSample;            // Item sample @SERIALIZABLE
		Sprite*           mHoverDrawable;         // Selection sprite @SERIALIZABLE
		Sprite*           mSelectedDrawable;      // Node selection drawable @SERIALIZABLE
		Layout            mHoverLayout;           // Selection layout, result selection area depends on selected item @SERIALIZABLE
						  
		RectF             mCurrentHoverRect;      // Current selection rectangle (for smoothing)
		RectF             mTargetHoverRect;       // Target selection rectangle (over selected item)
		Vec2F             mLastHoverCheckCursor;  // Last cursor position on selection check
		Vec2F             mPressedPoint;          // Last cursor pressed point
						  
		UITreeNode*       mHoveredItem;           // Current hovered tree node item
		SelectedNodesVec  mSelectedItems;         // Selected items
		bool              mWaitSelectionsUpdate;  // True when waiting for updating tree nodes selection checking
						  
		TreeNodesVec      mNodesPool;             // Tree nodes pool
		SpritesVec        mSelectionSpritesPool;  // Selections sprites pool

		UnknownObjectsVec mExpandedObjects;       // Expanded objects nodes

		RearrangeType     mRearrangeType;         // Current available rearrange type
		bool              mDraggingNodes;         // Is nodes moving by cursor
		UITreeNode*       mDragNode;              // Dragging node
		Sprite*           mDragNodeBack;          // Background for dragging node
		Vec2F             mDragOffset;            // Offset from cursor to dragging node's center
		UITreeNode*       mPressedNode;           // Node pressed by cursor
		UITreeNode*       mInsertNodeCandidate;   // Insertion node candidate when dragging nodes
		UITreeNode*       mUnderCursorItem;       // Item under cursor
		UITreeNode*       mExpandNodeCandidate;   // Expand node candidate when dragging
		float             mExpandInsertTime;      // Remaining time to expanding item under cursor when dragging nodes
		float             mPressedTime;           // Time from last item pressing
						  
		float             mDrawDepth;             // Drawing depth
		bool              mNeedUpdateLayout;      // True if layout needs to update

	protected:
		// Calls when widget was selected
		void OnSelected();

		// Calls when widget was deselected
		void OnDeselected();

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates scroll parameters: clip area, scroll size
		void UpdateScrollParams();

		// Returns current height with children
		float GetCurrentHeight() const;
		
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Selects single node under cursor
		void SelectSingleNode(const Input::Cursor& cursor);

		// Selects multiple nodes from cursor to lower selected node
		void SelectMultipleNodes(const Input::Cursor& cursor);

		// Calls when items selected
		void OnItemsSelected();

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Returns item widget under point
		UITreeNode* GetItemUnderPoint(const Vec2F& point);

		// Updates nodes dragging
		void UpdateDragging(const Input::Cursor& cursor);

		// Updates hover target rect and visibility
		void UpdateHover(UITreeNode* itemUnderCursor);

		// Begins dragging selected items
		void BeginDragging();

		// End nodes dragging
		void EndDragging();

		// Gets tree node from pool
		UITreeNode* CreateTreeNode();

		// Frees node to pool
		void FreeTreeNode(UITreeNode* node);

		// Gets free selection sprite from pool
		Sprite* CreateSelectionSprite();

		// Frees selection sprite and puts in pool
		void FreeSelectionSprite(Sprite* sprite);

		// Checks selected object for nodes bindings
		void CheckSelectedNodes();

		friend class UITreeNode;
	};
}
