#pragma once

#include "o2/Scene/UI/Widgets/Tree.h"

namespace o2
{
	class Actor;
	class SceneEditableObject;
	class Button;
	class EditBox;
	class Toggle;
	class ToggleGroup;
}

using namespace o2;

namespace Editor
{
	class ActorProperty;
	class ComponentProperty;

	// -----------------
	// Scene tree widget
	// -----------------
	class SceneTree: public Tree
	{
	public:
		Function<void(Vector<SceneEditableObject*>)> onObjectsSelectionChanged;

		// Default constructor
		SceneTree();

		// Copy-constructor
		SceneTree(const SceneTree& other);

		// Destructor
		~SceneTree();

		// Copy-operator
		SceneTree& operator=(const SceneTree& other);

		// Attaches to scene events
		void AttachToSceneEvents();

		// Deattaches from scene events
		void DeattachFromSceneEvents();

		// Returns ui node for object
		TreeNode* GetNode(SceneEditableObject* object);

		// Returns selected objects vector
		Vector<SceneEditableObject*> GetSelectedObjects() const;

		// Sets selected objects
		void SetSelectedObjects(const Vector<SceneEditableObject*>& objects);

		// Selects object
		void SelectObject(SceneEditableObject* object);

		// Selects object
		void SelectAndHighlightObject(SceneEditableObject* object);

		// Scrolls view to object and highlights
		void ScrollToAndHighlight(SceneEditableObject* object);

		// Deselects object
		void DeselectObject(SceneEditableObject* object);

		// Deselects all objects
		void DeselectAllObjects();

		// Scrolls view to object's tree node
		void ScrollTo(SceneEditableObject* object);

		// Sets watching editor UI state
		void SetEditorWatching(bool watching);

		// Returns is watching editor UI state
		bool IsEditorWatching() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(SceneTree);

	protected:
		ToggleGroup* mEnableTogglesGroup;    // Enable objects toggles group
		ToggleGroup* mLockTogglesGroup;	     // Lock objects toggles group
		bool         mAttachedToSceneEvents; // Is tree attached to scene events
						    						    
		ActorProperty*     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
		ComponentProperty* mDragComponentPropertyField; // Component property field under cursor when dragging actor

		bool mWatchEditor = false;

	protected:
		// Initializes widget logic
		void Initialize();

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

		// It is called when tree node was double clicked
		void OnNodeDblClick(TreeNode* nodeWidget) override;

		// It is called when objects was dragged in new parent in position next of prevObject
		void OnDraggedObjects(Vector<void*> objects, void* newParent, void* prevObject) override;

		// It is called when object was created
		void OnObjectCreated(SceneEditableObject* object);

		// It is called when object was destroyed
		void OnObjectDestroing(SceneEditableObject* object);

		// It is called when some objects were changed
		void OnObjectsChanged(const Vector<SceneEditableObject*>& objects);

		// It is called when object was changed
		void OnObjectChanged(SceneEditableObject* object);

		// It is called when enable objects toggle group pressed
		void EnableObjectsGroupPressed(bool value);

		// It is called when enable objects toggle group released
		void EnableObjectsGroupReleased(bool value);

		// It is called when lock objects toggle group pressed
		void LockObjectsGroupPressed(bool value);

		// It is called when lock objects toggle group released
		void LockObjectsGroupReleased(bool value);

		// It is called when list of selected objects was changed
		void OnNodesSelectionChanged(Vector<void*> objects) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		friend class SceneTreeNode;
	};

	// ---------------
	// Scene tree node
	// ---------------
	class SceneTreeNode: public TreeNode
	{
	public:
		// Default constructor
		SceneTreeNode();

		// Copy-constructor
		SceneTreeNode(const SceneTreeNode& other);

		// Copy operator
		SceneTreeNode& operator=(const SceneTreeNode& other);

		// Sets object and updates content
		void SetSceneObject(SceneEditableObject* object);

		// Enables edit name edit box
		void EnableEditName();

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(SceneTreeNode);

	protected:
		SceneEditableObject* mTargetObject = nullptr; // Target object
													    
		Toggle*      mLockToggle = nullptr;              // Lock toggle
		WidgetState* mLockToggleLockedState = nullptr;   // Lock toggle locked state
		WidgetState* mLockToggleHalfHideState = nullptr; // Lock toggle half hide state

		Toggle* mEnableToggle = nullptr; // Enable toggle

		Button*      mLinkBtn = nullptr;              // View link button
		WidgetState* mLinkBtnHalfHideState = nullptr; // View link button half hide state

		Text*        mNameDrawable = nullptr; // Object name drawable
		EditBox*     mNameEditBox = nullptr;  // Object's name edit box
		WidgetState* mEditState = nullptr;    // Object's name edit state

	protected:
		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataValue& node) override;

		// initializes controls and widgets
		void InitializeControls();

		// It is called when lock toggle was clicked and changes target object's lock state
		void OnLockClicked();

		// It is called when enable toggle was clicked and changes target object's enable state
		void OnEnableCkicked();

		// It is called when object name edit box changed
		void OnObjectNameChanged(const WString& text);

		friend class SceneTree;
	};
}

CLASS_BASES_META(Editor::SceneTree)
{
	BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::SceneTree)
{
	FIELD().PUBLIC().NAME(onObjectsSelectionChanged);
	FIELD().PROTECTED().NAME(mEnableTogglesGroup);
	FIELD().PROTECTED().NAME(mLockTogglesGroup);
	FIELD().PROTECTED().NAME(mAttachedToSceneEvents);
	FIELD().PROTECTED().NAME(mDragActorPropertyField);
	FIELD().PROTECTED().NAME(mDragComponentPropertyField);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWatchEditor);
}
END_META;
CLASS_METHODS_META(Editor::SceneTree)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const SceneTree&);
	FUNCTION().PUBLIC().SIGNATURE(void, AttachToSceneEvents);
	FUNCTION().PUBLIC().SIGNATURE(void, DeattachFromSceneEvents);
	FUNCTION().PUBLIC().SIGNATURE(TreeNode*, GetNode, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetSelectedObjects);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSelectedObjects, const Vector<SceneEditableObject*>&);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectObject, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectAndHighlightObject, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, ScrollToAndHighlight, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, DeselectObject, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, DeselectAllObjects);
	FUNCTION().PUBLIC().SIGNATURE(void, ScrollTo, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEditorWatching, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEditorWatching);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, Initialize);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateVisibleNodes);
	FUNCTION().PROTECTED().SIGNATURE(TreeNode*, CreateTreeNodeWidget);
	FUNCTION().PROTECTED().SIGNATURE(void*, GetObjectParent, void*);
	FUNCTION().PROTECTED().SIGNATURE(Vector<void*>, GetObjectChilds, void*);
	FUNCTION().PROTECTED().SIGNATURE(String, GetObjectDebug, void*);
	FUNCTION().PROTECTED().SIGNATURE(void, FillNodeDataByObject, TreeNode*, void*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnNodeDblClick, TreeNode*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDraggedObjects, Vector<void*>, void*, void*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectCreated, SceneEditableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectDestroing, SceneEditableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectsChanged, const Vector<SceneEditableObject*>&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectChanged, SceneEditableObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, EnableObjectsGroupPressed, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, EnableObjectsGroupReleased, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, LockObjectsGroupPressed, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, LockObjectsGroupReleased, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnNodesSelectionChanged, Vector<void*>);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragExit, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, ISelectableDragableObjectsGroup*);
}
END_META;

CLASS_BASES_META(Editor::SceneTreeNode)
{
	BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::SceneTreeNode)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTargetObject);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockToggleLockedState);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLockToggleHalfHideState);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEnableToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLinkBtn);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLinkBtnHalfHideState);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mNameDrawable);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mNameEditBox);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditState);
}
END_META;
CLASS_METHODS_META(Editor::SceneTreeNode)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const SceneTreeNode&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSceneObject, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, EnableEditName);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLockClicked);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableCkicked);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectNameChanged, const WString&);
}
END_META;
