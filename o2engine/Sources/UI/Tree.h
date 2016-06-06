#pragma once

#include "UI/ScrollArea.h"
#include "UI/VerticalLayout.h"
#include "Utils/DragAndDrop.h"

namespace o2
{
	class Sprite;
	class UITree;

	// ------------
	// UI Tree node
	// ------------
	class UITreeNode: public UIWidget, public SelectableDragableObject
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

		// Updates widget
		void Update(float dt);

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
		void UpdateView(bool withChilds = true, bool deepRebuild = true, bool immediately = true);

		// Sets children offset
		void SetChildrenOffset(float offset);

		// Returns children offset
		float GetChildrenOffset() const;

		// Returns object pointer
		UnknownType* GetObject() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UITreeNode);

	protected:
		bool           mNeedRebuild = false;     // Is node needs to be rebuilded
		UIWidgetState* mExpandedState = nullptr; // Expanded state. Changes mExpandCoef
		float          mExpandCoef = 0.0f;       // Expand coefficient (0...1)
		UnknownType*   mObject = nullptr;        // Object pointer
		UITree*        mOwnerTree = nullptr;     // Owner tree
		float          mChildsOffset = 0.0f;     // Children nodes offset @SERIALIZABLE
		float          mInsertSizeCoef = 0.0f;   // Inserting as previous size coef 
		float          mDragSizeCoef = 1.0f;     // Dragging size coef 1 -> 0

	protected:
		// Creates children nodes
		void RebuildChildrenNodes(Vector<UnknownType*> objects, bool deepRebuild);

		// Removes children nodes
		void RemoveChildrenNodes();

		// Updates expanding
		void UpdateTreeLayout(float dt);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Returns current height with children, if straight is false - don't using coefficients
		float GetCurrentHeight(bool straight = false) const;

		// Calls when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor);

		// Calls when cursor enters this object, moving hover of tree to this
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object, moving hover of tree to this
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when started dragging
		void OnDragStart(const Input::Cursor& cursor);

		// Calls when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area);

		// Calls when dragging completed
		void OnDragEnd(const Input::Cursor& cursor);

		// Calls when this was selected
		void OnSelected();

		// Calls when this was unselected
		void OnDeselected(); 

		friend class UITree;
	};

	// -------
	// UI Tree
	// -------
	class UITree: public UIScrollArea, public DragDropArea, public ISelectableDragableObjectsGroup
	{
	public:
		enum class RearrangeType { Disabled, Enabled, OnlyReparent };

		typedef Vector<UnknownType*> UnknownObjectsVec;
		typedef Vector<UITreeNode*> TreeNodesVec;

	public:
		Function<void(UnknownObjectsVec, UnknownType*, UnknownType*)> onDraggedObjects;        // Items dragged event
		Function<UnknownType*(UnknownType*)>                          getParentFunc;           // Getting objects' parent delegate
		Function<Vector<UnknownType*>(UnknownType*)>                  getChildsFunc;           // Getting objects' childs count delegate 
		Function<void(UITreeNode*, UnknownType*)>                     setupNodeFunc;           // Setup tree node item delegate
		Function<void(UITreeNode*)>                                   onItemClick;             // Item click event
		Function<void(UITreeNode*)>                                   onItemDblClick;          // Item double clicked event
		Function<void(UITreeNode*)>                                   onItemRBClick;           // Item right button click event
		Function<void(UnknownObjectsVec)>                             onItemsSelectionChanged; // Items selected event

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
		void UpdateView(bool immediately = true);

		// Updates tree node for object
		void UpdateNodeView(UnknownType* object);

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

		// Selects object
		void SelectAndExpandObject(UnknownType* object);

		// Deselects object
		void DeselectObject(UnknownType* object);

		// Deselects all objects
		void DeselectAllObjects();

		// Scrolls view to object
		void ScrollTo(UnknownType* object);

		// Returns item widget under point
		UITreeNode* GetTreeNodeUnderPoint(const Vec2F& point);

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

		// Updates tree for changed objects
		void OnObjectsChanged(const UnknownObjectsVec& objects);

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

		// Sets available multi selection
		void SetMultipleSelectionAvailable(bool available);

		// Returns is multi selection available
		bool IsMultiSelectionAvailable() const;

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

		// Sets node pressing and expanding time
		void SetNodeExpandTimer(float time);

		// Returns node pressing and expanding time
		float GetNodeExpandTimer() const;

		// Returns is this widget can be selected
		bool IsFocusable() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UITree);

	protected:
		// -----------------------------
		// Selected tree node definition
		// -----------------------------
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
		int    mNodesPoolResizeCount = 20;                         // Nodes pool resize step
		int    mSelectionSpritesPoolResizeCount = 10;              // Sprites for selection backs pool resize step
		Color4 mSelectedColor = Color4(175, 175, 255, 150);        // Selected node background color @SERIALIZABLE
		Color4 mUnselectedColor = Color4(100, 100, 100, 100);      // Selected node background color, when unfocused @SERIALIZABLE
		Color4 mHoverColor = Color4(100, 100, 100, 100);           // Hovered node background color @SERIALIZABLE
		float  mNodeExpandTime = 0.6f;                             // Node expand time when dragging actors @SERIALIZABLE
		float  mNodeDragIntoZone = 0.3f;                           // Node inside dragging zone coefficient (0.5 is full node area)
																   
		bool              mNeedUpdateView = false;                 // Is tree needs to be rebuilded
		TreeNodesVec      mAllNodes;                               // Array of all tree nodes
		UITreeNode*       mNodeSample = nullptr;                   // Item sample @SERIALIZABLE
		Sprite*           mHoverDrawable = nullptr;                // Selection sprite @SERIALIZABLE
		Sprite*           mSelectedDrawable = nullptr;             // Node selection drawable @SERIALIZABLE
		Layout            mHoverLayout;                            // Selection layout, result selection area depends on selected item @SERIALIZABLE
						  										   
		RectF             mCurrentHoverRect;                       // Current selection rectangle (for smoothing)
		RectF             mTargetHoverRect;                        // Target selection rectangle (over selected item)
		Vec2F             mLastHoverCheckCursor;                   // Last cursor position on selection check
		Vec2F             mPressedPoint;                           // Last cursor pressed point
						  										   
		UITreeNode*       mHoveredItem = nullptr;                  // Current hovered tree node item
		SelectedNodesVec  mSelectedItems;                          // Selected items
		bool              mWaitSelectionsUpdate = false;           // True when waiting for updating tree nodes selection checking
						  										   
		TreeNodesVec      mNodesPool;                              // Tree nodes pool
		SpritesVec        mSelectionSpritesPool;                   // Selections sprites pool
																   
		UnknownObjectsVec mExpandedObjects;                        // Expanded objects nodes
																   
		RearrangeType     mRearrangeType = RearrangeType::Enabled; // Current available rearrange type
		bool              mMultiSelectAvailable = true;            // Is multi selection available
																   
		bool              mIsDraggingNodes = false;                // Is nodes moving by cursor
		UITreeNode*       mFakeDragNode = nullptr;                 // Dragging node
		Sprite*           mFakeDragNodeBack = nullptr;             // Background for dragging node
		Vec2F             mDragOffset;                             // Offset from cursor to dragging node's center
		UITreeNode*       mInsertNodeCandidate = nullptr;          // Insertion node candidate when dragging nodes
		SelectedNodesVec  mBeforeDragSelectedItems;                // Before drag begin selection
																   
		UITreeNode*       mExpandNodeCandidate = nullptr;          // Expand node candidate when dragging
		float             mExpandInsertTime = -1.0f;               // Remaining time to expanding item under cursor when dragging nodes
		float             mPressedTime = 10.0f;                    // Time from last item pressing
						  										   
		bool              mNeedUpdateLayout = false;               // True if layout needs to update

	protected:
		// Returns selected objects in group
		SelectDragObjectsVec GetSelectedDragObjects() const;

		// Returns all objects in group 
		SelectDragObjectsVec GetAllObjects() const;

		// Selects object
		void Select(SelectableDragableObject* object);

		// Deselects object
		void Deselect(SelectableDragableObject* object);

		// Adds selectable object to group
		void AddSelectableObject(SelectableDragableObject* object);

		// Removes selectable object from group
		void RemoveSelectableObject(SelectableDragableObject* object);

		// Calls when selectable draggable object was released
		void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor);

		// Calls when selectable object was began to drag
		void OnSelectableObjectBeganDragging(SelectableDragableObject* object);

		// Checks multiple selection nodes (when Shift key is down) and returns is someone was selected
		bool CheckMultipleSelection(const Vec2F& point);

		// Calls when items selected
		void OnSelectionChanged();

		// Checks and updates pressed node expanding
		void UpdatePressedNodeExpand(float dt);

		// Updates root nodes and their childs if need
		void UpdateRootNodes(bool updateChilds);

		// Calls when widget was selected
		void OnFocused();

		// Calls when widget was deselected
		void OnUnfocused();

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Calculates scroll area
		void CalculateScrollArea();

		// Returns current height with children
		float GetCurrentHeight() const;
		
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Updates hover target rect and visibility
		void UpdateHover(UITreeNode* itemUnderCursor);

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

		// Begins dragging selected items
		void BeginDragging(UITreeNode* node);

		// Ends dragging items
		void EndDragging();

		// Updates dragging graphics
		void UpdateDraggingGraphics();

		// Updates insert states on nodes when dragging
		void UpdateDraggingInsertion();

		// Calls when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group);

		// Calls when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

		// Returns tree node offset from top of this
		float GetTreeNodeOffset(UITreeNode* node);

		friend class UITreeNode;
	};
}
