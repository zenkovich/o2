#pragma once

#include "Scene/UI/Widgets/Tree.h"

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
		typedef Vector<SceneEditableObject*> SceneEditableObjsVec;

	public:
		Function<void(SceneEditableObjsVec)> onObjectsSelectionChanged;

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
		UITreeNode* GetNode(SceneEditableObject* object);

		// Returns selected objects vector
		SceneEditableObjsVec GetSelectedObjects() const;

		// Sets selected objects
		void SetSelectedObjects(const SceneEditableObjsVec& objects);

		// Selects object
		void SelectObject(SceneEditableObject* object);

		// Selects object
		void SelectAndHightlightObject(SceneEditableObject* object);

		// Scrolls view to object and highlights
		void ScrollToAndHightlight(SceneEditableObject* object);

		// Deselects object
		void DeselectObject(SceneEditableObject* object);

		// Deselects all objects
		void DeselectAllObjects();

		// Scrolls view to object's tree node
		void ScrollTo(SceneEditableObject* object);

		// Sets watching editor UI state
		void SetEditorWatching(bool watching);

		SERIALIZABLE(SceneTree);

	protected:
		ToggleGroup*     mEnableTogglesGroup;    // Enable objects toggles group
		ToggleGroup*     mLockTogglesGroup;	   // Lock objects toggles group
		bool               mAttachedToSceneEvents; // Is tree attached to scene events
						    						    
		ActorProperty*     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
		ComponentProperty* mDragComponentPropertyField; // Component property field under cursor when dragging actor

		bool               mWatchEditor = false;

	protected:
		// Initializes widget logic
		void Initialize();

		// Updates visible nodes (calculates range and initializes nodes), enables editor mode
		void UpdateVisibleNodes() override;

		// Gets tree node from pool or creates new, enables editor mode
		UITreeNode* CreateTreeNodeWidget() override;

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object) override;

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object) override;

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object) override;

		// It is called when tree node was double clicked
		void OnNodeDblClick(UITreeNode* nodeWidget) override;

		// It is called when objects was dragged in new parent in position next of prevObject
		void OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject) override;

		// It is called when object was created
		void OnObjectCreated(SceneEditableObject* object);

		// It is called when object was destroyed
		void OnObjectDestroing(SceneEditableObject* object);

		// It is called when some objects were changed
		void OnObjectsChanged(const SceneEditableObjsVec& objects);

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
		void OnNodesSelectionChanged(UnknownPtrsVec objects) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		friend class UISceneTreeNode;
	};

	// ---------------
	// Scene tree node
	// ---------------
	class UISceneTreeNode: public UITreeNode
	{
	public:
		// Default constructor
		UISceneTreeNode();

		// Copy-constructor
		UISceneTreeNode(const UISceneTreeNode& other);

		// Copy operator
		UISceneTreeNode& operator=(const UISceneTreeNode& other);

		// Sets object and updates content
		void SetSceneObject(SceneEditableObject* object);

		// Enables edit name edit box
		void EnableEditName();

		SERIALIZABLE(UISceneTreeNode);

	protected:
		SceneEditableObject* mTargetObject = nullptr; // Target object

		Text*                mNameDrawable = nullptr; // Object name drawable
		Toggle*            mLockToggle = nullptr;   // Lock toggle
		Toggle*            mEnableToggle = nullptr; // Enable toggle
		Button*            mLinkBtn = nullptr;      // View link button
		EditBox*           mNameEditBox = nullptr;  // Object's name edit box
		WidgetState*       mEditState = nullptr;    // Object's name edit state

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

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
	PUBLIC_FIELD(onObjectsSelectionChanged);
	PROTECTED_FIELD(mEnableTogglesGroup);
	PROTECTED_FIELD(mLockTogglesGroup);
	PROTECTED_FIELD(mAttachedToSceneEvents);
	PROTECTED_FIELD(mDragActorPropertyField);
	PROTECTED_FIELD(mDragComponentPropertyField);
	PROTECTED_FIELD(mWatchEditor);
}
END_META;
CLASS_METHODS_META(Editor::SceneTree)
{

	PUBLIC_FUNCTION(void, AttachToSceneEvents);
	PUBLIC_FUNCTION(void, DeattachFromSceneEvents);
	PUBLIC_FUNCTION(UITreeNode*, GetNode, SceneEditableObject*);
	PUBLIC_FUNCTION(SceneEditableObjsVec, GetSelectedObjects);
	PUBLIC_FUNCTION(void, SetSelectedObjects, const SceneEditableObjsVec&);
	PUBLIC_FUNCTION(void, SelectObject, SceneEditableObject*);
	PUBLIC_FUNCTION(void, SelectAndHightlightObject, SceneEditableObject*);
	PUBLIC_FUNCTION(void, ScrollToAndHightlight, SceneEditableObject*);
	PUBLIC_FUNCTION(void, DeselectObject, SceneEditableObject*);
	PUBLIC_FUNCTION(void, DeselectAllObjects);
	PUBLIC_FUNCTION(void, ScrollTo, SceneEditableObject*);
	PUBLIC_FUNCTION(void, SetEditorWatching, bool);
	PROTECTED_FUNCTION(void, Initialize);
	PROTECTED_FUNCTION(void, UpdateVisibleNodes);
	PROTECTED_FUNCTION(UITreeNode*, CreateTreeNodeWidget);
	PROTECTED_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PROTECTED_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PROTECTED_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PROTECTED_FUNCTION(void, FillNodeDataByObject, UITreeNode*, UnknownPtr);
	PROTECTED_FUNCTION(void, OnNodeDblClick, UITreeNode*);
	PROTECTED_FUNCTION(void, OnDraggedObjects, UnknownPtrsVec, UnknownPtr, UnknownPtr);
	PROTECTED_FUNCTION(void, OnObjectCreated, SceneEditableObject*);
	PROTECTED_FUNCTION(void, OnObjectDestroing, SceneEditableObject*);
	PROTECTED_FUNCTION(void, OnObjectsChanged, const SceneEditableObjsVec&);
	PROTECTED_FUNCTION(void, OnObjectChanged, SceneEditableObject*);
	PROTECTED_FUNCTION(void, EnableObjectsGroupPressed, bool);
	PROTECTED_FUNCTION(void, EnableObjectsGroupReleased, bool);
	PROTECTED_FUNCTION(void, LockObjectsGroupPressed, bool);
	PROTECTED_FUNCTION(void, LockObjectsGroupReleased, bool);
	PROTECTED_FUNCTION(void, OnNodesSelectionChanged, UnknownPtrsVec);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
}
END_META;

CLASS_BASES_META(Editor::UISceneTreeNode)
{
	BASE_CLASS(o2::UITreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::UISceneTreeNode)
{
	PROTECTED_FIELD(mTargetObject);
	PROTECTED_FIELD(mNameDrawable);
	PROTECTED_FIELD(mLockToggle);
	PROTECTED_FIELD(mEnableToggle);
	PROTECTED_FIELD(mLinkBtn);
	PROTECTED_FIELD(mNameEditBox);
	PROTECTED_FIELD(mEditState);
}
END_META;
CLASS_METHODS_META(Editor::UISceneTreeNode)
{

	PUBLIC_FUNCTION(void, SetSceneObject, SceneEditableObject*);
	PUBLIC_FUNCTION(void, EnableEditName);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnLockClicked);
	PROTECTED_FUNCTION(void, OnEnableCkicked);
	PROTECTED_FUNCTION(void, OnObjectNameChanged, const WString&);
}
END_META;
