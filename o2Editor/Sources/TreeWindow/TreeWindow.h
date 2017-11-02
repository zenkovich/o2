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
		IOBJECT(TreeWindow);

	public:
		// Returns actors tree widget
		UIActorsTree* GetActorsTree() const;

		// Expands all actor's parents nodes in tree and hightlights actor
		void HightlightActorsTreeNode(Actor* targetActor);

	protected:
		UIToggle*      mListTreeToggle;		// TOggle between list and tree views
		UIEditBox*     mSearchEditBox;		// Search actors edit box
		UIActorsTree*  mActorsTree;			// Main actors tree
		UIContextMenu* mTreeContextMenu;	// Context menu

		bool           mInSearch = false;	// True when searching actors (mSearchEditBox isn't empty)
		Vector<Actor*> mSearchActors;		// Array of searched actors

	public:
		// Default constructor
		TreeWindow();

		// Copy-constructor
		TreeWindow(const TreeWindow& other);

		// Destructor
		~TreeWindow();

	protected:
		// Initializes window
		void InitializeWindow();

		// It is called after that all windows was created
		void PostInitializeWindow();

		// It is called when search button pressed
		void OnSearchPressed();

		// It is called when list-tree toggle changed value
		void OnListTreeToggled(bool value);

		// It is called when search field was changed
		void OnSearchEdited(const WString& searchStr);

		// Searches actors
		void SearchActorsRecursive(Actor* actor, const String& searchStr);

		// It is called when tree node clicked by right button (For tree widget)
		void OnTreeRBPressed(UITreeNode* node);

		// It is called when some actor needs to created and registers actor insert action
		void CreateActor(Actor* newActor);

		// It is called when pressed "Create new" in context menu
		void OnContextCreateNewPressed();

		// It is called when pressed "Create sprite" in context menu
		void OnContextCreateSprite();

		// It is called when pressed "Create button" in context menu
		void OnContextCreateButton();

		// It is called when pressed "Copy" in context menu
		void OnContextCopyPressed();

		// It is called when pressed "Cut" in context menu
		void OnContextCutPressed();

		// It is called when pressed "Paste" in context menu
		void OnContextPastePressed();

		// It is called when pressed "Delete" in context menu
		void OnContextDeletePressed();

		// It is called when pressed "Duplicate" in context menu
		void OnContextDuplicatePressed();

		// It is called when pressed "Expand all" in context menu
		void OnContextExpandAllPressed();

		// It is called when pressed "Collapse all" in context menu
		void OnContextCollapseAllPressed();

		// It is called when pressed "Lock/unlock" in context menu
		void OnContextLockPressed();

		// It is called when pressed "Enable/disable" in context menu
		void OnContextEnablePressed();

		// It is called when actor was created
		void OnActorCreated(Actor* actor);

		// It is called when actor was destroyed
		void OnActorDestroyed(Actor* actor);

		friend class SceneEditWidget;
	};
}

CLASS_BASES_META(Editor::TreeWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::Singleton<TreeWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::TreeWindow)
{
	PROTECTED_FIELD(mListTreeToggle);
	PROTECTED_FIELD(mSearchEditBox);
	PROTECTED_FIELD(mActorsTree);
	PROTECTED_FIELD(mTreeContextMenu);
	PROTECTED_FIELD(mInSearch);
	PROTECTED_FIELD(mSearchActors);
}
END_META;
CLASS_METHODS_META(Editor::TreeWindow)
{

	PUBLIC_FUNCTION(UIActorsTree*, GetActorsTree);
	PUBLIC_FUNCTION(void, HightlightActorsTreeNode, Actor*);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
	PROTECTED_FUNCTION(void, OnSearchPressed);
	PROTECTED_FUNCTION(void, OnListTreeToggled, bool);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, SearchActorsRecursive, Actor*, const String&);
	PROTECTED_FUNCTION(void, OnTreeRBPressed, UITreeNode*);
	PROTECTED_FUNCTION(void, CreateActor, Actor*);
	PROTECTED_FUNCTION(void, OnContextCreateNewPressed);
	PROTECTED_FUNCTION(void, OnContextCreateSprite);
	PROTECTED_FUNCTION(void, OnContextCreateButton);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextDuplicatePressed);
	PROTECTED_FUNCTION(void, OnContextExpandAllPressed);
	PROTECTED_FUNCTION(void, OnContextCollapseAllPressed);
	PROTECTED_FUNCTION(void, OnContextLockPressed);
	PROTECTED_FUNCTION(void, OnContextEnablePressed);
	PROTECTED_FUNCTION(void, OnActorCreated, Actor*);
	PROTECTED_FUNCTION(void, OnActorDestroyed, Actor*);
}
END_META;
