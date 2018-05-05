#pragma once

#include "UI/Tree.h"

namespace o2
{
	class Actor;
	class UIButton;
	class UIEditBox;
	class UIToggle;
	class UIToggleGroup;
}

using namespace o2;

namespace Editor
{
	class ActorProperty;
	class ComponentProperty;

	// ------------------
	// Actors tree widget
	// ------------------
	class UIActorsTree: public UITree
	{
	public:
		typedef Vector<Actor*> ActorsVec;

	public:
		Function<void(ActorsVec)> onObjectsSelectionChanged;

		// Default constructor
		UIActorsTree();

		// Copy-constructor
		UIActorsTree(const UIActorsTree& other);

		// Destructor
		~UIActorsTree();

		// Copy-operator
		UIActorsTree& operator=(const UIActorsTree& other);

		// Attaches to scene events
		void AttachToSceneEvents();

		// Deattaches from scene events
		void DeattachFromSceneEvents();

		// Returns ui node for object
		UITreeNode* GetNode(Actor* object);

		// Returns selected objects vector
		ActorsVec GetSelectedActors() const;

		// Sets selected objects
		void SetSelectedActors(const ActorsVec& actors);

		// Selects object
		void SelectActor(Actor* object);

		// Selects object
		void SelectAndHightlightActor(Actor* object);

		// Scrolls view to object and hightlights
		void ScrollToAndHightlight(Actor* object);

		// Deselects object
		void DeselectActor(Actor* object);

		// Deselects all objects
		void DeselectAllActors();

		// Scrolls view to actor's tree node
		void ScrollTo(Actor* object);

		// Sets wathing editor UI state
		void SetEditorWatching(bool watching);

		SERIALIZABLE(UIActorsTree);

	protected:
		UIToggleGroup*     mEnableActorsTogglesGroup;   // Enable actors toggles group
		UIToggleGroup*     mLockActorsTogglesGroup;	    // Lock actors toggles group
		bool               mAttackedToSceneEvents;      // Is tree attached to scene events
						    						    
		ActorProperty*     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
		ComponentProperty* mDragComponentPropertyField; // Component property field under cursor when dragging actor

		bool               mWatchEditor = true;

	protected:
		// Initializes widget logic
		void Initialize();

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object);

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object);

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object);

		// Sets nodeWidget data by object
		void FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object);

		// It is called when tree node was double clicked
		void OnNodeDblClick(UITreeNode* nodeWidget);

		// It is called when objects was dragged in new parent in position next of prevObject
		void OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject);

		// It is called when actor was created
		void OnActorCreated(Actor* actor);

		// It is called when actor was destroyed
		void OnActorDestroyed(Actor* actor);

		// It is called when some actors were changed
		void OnActorsChanged(const ActorsVec& actors);

		// It is called when actor was changed
		void OnActorChanged(Actor* actor);

		// It is called when enable actors toggle group pressed
		void EnableActorsGroupPressed(bool value);

		// It is called when enable actors toggle group released
		void EnableActorsGroupReleased(bool value);

		// It is called when lock actors toggle group pressed
		void LockActorsGroupPressed(bool value);

		// It is called when lock actors toggle group released
		void LockActorsGroupReleased(bool value);

		// It is called when list of selected objects was changed
		void OnNodesSelectionChanged(UnknownPtrsVec objects);

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group);

		// It is called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group);

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

		friend class UIActorsTreeNode;
	};

	// ----------------
	// Actors tree node
	// ----------------
	class UIActorsTreeNode: public UITreeNode
	{
	public:
		// Default constructor
		UIActorsTreeNode();

		// Copy-constructor
		UIActorsTreeNode(const UIActorsTreeNode& other);

		// Copy operator
		UIActorsTreeNode& operator=(const UIActorsTreeNode& other);

		// Sets actor and updates content
		void SetActor(Actor* actor);

		// Enables edit name edit box
		void EnableEditName();

		SERIALIZABLE(UIActorsTreeNode);

	protected:
		Actor*         mTargetActor = nullptr;  // target actor

		Text*          mNameDrawable = nullptr; // Actor name drawable
		UIToggle*      mLockToggle = nullptr;   // Lock toggle
		UIToggle*      mEnableToggle = nullptr; // Enable toggle
		UIButton*      mLinkBtn = nullptr;      // View link button
		UIEditBox*     mNameEditBox = nullptr;  // Actor's name edit box
		UIWidgetState* mEditState = nullptr;    // Actor's name edit state

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializs controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();

		// It is called when lock toggle was clicked and changes target actor's lock state
		void OnLockClicked();

		// It is called when enable toggle was clicked and changes target actor's enable state
		void OnEnableCkicked();

		// It is called when actor name edit box changed
		void OnActorNameChanged(const WString& text);

		friend class UIActorsTree;
	};
}

CLASS_BASES_META(Editor::UIActorsTree)
{
	BASE_CLASS(o2::UITree);
}
END_META;
CLASS_FIELDS_META(Editor::UIActorsTree)
{
	PUBLIC_FIELD(onObjectsSelectionChanged);
	PROTECTED_FIELD(mEnableActorsTogglesGroup);
	PROTECTED_FIELD(mLockActorsTogglesGroup);
	PROTECTED_FIELD(mAttackedToSceneEvents);
	PROTECTED_FIELD(mDragActorPropertyField);
	PROTECTED_FIELD(mDragComponentPropertyField);
	PROTECTED_FIELD(mWatchEditor);
}
END_META;
CLASS_METHODS_META(Editor::UIActorsTree)
{

	PUBLIC_FUNCTION(void, AttachToSceneEvents);
	PUBLIC_FUNCTION(void, DeattachFromSceneEvents);
	PUBLIC_FUNCTION(UITreeNode*, GetNode, Actor*);
	PUBLIC_FUNCTION(ActorsVec, GetSelectedActors);
	PUBLIC_FUNCTION(void, SetSelectedActors, const ActorsVec&);
	PUBLIC_FUNCTION(void, SelectActor, Actor*);
	PUBLIC_FUNCTION(void, SelectAndHightlightActor, Actor*);
	PUBLIC_FUNCTION(void, ScrollToAndHightlight, Actor*);
	PUBLIC_FUNCTION(void, DeselectActor, Actor*);
	PUBLIC_FUNCTION(void, DeselectAllActors);
	PUBLIC_FUNCTION(void, ScrollTo, Actor*);
	PUBLIC_FUNCTION(void, SetEditorWatching, bool);
	PROTECTED_FUNCTION(void, Initialize);
	PROTECTED_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PROTECTED_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PROTECTED_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PROTECTED_FUNCTION(void, FillNodeDataByObject, UITreeNode*, UnknownPtr);
	PROTECTED_FUNCTION(void, OnNodeDblClick, UITreeNode*);
	PROTECTED_FUNCTION(void, OnDraggedObjects, UnknownPtrsVec, UnknownPtr, UnknownPtr);
	PROTECTED_FUNCTION(void, OnActorCreated, Actor*);
	PROTECTED_FUNCTION(void, OnActorDestroyed, Actor*);
	PROTECTED_FUNCTION(void, OnActorsChanged, const ActorsVec&);
	PROTECTED_FUNCTION(void, OnActorChanged, Actor*);
	PROTECTED_FUNCTION(void, EnableActorsGroupPressed, bool);
	PROTECTED_FUNCTION(void, EnableActorsGroupReleased, bool);
	PROTECTED_FUNCTION(void, LockActorsGroupPressed, bool);
	PROTECTED_FUNCTION(void, LockActorsGroupReleased, bool);
	PROTECTED_FUNCTION(void, OnNodesSelectionChanged, UnknownPtrsVec);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
}
END_META;

CLASS_BASES_META(Editor::UIActorsTreeNode)
{
	BASE_CLASS(o2::UITreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::UIActorsTreeNode)
{
	PROTECTED_FIELD(mTargetActor);
	PROTECTED_FIELD(mNameDrawable);
	PROTECTED_FIELD(mLockToggle);
	PROTECTED_FIELD(mEnableToggle);
	PROTECTED_FIELD(mLinkBtn);
	PROTECTED_FIELD(mNameEditBox);
	PROTECTED_FIELD(mEditState);
}
END_META;
CLASS_METHODS_META(Editor::UIActorsTreeNode)
{

	PUBLIC_FUNCTION(void, SetActor, Actor*);
	PUBLIC_FUNCTION(void, EnableEditName);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnLockClicked);
	PROTECTED_FUNCTION(void, OnEnableCkicked);
	PROTECTED_FUNCTION(void, OnActorNameChanged, const WString&);
}
END_META;
