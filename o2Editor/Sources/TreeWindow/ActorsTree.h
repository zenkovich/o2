#pragma once

#include "UI/Tree.h"

namespace o2
{
	class Actor;
	class UIButton;
	class UIEditBox;
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

		SERIALIZABLE(UIActorsTree);

	protected:
		UIToggleGroup*     mEnableActorsTogglesGroup;   // Enable actors toggles group
		UIToggleGroup*     mLockActorsTogglesGroup;	    // Lock actors toggles group
		bool               mAttackedToSceneEvents;      // Is tree attached to scene events
						    						    
		ActorProperty*     mDragActorPropertyField;     // Actor property field under cursor when dragging actor
		ComponentProperty* mDragComponentPropertyField; // Component property field under cursor when dragging actor

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
