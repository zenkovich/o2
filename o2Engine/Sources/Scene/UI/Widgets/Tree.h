#pragma once

#include "Scene/UI/Widgets/ScrollArea.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Utils/Editor/DragAndDrop.h"
#include "Utils/Math/Curve.h"
#include "Utils/Types/UnknownPtr.h"

namespace o2
{
	class Sprite;
	class Button;
	class TreeNode;

	// -------
	// UI Tree
	// -------
	class Tree: public ScrollArea, public DragDropArea, public ISelectableDragableObjectsGroup
	{
	public:
		// Tree nodes rearrange available types
		enum class RearrangeType { Disabled, Enabled, OnlyReparent };

		// Tree node expand states
		enum class ExpandState { None, Expanding, Collaping };

		typedef Vector<UnknownPtr>  UnknownPtrsVec;
		typedef Vector<TreeNode*> TreeNodesVec;

	public:
		Function<UnknownPtr(UnknownPtr)>         getObjectParentDelegate;        // Getting objects' parent delegate
		Function<Vector<UnknownPtr>(UnknownPtr)> getObjectChildrenDelegate;      // Getting objects' childs count delegate 
		Function<void(TreeNode*, UnknownPtr)>  fillNodeDataByObjectDelegate;   // Setup tree node item delegate
		Function<String(UnknownPtr)>             getDebugForObject;              // Getting debug string for object delegate
		Function<void(TreeNode*)>              onNodeDoubleClicked;            // Node double clicked event
		Function<void(TreeNode*)>              onNodeRightButtonClicked;       // Node right button click event
		Function<void(UnknownPtrsVec)>           onObjectsSelectionChanged;      // Objects selected event

		Function<void(UnknownPtrsVec, UnknownPtr, UnknownPtr)> onDraggedObjects; // Objects dragged event

		// Default constructor
		Tree();

		// Copy-constructor
		Tree(const Tree& other);

		// Destructor
		~Tree();


		// Copy-operator
		Tree& operator=(const Tree& other);


		// Creates tree node for object
		void OnObjectCreated(UnknownPtr object, UnknownPtr parent);

		// Removes tree node for object
		void OnObjectRemoved(UnknownPtr object);

		// Updates tree for changed objects
		void OnObjectsChanged(const UnknownPtrsVec& objects);


		// Draws widget
		void Draw() override;


		// Updates widget
		void Update(float dt) override;

		// Updates childs
		void UpdateChildren(float dt) override;


		// Updates children and internal children transforms
		void UpdateChildrenTransforms() override;


		// Rebuilds all tree
		void UpdateNodesView(bool immediately = true);


		// Returns ui node for object
		TreeNode* GetNode(UnknownPtr object);


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
		TreeNode* GetTreeNodeUnderPoint(const Vec2F& point);


		// Sets rearrange type
		void SetRearrangeType(RearrangeType type);

		// Returns available rearrange type 
		RearrangeType GetRearrangeType() const;


		// Sets available multi selection
		void SetMultipleSelectionAvailable(bool available);

		// Returns is multi selection available
		bool IsMultiSelectionAvailable() const;


		// Returns item sample
		TreeNode* GetNodeSample() const;

		// Sets item sample
		void SetNodeSample(TreeNode* sample);


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
		bool IsScrollable() const override;

		// Returns is this widget can be selected
		bool IsFocusable() const override;


		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;


		// Updates layout
		void UpdateSelfTransform() override;

		SERIALIZABLE(Tree);

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
			TreeNode* widget = nullptr;   // Node widget
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
			TreeNode* widget;
			int         position;

			bool operator==(const VisibleWidgetDef& other) const;
		};
		typedef Vector<VisibleWidgetDef> VisibleWidgetsDefsVec;

	protected:
		RearrangeType  mRearrangeType = RearrangeType::Enabled; // Current available rearrange type @SERIALIZABLE
		bool           mMultiSelectAvailable = true;            // Is multi selection available @SERIALIZABLE

		TreeNode*    mNodeWidgetSample = nullptr;             // Item sample @SERIALIZABLE
		float          mChildrenOffset = 10.0f;                 // Children nodes offset from parent @SERIALIZABLE

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

		TreeNode*    mHoveredItem = nullptr;                  // Current hovered tree node item
		Sprite*        mHoverDrawable = nullptr;                // Selection sprite @SERIALIZABLE
		Layout         mHoverLayout;                            // Selection layout, result selection area depends on selected item @SERIALIZABLE						  										   
		RectF          mCurrentHoverRect;                       // Current selection rectangle (for smoothing)
		RectF          mTargetHoverRect;                        // Target selection rectangle (over selected item)	

		bool           mIsDraggingNodes = false;                // Is nodes moving by cursor
		TreeNode*    mFakeDragNode = nullptr;                 // Dragging node
		Vec2F          mDragOffset;                             // Offset from cursor to dragging node's center
		TreeNode*    mInsertNodeCandidate = nullptr;          // Insertion node candidate when dragging nodes
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
		float          mExpandNodeTime = 0.4f;                  // Node expanding time
		Curve          mExpandingNodeFunc = Curve::EaseInOut(); // Expanding easing node curve

		TreeNode*    mExpandNodeCandidate = nullptr;          // Expand node candidate when dragging
		float          mExpandInsertTime = -1.0f;               // Remaining time to expanding item under cursor when dragging nodes
		float          mPressedTime = 10.0f;                    // Time from last item pressing

		float          mNodeExpandTime = 2.0f;                  // Node expand time when dragging actors @SERIALIZABLE
		float          mNodeDragIntoZone = 0.3f;                // Node inside dragging zone coefficient (0.5 is full node area) @SERIALIZABLE

		Animation      mHighlightAnim;                          // Node highlight animation @SERIALIZABLE
		Sprite*        mHighlightSprite = nullptr;              // Node highlight sprite @SERIALIZABLE
		Layout         mHighlightLayout;                        // Node highlight sprite layout @SERIALIZABLE
		Node*          mHighlighNode = nullptr;                 // Hightlighing node
		UnknownPtr     mHighlightObject;                        // Highlight object

		VisibleWidgetsDefsVec mVisibleWidgetsCache;             // Visible widgets cache

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when widget was selected
		void OnFocused() override;

		// It is called when widget was deselected
		void OnUnfocused() override;

		// Returns object's parent
		virtual UnknownPtr GetObjectParent(UnknownPtr object);

		// Returns object's children
		virtual Vector<UnknownPtr> GetObjectChilds(UnknownPtr object);

		// Returns debugging string for object
		virtual String GetObjectDebug(UnknownPtr object);

		// Sets nodeWidget data by object
		virtual void FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object);

		// It is called when tree node was double clicked
		virtual void OnNodeDblClick(TreeNode* nodeWidget);

		// It is called when tree node was clicked by right button
		virtual void OnNodeRBClick(TreeNode* nodeWidget);

		// It is called when list of selected objects was changed
		virtual void OnNodesSelectionChanged(UnknownPtrsVec objects);

		// It is called when objects was dragged in new parent in position next of prevObject
		virtual void OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject);

// ISelectableDragableObjectsGroup implementation

		// Returns selected objects in group
		SelectDragObjectsVec GetSelectedDragObjects() const override;

		// Returns all objects in group 
		SelectDragObjectsVec GetAllObjects() const override;

		// Selects object
		void Select(SelectableDragableObject* object) override;

		// Selects object
		void Select(SelectableDragableObject* object, bool sendOnSelectionChanged);

		// Deselects object
		void Deselect(SelectableDragableObject* object) override;

		// Adds selectable object to group
		void AddSelectableObject(SelectableDragableObject* object) override;

		// Removes selectable object from group
		void RemoveSelectableObject(SelectableDragableObject* object) override;

		// It is called when selectable draggable object was released
		void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) override;

		// It is called when selectable object was began to drag
		void OnSelectableObjectBeganDragging(SelectableDragableObject* object) override;

// -------------

		// Checks multiple selection nodes (when Shift key is down) and returns is someone was selected
		bool CheckMultipleSelection(const Vec2F& point);

		// It is called when items selected
		void OnSelectionChanged();

		// Updates highlight animation
		void UpdateHighlighting(float dt);

		// Checks and updates pressed node expanding
		void UpdatePressedNodeExpand(float dt);

		// Updates root nodes and their childs if need
		virtual void UpdateNodesStructure();

		// Inserts node to hierarchy
		int InsertNodes(Node* parentNode, int position, NodesVec* newNodes = nullptr);

		// Removes node from hierarchy
		void RemoveNodes(Node* parentNode);

		// Creates node from object with parent
		Node* CreateNode(UnknownPtr object, Node* parent);

		// Updates visible nodes (calculates range and initializes nodes)
		virtual void UpdateVisibleNodes();

		// Createw UITreeNode for visible node at position i
		void CreateVisibleNodeWidget(Node* node, int i);

		// Updates node view
		void UpdateNodeView(Node* node, TreeNode* widget, int idx);

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

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// Updates hover target rect and visibility
		void UpdateHover(TreeNode* itemUnderCursor);

		// Gets tree node from pool or creates new
		virtual TreeNode* CreateTreeNodeWidget();

// DragDropArea implementation

		// Begins dragging selected items
		void BeginDragging(TreeNode* node);

		// Ends dragging items
		void EndDragging(bool droppedToThis = false);

		// Updates dragging graphics
		void UpdateDraggingGraphics();

		// Updates insert states on nodes when dragging
		void UpdateDraggingInsertion();

		// Updates insertion animation when dragging
		void UpdateDraggingInsertionAnim(float dt);

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

// ------------

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node) override;

		friend class TreeNode;
	};

	// ------------
	// UI Tree node
	// ------------
	class TreeNode: public Widget, public SelectableDragableObject
	{
	public:
		// Default constructor
		TreeNode();

		// Copy-constructor
		TreeNode(const TreeNode& other);

		// Destructor
		~TreeNode();

		// Copy-operator
		TreeNode& operator=(const TreeNode& other);

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
		bool IsUnderPoint(const Vec2F& point) override;

		SERIALIZABLE(TreeNode);

	protected:
		Tree::Node* mNodeDef = nullptr;       // Node definition
		Tree*       mOwnerTree = nullptr;     // Owner tree
		Button*     mExpandBtn = nullptr;     // Node expanding button

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Updates expanding
		void UpdateTreeLayout(float dt);

		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object, moving hover of tree to this
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object, moving hover of tree to this
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when started dragging
		void OnDragStart(const Input::Cursor& cursor) override;

		// It is called when dragged
		void OnDragged(const Input::Cursor& cursor, DragDropArea* area) override;

		// It is called when dragging completed
		void OnDragEnd(const Input::Cursor& cursor) override;

		// It is called when this was selected
		void OnSelected();

		// It is called when this was unselected
		void OnDeselected();

		friend class Tree;
	};
}

CLASS_BASES_META(o2::Tree)
{
	BASE_CLASS(o2::ScrollArea);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::ISelectableDragableObjectsGroup);
}
END_META;
CLASS_FIELDS_META(o2::Tree)
{
	PUBLIC_FIELD(getObjectParentDelegate);
	PUBLIC_FIELD(getObjectChildrenDelegate);
	PUBLIC_FIELD(fillNodeDataByObjectDelegate);
	PUBLIC_FIELD(getDebugForObject);
	PUBLIC_FIELD(onNodeDoubleClicked);
	PUBLIC_FIELD(onNodeRightButtonClicked);
	PUBLIC_FIELD(onObjectsSelectionChanged);
	PUBLIC_FIELD(onDraggedObjects);
	PROTECTED_FIELD(mRearrangeType).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMultiSelectAvailable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mNodeWidgetSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mChildrenOffset).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsNeedUpdateView);
	PROTECTED_FIELD(mIsNeedUdateLayout);
	PROTECTED_FIELD(mIsNeedUpdateVisibleNodes);
	PROTECTED_FIELD(mAllNodes);
	PROTECTED_FIELD(mSelectedObjects);
	PROTECTED_FIELD(mSelectedNodes);
	PROTECTED_FIELD(mNodeWidgetsBuf);
	PROTECTED_FIELD(mNodesBuf);
	PROTECTED_FIELD(mVisibleNodes);
	PROTECTED_FIELD(mMinVisibleNodeIdx);
	PROTECTED_FIELD(mMaxVisibleNodeIdx);
	PROTECTED_FIELD(mLastClickPos);
	PROTECTED_FIELD(mHoveredItem);
	PROTECTED_FIELD(mHoverDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurrentHoverRect);
	PROTECTED_FIELD(mTargetHoverRect);
	PROTECTED_FIELD(mIsDraggingNodes);
	PROTECTED_FIELD(mFakeDragNode);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mInsertNodeCandidate);
	PROTECTED_FIELD(mBeforeDragSelectedItems);
	PROTECTED_FIELD(mDragEnded);
	PROTECTED_FIELD(mExpandedObjects);
	PROTECTED_FIELD(mExpandingNodeState);
	PROTECTED_FIELD(mExpandingNodeIdx);
	PROTECTED_FIELD(mExpandingNodeChildsCount);
	PROTECTED_FIELD(mExpandingNodePosition);
	PROTECTED_FIELD(mExpandingNodeBottomPosition);
	PROTECTED_FIELD(mExpandingNodeCurrCoef);
	PROTECTED_FIELD(mExpandingNodeCurrHeight);
	PROTECTED_FIELD(mExpandingNodeTargetHeight);
	PROTECTED_FIELD(mExpandNodeTime);
	PROTECTED_FIELD(mExpandingNodeFunc);
	PROTECTED_FIELD(mExpandNodeCandidate);
	PROTECTED_FIELD(mExpandInsertTime);
	PROTECTED_FIELD(mPressedTime);
	PROTECTED_FIELD(mNodeExpandTime).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mNodeDragIntoZone).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHighlightAnim).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHighlightSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHighlightLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHighlighNode);
	PROTECTED_FIELD(mHighlightObject);
	PROTECTED_FIELD(mVisibleWidgetsCache);
}
END_META;
CLASS_METHODS_META(o2::Tree)
{

	PUBLIC_FUNCTION(void, OnObjectCreated, UnknownPtr, UnknownPtr);
	PUBLIC_FUNCTION(void, OnObjectRemoved, UnknownPtr);
	PUBLIC_FUNCTION(void, OnObjectsChanged, const UnknownPtrsVec&);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
	PUBLIC_FUNCTION(void, UpdateNodesView, bool);
	PUBLIC_FUNCTION(TreeNode*, GetNode, UnknownPtr);
	PUBLIC_FUNCTION(void, ExpandAll);
	PUBLIC_FUNCTION(void, CollapseAll);
	PUBLIC_FUNCTION(Vector<UnknownPtr>, GetSelectedObjects);
	PUBLIC_FUNCTION(void, SetSelectedObjects, const UnknownPtrsVec&);
	PUBLIC_FUNCTION(void, SelectObject, UnknownPtr);
	PUBLIC_FUNCTION(void, SelectAndHightlightObject, UnknownPtr);
	PUBLIC_FUNCTION(void, DeselectObject, UnknownPtr);
	PUBLIC_FUNCTION(void, DeselectAllObjects);
	PUBLIC_FUNCTION(void, ScrollTo, UnknownPtr);
	PUBLIC_FUNCTION(void, ScrollToAndHightlight, UnknownPtr);
	PUBLIC_FUNCTION(void, ExpandParentObjects, UnknownPtr);
	PUBLIC_FUNCTION(TreeNode*, GetTreeNodeUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, SetRearrangeType, RearrangeType);
	PUBLIC_FUNCTION(RearrangeType, GetRearrangeType);
	PUBLIC_FUNCTION(void, SetMultipleSelectionAvailable, bool);
	PUBLIC_FUNCTION(bool, IsMultiSelectionAvailable);
	PUBLIC_FUNCTION(TreeNode*, GetNodeSample);
	PUBLIC_FUNCTION(void, SetNodeSample, TreeNode*);
	PUBLIC_FUNCTION(Sprite*, GetHoverDrawable);
	PUBLIC_FUNCTION(void, SetHoverLayout, const Layout&);
	PUBLIC_FUNCTION(Sprite*, GetHightlightDrawable);
	PUBLIC_FUNCTION(void, SetHightlightAnimation, const Animation&);
	PUBLIC_FUNCTION(void, SetHightlightLayout, const Layout&);
	PUBLIC_FUNCTION(void, SetNodeExpandTimer, float);
	PUBLIC_FUNCTION(float, GetNodeExpandTimer);
	PUBLIC_FUNCTION(void, SetChildsNodesOffset, float);
	PUBLIC_FUNCTION(float, GetChildsNodesOffset);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PROTECTED_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PROTECTED_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PROTECTED_FUNCTION(void, FillNodeDataByObject, TreeNode*, UnknownPtr);
	PROTECTED_FUNCTION(void, OnNodeDblClick, TreeNode*);
	PROTECTED_FUNCTION(void, OnNodeRBClick, TreeNode*);
	PROTECTED_FUNCTION(void, OnNodesSelectionChanged, UnknownPtrsVec);
	PROTECTED_FUNCTION(void, OnDraggedObjects, UnknownPtrsVec, UnknownPtr, UnknownPtr);
	PROTECTED_FUNCTION(SelectDragObjectsVec, GetSelectedDragObjects);
	PROTECTED_FUNCTION(SelectDragObjectsVec, GetAllObjects);
	PROTECTED_FUNCTION(void, Select, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, Select, SelectableDragableObject*, bool);
	PROTECTED_FUNCTION(void, Deselect, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, AddSelectableObject, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, RemoveSelectableObject, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, OnSelectableObjectCursorReleased, SelectableDragableObject*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelectableObjectBeganDragging, SelectableDragableObject*);
	PROTECTED_FUNCTION(bool, CheckMultipleSelection, const Vec2F&);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
	PROTECTED_FUNCTION(void, UpdateHighlighting, float);
	PROTECTED_FUNCTION(void, UpdatePressedNodeExpand, float);
	PROTECTED_FUNCTION(void, UpdateNodesStructure);
	PROTECTED_FUNCTION(int, InsertNodes, Node*, int, NodesVec*);
	PROTECTED_FUNCTION(void, RemoveNodes, Node*);
	PROTECTED_FUNCTION(Node*, CreateNode, UnknownPtr, Node*);
	PROTECTED_FUNCTION(void, UpdateVisibleNodes);
	PROTECTED_FUNCTION(void, CreateVisibleNodeWidget, Node*, int);
	PROTECTED_FUNCTION(void, UpdateNodeView, Node*, TreeNode*, int);
	PROTECTED_FUNCTION(void, UpdateNodeWidgetLayout, Node*, int);
	PROTECTED_FUNCTION(int, GetNodeIndex, float);
	PROTECTED_FUNCTION(float, GetNodePosition, int);
	PROTECTED_FUNCTION(void, ExpandNode, Node*);
	PROTECTED_FUNCTION(void, CollapseNode, Node*);
	PROTECTED_FUNCTION(void, StartExpandingAnimation, ExpandState, Node*, int);
	PROTECTED_FUNCTION(void, UpdateNodeExpanding, float);
	PROTECTED_FUNCTION(void, CalculateScrollArea);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, UpdateHover, TreeNode*);
	PROTECTED_FUNCTION(TreeNode*, CreateTreeNodeWidget);
	PROTECTED_FUNCTION(void, BeginDragging, TreeNode*);
	PROTECTED_FUNCTION(void, EndDragging, bool);
	PROTECTED_FUNCTION(void, UpdateDraggingGraphics);
	PROTECTED_FUNCTION(void, UpdateDraggingInsertion);
	PROTECTED_FUNCTION(void, UpdateDraggingInsertionAnim, float);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;

CLASS_BASES_META(o2::TreeNode)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::SelectableDragableObject);
}
END_META;
CLASS_FIELDS_META(o2::TreeNode)
{
	PROTECTED_FIELD(mNodeDef);
	PROTECTED_FIELD(mOwnerTree);
	PROTECTED_FIELD(mExpandBtn);
}
END_META;
CLASS_METHODS_META(o2::TreeNode)
{

	PUBLIC_FUNCTION(void, SetExpanded, bool, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, Expand, bool);
	PUBLIC_FUNCTION(void, Collapse, bool);
	PUBLIC_FUNCTION(UnknownPtr, GetObject);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateTreeLayout, float);
	PROTECTED_FUNCTION(void, OnCursorDblClicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnDragStart, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnDragged, const Input::Cursor&, DragDropArea*);
	PROTECTED_FUNCTION(void, OnDragEnd, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelected);
	PROTECTED_FUNCTION(void, OnDeselected);
}
END_META;
