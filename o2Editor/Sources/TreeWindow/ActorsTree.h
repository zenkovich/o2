#pragma once

#include "UI/Tree.h"

namespace o2
{
	class Actor;
	class UIToggleGroup;
}

using namespace o2;

namespace Editor
{
	// ------------------
	// Actors tree widget
	// ------------------
	class UIActorsTree: public UITree
	{
	public:
		typedef Vector<Actor*> ActorsVec;

	public:
		Function<void(ActorsVec, Actor*, Actor*)> onDraggedActors;
		Function<void(UITreeNode*, Actor*)>       onItemClick;
		Function<void(UITreeNode*, Actor*)>       onItemDblClick;
		Function<void(UITreeNode*, Actor*)>       onItemRBClick;
		Function<void(ActorsVec)>                 onItemsSelectionChanged;

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

		// Draws widget
		void Draw();

		// Updates widget
		void Update(float dt);

		// Rebuilds all tree
		void RebuildTree();

		// Updates tree node for object
		void UpdateTreeNode(Actor* object);

		// Returns ui node for object
		UITreeNode* GetNode(Actor* object);

		// Expands all nodes
		void ExpandAll();

		// Collapses all nodes
		void CollapseAll();

		// Forcible begins to dragging actors' nodes
		void ManualBeginDraggingActors(const ActorsVec& actors);

		// Updates manual actors dragging
		void ManualUpdateDraggingActors(const Input::Cursor& cursor);

		// Completed manual dragging actors
		void CompleteManualDraggingActors();

		// Breaks actors' nodes dragging
		void BreakDragging();

		// Returns selected objects vector
		ActorsVec GetSelectedActors() const;

		// Sets selected objects
		void SetSelectedActors(const ActorsVec& actors);

		// Selects object
		void SelectActor(Actor* object);

		// Selects object
		void SelectAndExpandActor(Actor* object);

		// Deselects object
		void DeselectActor(Actor* object);

		// Deselects all objects
		void DeselectAllActors();

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

		// Returns is listener scrollable
		bool IsScrollable() const;

		// Sets nodes pool resize count
		void SetNodesPoolResizeCount(int count);

		// Sets selection sprites pool resize count
		void SetSelectionSpritesPoolResizeCount(int count);

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

		SERIALIZABLE(UIActorsTree);

	protected:
		UIToggleGroup* mEnableActorsTogglesGroup; // Enable actors toggles group
		UIToggleGroup* mLockActorsTogglesGroup;	  // Lock actors toggles group
		bool           mAttackedToSceneEvents;    // Is tree attached to scene events

	protected:
		// Initializes widget logic
		void Initialize();

		// Returns actor's parent (For tree widget)
		UnknownType* GetActorsParent(UnknownType* obj);

		// Returns actor's children (For tree widget)
		Vector<UnknownType*> GetActorsChildren(UnknownType* parentObj);

		// Initializes tree node by actor (For tree widget)
		void SetupTreeNodeActor(UITreeNode* node, UnknownType* actorObj);

		// Moves actors to new parent and position, when they was dragged in tree (For tree widget)
		void RearrangeActors(Vector<UnknownType*> objects, UnknownType* parentObj, UnknownType* prevObj);

		// Calls when tree node double clicked (For tree widget)
		void OnTreeNodeDblClick(UITreeNode* node, Actor* actor);

		// Calls when enable actors toggle group pressed
		void EnableActorsGroupPressed(bool value);

		// Calls when enable actors toggle group released
		void EnableActorsGroupReleased(bool value);

		// Calls when lock actors toggle group pressed
		void LockActorsGroupPressed(bool value);

		// Calls when lock actors toggle group released
		void LockActorsGroupReleased(bool value);

		// Calls when actor was created
		void OnActorCreated(Actor* actor);

		// Calls when actor was destroyed
		void OnActorDestroyed(Actor* actor);

		// Updates nodes dragging
		void UpdateDragging(const Input::Cursor& cursor);

		// End nodes dragging
		void EndDragging();
	};
}