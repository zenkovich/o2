#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Utils/Singleton.h"

namespace o2
{
	class UIToggle;
	class UIEditBox;
	class UIButtonGroup;
	class UIToggleGroup;
	class UIContextMenu;
	class Actor;
}

using namespace o2;

// Editor actors tree access macros
#define o2EditorTree TreeWindow::Instance()

namespace Editor
{
	class UIActorsTree;

	// ------------------
	// Actors tree window
	// ------------------
	class TreeWindow: public IEditorWindow, public Singleton<TreeWindow>
	{
	public:
		IOBJECT(TreeWindow);

		// Returns actors tree widget
		UIActorsTree* GetActorsTree() const;

		// Expands all parent actor's nodes in actors tree
		void ExpandActorsTreeNode(Actor* targetActor);

	protected:
		UIToggle*      mListTreeToggle;			  // TOggle between list and tree views
		UIEditBox*     mSearchEditBox;			  // Search actors edit box
		UIActorsTree*  mActorsTree;				  // Main actors tree
		UIContextMenu* mTreeContextMenu;		  // Context menu

		bool           mInSearch = false;		  // True when searching actors (mSearchEditBox isn't empty)
		Vector<Actor*> mSearchActors;			  // Array of searched actors

	protected:
		// Default constructor
		TreeWindow();

		// Copy-constructor
		TreeWindow(const TreeWindow& other);

		// Destructor
		~TreeWindow();

		// Initializes window
		void InitializeWindow();

		// Calls after that all windows was created
		void PostInitializeWindow();

		// Calls when search button pressed
		void OnSearchPressed();

		// Calls when list-tree toggle changed value
		void OnListTreeToggled(bool value);

		// Calls when search field was changed
		void OnSearchEdited(const WString& searchStr);

		// Searches actors
		void SearchActorsRecursive(Actor* actor, const String& searchStr);

		// Calls when tree node clicked by right button (For tree widget)
		void OnTreeRBPressed(UITreeNode* node, Actor* actor);

		// Calls when some actor needs to created and registers actor insert action
		void CreateActor(Actor* newActor);

		// Calls when pressed "Create new" in context menu
		void OnContextCreateNewPressed();

		// Calls when pressed "Create sprite" in context menu
		void OnContextCreateSprite();

		// Calls when pressed "Create button" in context menu
		void OnContextCreateButton();

		// Calls when pressed "Copy" in context menu
		void OnContextCopyPressed();

		// Calls when pressed "Cut" in context menu
		void OnContextCutPressed();

		// Calls when pressed "Paste" in context menu
		void OnContextPastePressed();

		// Calls when pressed "Delete" in context menu
		void OnContextDeletePressed();

		// Calls when pressed "Duplicate" in context menu
		void OnContextDuplicatePressed();

		// Calls when pressed "Expand all" in context menu
		void OnContextExpandAllPressed();

		// Calls when pressed "Collapse all" in context menu
		void OnContextCollapseAllPressed();

		// Calls when pressed "Lock/unlock" in context menu
		void OnContextLockPressed();

		// Calls when pressed "Enable/disable" in context menu
		void OnContextEnablePressed();

		// Calls when actor was created
		void OnActorCreated(Actor* actor);

		// Calls when actor was destroyed
		void OnActorDestroyed(Actor* actor);

		friend class SceneEditWidget;
	};
}