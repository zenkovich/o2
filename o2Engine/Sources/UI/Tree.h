#pragma once

#include "UI/ScrollArea.h"
#include "UI/VerticalLayout.h"
#include "Utils/DragAndDrop.h"
#include "Utils/Math/Curve.h"
#include "Utils/UnknownPtr.h"

namespace o2
{
	class Sprite;
	class UIButton;
	class UITreeNode;

	// -------
	// UI Tree
	// -------
	class UITree: public UIScrollArea, public DragDropArea, public ISelectableDragableObjectsGroup
	{
	public:
		// Tree nodes rearrange available types
		enum class RearrangeType { Disabled, Enabled, OnlyReparent };

		// Tree node expand states
		enum class ExpandState { None, Expanding, Collaping };

		typedef Vector<UnknownPtr>  UnknownPtrsVec;
		typedef Vector<UITreeNode*> TreeNodesVec;

	public:
		Function<UnknownPtr(UnknownPtr)>         getObjectParentDelegate;        // Getting objects' parent delegate
		Function<Vector<UnknownPtr>(UnknownPtr)> getObjectChildrenDelegate;      // Getting objects' childs count delegate 
		Function<void(UITreeNode*, UnknownPtr)>  fillNodeDataByObjectDelegate;   // Setup tree node item delegate
		Function<String(UnknownPtr)>             getDebugForObject;              // Getting debug string for object delegate
		Function<void(UITreeNode*)>              onNodeDoubleClicked;            // Node double clicked event
		Function<void(UITreeNode*)>              onNodeRightButtonClicked;       // Node right button click event
		Function<void(UnknownPtrsVec)>           onObjectsSelectionChanged;      // Objects selected event

		Function<void(UnknownPtrsVec, UnknownPtr, UnknownPtr)> onDraggedObjects; // Objects dragged event

		// Default constructor
		UITree();

		// Copy-constructor
		UITree(const UITree& other);

		// Destructor
		~UITree();

		// Copy-operator
		UITree& operator=(const UITree& other);

		// Creates tree node for object
		void OnObjectCreated(UnknownPtr object, UnknownPtr parent);

		// Removes tree node for object
		void OnObjectRemoved(UnknownPtr object);

		// Updates tree for changed objects
		void OnObjectsChanged(const UnknownPtrsVec& objects);

		// Draws widget
		void Draw();

		// Updates widget
		void Update(float dt);

		// Rebuilds all tree
		void UpdateNodesView(bool immediately = true);

		// Returns ui node for object
		UITreeNode* GetNode(UnknownPtr object);

		// Expands all nodes
		void ExpandAll();

		// Collapses all nodes
		void CollapseAll();

		// Returns selected objects vector
		Vector<UnknownPtr> GetSelectedObjects() const;

		// Sets selected objects
		void SetSelectedObjects(const UnknownPtrsVec& objects);

		// Selects object
		void SelectObject(UnknownPtr object);

		// Selects object
		void SelectAndHightlightObject(UnknownPtr object);

		// Deselects object
		void DeselectObject(UnknownPtr object);

		// Deselects all objects
		void DeselectAllObjects();

		// Scrolls view to object
		void ScrollTo(UnknownPtr object);

		// Scrolls view to object and hightlights
		void ScrollToAndHightlight(UnknownPtr object);

		// Expands all parent objects for specified object
		void ExpandParentObjects(UnknownPtr object);

		// Returns item widget under point
		UITreeNode* GetTreeNodeUnderPoint(const Vec2F& point);

		// Sets rearrange type
		void SetRearrangeType(RearrangeType type);

		// Returns available rearrange type 
		RearrangeType GetRearrangeType() const;

		// Sets available multi selection
		void SetMultipleSelectionAvailable(bool available);

		// Returns is multi selection available
		bool IsMultiSelectionAvailable() const;

		// Returns item sample
		UITreeNode* GetNodeSample() const;

		// Returns hover drawable
		Sprite* GetHoverDrawable() const;

		// Sets hover layout
		void SetHoverLayout(const Layout& layout);

		// Returns node hightlight drawable
		Sprite* GetHightlightDrawable() const;

		// Sets hightlight animation
		void SetHightlightAnimation(const Animation& animation);

		// Sets hightlight layout
		void SetHightlightLayout(const Layout& layout);

		// Sets node pressing and expanding time
		void SetNodeExpandTimer(float time);

		// Returns node pressing and expanding time
		float GetNodeExpandTimer() const;

		// Sets children nodes horizontal offset from parent
		void SetChildsNodesOffset(float offset);

		// Returns childs nodes horizontal offset
		float GetChildsNodesOffset() const;

		// Returns is listener scrollable
		bool IsScrollable() const;

		// Returns is this widget can be selected
		bool IsFocusable() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		SERIALIZABLE(UITree);

	protected:
		struct Node;
		typedef Vector<Node*> NodesVec;

		// --------------------
		// Tree node definition
		// --------------------
		struct Node
		{
			String      id;
			UnknownPtr  object;             // Pointer to object
			UITreeNode* widget = nullptr;   // Node widget
			int         level = 0;          // Hierarchy depth level
			bool        isSelected = false; // Is node selected
			bool        isExpanded = false; // Is node expanded

			Node*       parent = nullptr;   // Parent node definition
			NodesVec    childs;             // Children nodes definitions

			bool        inserting = false;  // Node insertion flag
			float       insertCoef = 0.0f;  // Inserting coefficient (0...1)

		public:
			// Returns height of node with children and coefficients
			float GetHeight() const;

			// Sets widget state to selected and stores selected flag
			void SetSelected(bool selected);

			// Returns all children count
			int GetChildCount() const;
		};

	protected:
		// -----------------------------------
		// Visible tree node widget cache info
		// -----------------------------------
		struct VisibleWidgetDef
		{
			UnknownPtr  object;
			UITreeNode* widget;
			int         position;

			bool operator==(const VisibleWidgetDef& other) const;
		};
		typedef Vector<VisibleWidgetDef> VisibleWidgetsDefsVec;

	protected:
		RearrangeType  mRearrangeType = RearrangeType::Enabled; // Current available rearrange type @SERIALIZABLE
		bool           mMultiSelectAvailable = true;            // Is multi selection available @SERIALIZABLE

		UITreeNode*    mNodeWidgetSample = nullptr;             // Item sample @SERIALIZABLE
		float          mChildsOffset = 10.0f;                   // Children nodes offset from parent @SERIALIZABLE

		bool           mIsNeedUpdateView = false;               // Is tree needs to be rebuild
		bool           mIsNeedUdateLayout = false;              // Is layout needs to rebuild
		bool           mIsNeedUpdateVisibleNodes = false;       // In need to update visible nodes

		NodesVec       mAllNodes;                               // All expanded nodes definitions

		UnknownPtrsVec mSelectedObjects;                        // Selected objects
		NodesVec       mSelectedNodes;                          // Selected nodes definitions

		TreeNodesVec   mNodeWidgetsBuf;                         // Nodes widgets buffer
		NodesVec       mNodesBuf;                               // Nodes buffer

		NodesVec       mVisibleNodes;                           // Visible nodes
		int            mMinVisibleNodeIdx = 0;                  // Minimal visible node index
		int            mMaxVisibleNodeIdx = -1;                 // Maximum visible node index

		Vec2F          mLastClickPos;                           // Last click position in scroll space (depends on scroll position)

		UITreeNode*    mHoveredItem = nullptr;                  // Current hovered tree node item
		Sprite*        mHoverDrawable = nullptr;                // Selection sprite @SERIALIZABLE
		Layout         mHoverLayout;                            // Selection layout, result selection area depends on selected item @SERIALIZABLE						  										   
		RectF          mCurrentHoverRect;                       // Current selection rectangle (for smoothing)
		RectF          mTargetHoverRect;                        // Target selection rectangle (over selected item)	

		bool           mIsDraggingNodes = false;                // Is nodes moving by cursor
		UITreeNode*    mFakeDragNode = nullptr;                 // Dragging node
		Vec2F          mDragOffset;                             // Offset from cursor to dragging node's center
		UITreeNode*    mInsertNodeCandidate = nullptr;          // Insertion node candidate when dragging nodes
		UnknownPtrsVec mBeforeDragSelectedItems;                // Before drag begin selection
		bool           mDragEnded = false;                      // Is dragging ended and it needs to call EndDragging

		UnknownPtrsVec mExpandedObjects;                        // Expanded objects

		ExpandState    mExpandingNodeState = ExpandState::None; // Expanding node state
		int            mExpandingNodeIdx = -1;                  // Current expanding node index. -1 if no expanding node
		int            mExpandingNodeChildsCount = 0;           // Current expanding node children count
		float          mExpandingNodePosition = 0.0f;           // Current expanding node position in this local coordinates
		float          mExpandingNodeBottomPosition = 0.0f;     // Current expanding node position of bottom
		float          mExpandingNodeCurrCoef = 0.0f;           // Current expanding node coefficient of expanding
		float          mExpandingNodeCurrHeight = 0.0f;         // Current expanding node current height
		float          mExpandingNodeTargetHeight = 0.0f;       // Current expanding node target height
		float          mExpandNodeTime = 0.25f;                 // Node expanding time
		Curve          mExpandingNodeFunc = Curve::EaseInOut(); // Expanding easing node curve

		UITreeNode*    mExpandNodeCandidate = nullptr;          // Expand node candidate when dragging
		float          mExpandInsertTime = -1.0f;               // Remaining time to expanding item under cursor when dragging nodes
		float          mPressedTime = 10.0f;                    // Time from last item pressing

		float          mNodeExpandTime = 2.0f;                  // Node expand time when dragging actors @SERIALIZABLE
		float          mNodeDragIntoZone = 0.3f;                // Node inside dragging zone coefficient (0.5 is full node area) @SERIALIZABLE

		Animation      mHightlightAnim;                         // Node hightlight animation @SERIALIZABLE
		Sprite*        mHightlightSprite = nullptr;             // Node hightlight sprite @SERIALIZABLE
		Layout         mHightlightLayout;                       // Node hightlight sprite layout @SERIALIZABLE
		Node*          mHightlighNode = nullptr;                // Hightlighting node

		VisibleWidgetsDefsVec mVisibleWidgetsCache;             // Visible widgets cache

	protected:
		// Returns object's parent
		virtual UnknownPtr GetObjectParent(UnknownPtr object);

		// Returns object's children
		virtual Vector<UnknownPtr> GetObjectChilds(UnknownPtr object);

		// Returns debugging string for object
		virtual String GetObjectDebug(UnknownPtr object);

		// Sets nodeWidget data by object
		virtual void FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object);

		// Calls when tree node was double clicked
		virtual void OnNodeDblClick(UITreeNode* nodeWidget);

		// Calls when tree node was clicked by right button
		virtual void OnNodeRBClick(UITreeNode* nodeWidget);

		// Calls when list of selected objects was changed
		virtual void OnNodesSelectionChanged(UnknownPtrsVec objects);

		// Calls when objects was dragged in new parent in position next of prevObject
		virtual void OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject);

// ISelectableDragableObjectsGroup implementation

		// Returns selected objects in group
		SelectDragObjectsVec GetSelectedDragObjects() const;

		// Returns all objects in group 
		SelectDragObjectsVec GetAllObjects() const;

		// Selects object
		void Select(SelectableDragableObject* object);

		// Selects object
		void Select(SelectableDragableObject* object, bool sendOnSelectionChanged);

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

// -------------

		// Checks multiple selection nodes (when Shift key is down) and returns is someone was selected
		bool CheckMultipleSelection(const Vec2F& point);

		// Calls when items selected
		void OnSelectionChanged();

		// Checks and updates pressed node expanding
		void UpdatePressedNodeExpand(float dt);

		// Updates root nodes and their childs if need
		void UpdateNodesStructure();

		// Inserts node to hierarchy
		int InsertNodes(Node* parentNode, int position, NodesVec* newNodes = nullptr);

		// Removes node from hierarchy
		void RemoveNodes(Node* parentNode);

		// Creates node from object with parent
		Node* CreateNode(UnknownPtr object, Node* parent);

		// Calls when widget was selected
		void OnFocused();

		// Calls when widget was deselected
		void OnUnfocused();

		// Updates visible nodes (calculates range and initializes nodes)
		void UpdateVisibleNodes();

		// Createw UITreeNode for visible node at position i
		void CreateVisibleNodeWidget(Node* node, int i);

		// Updates node view
		void UpdateNodeView(Node* node, UITreeNode* widget, int idx);

		// Updates node widget layout
		void UpdateNodeWidgetLayout(Node* node, int idx);

		// Returns node index from mAllNodes by position in local coordinates
		int GetNodeIndex(float position) const;

		// Returns node position in local coordinates by index in mAllNodes array
		float GetNodePosition(int idx) const;

		// Expands node: creates his children and sets expanded state
		void ExpandNode(Node* node);

		// Collapses node: removes children from hierarchy and widgets if they're visible
		void CollapseNode(Node* node);

		// Starts expanding or collapse animation of node
		void StartExpandingAnimation(ExpandState direction, Node* node, int childrenCount);

		// Updates node expanding
		void UpdateNodeExpanding(float dt);

		// Calculates scroll area
		void CalculateScrollArea();

		// Moves scroll position and updates children widgets clipping and layout
		void MoveScrollPosition(const Vec2F& delta);

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
		UITreeNode* CreateTreeNodeWidget();

// DragDropArea implementation

		// Begins dragging selected items
		void BeginDragging(UITreeNode* node);

		// Ends dragging items
		void EndDragging(bool droppedToThis = false);

		// Updates dragging graphics
		void UpdateDraggingGraphics();

		// Updates insert states on nodes when dragging
		void UpdateDraggingInsertion();

		// Updates insertion animation when dragging
		void UpdateDraggingInsertionAnim(float dt);

		// Calls when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group);

		// Calls when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

// ------------

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		friend class UITreeNode;
	};

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

		// Sets expanding
		void SetExpanded(bool expanded, bool forcible = false);

		// Is node expanded
		bool IsExpanded() const;

		// Expands node
		void Expand(bool forcible = false);

		// Collapses node
		void Collapse(bool forcible = false);

		// Returns object pointer
		UnknownPtr GetObject() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UITreeNode);

	protected:
		UITree::Node* mNodeDef = nullptr;       // Node definition
		UITree*       mOwnerTree = nullptr;     // Owner tree
		UIButton*     mExpandBtn = nullptr;     // Node expanding button

	protected:
		// Updates expanding
		void UpdateTreeLayout(float dt);

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
}
