#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

namespace o2
{
	class UIToggle;
	class UIEditBox;
	class UITree;
	class UIButtonGroup;
	class UIToggleGroup;
	class UIContextMenu;
	class Actor;
}

using namespace o2;

// ------------------
// Actors tree window
// ------------------
class TreeWindow: public IEditorWindow
{
public:
	IOBJECT(TreeWindow);

protected:
	UIToggle*      mListTreeToggle;			  // TOggle between list and tree views
	UIEditBox*     mSearchEditBox;			  // Search actors edit box
	UITree*        mActorsTree;				  // Main actors tree
	UIToggleGroup* mEnableActorsTogglesGroup; // Enable actors toggles group
	UIToggleGroup* mLockActorsTogglesGroup;	  // Lock actors toggles group
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

	// Calls when search button pressed
	void OnSearchPressed();

	// Calls when list-tree toggle changed value
	void OnListTreeToggled(bool value);

	// Calls when search field was changed
	void OnSearchEdited(const WString& searchStr);

	// Searches actors
	void SearchActorsRecursive(Actor* actor, const String& searchStr);

	// Returns actor's parent (For tree widget)
	UnknownType* GetActorsParent(UnknownType* obj);

	// Returns actor's children (For tree widget)
	Vector<UnknownType*> GetActorsChildren(UnknownType* parentObj);

	// Initializes tree node by actor (For tree widget)
	void SetupTreeNodeActor(UITreeNode* node, UnknownType* actorObj);

	// Moves actors to new parent and position, when they was dragged in tree (For tree widget)
	void RearrangeActors(Vector<UnknownType*> objects, UnknownType* parentObj, UnknownType* prevObj);

	// Calls when tree node double clicked (For tree widget)
	void OnTreeNodeDblClick(UITreeNode* node);

	// Calls when tree node clicked by right button (For tree widget)
	void OnTreeRBPressed(UITreeNode* node);

	void OnContextCreateNewPressed();
	void OnContextCreateSprite();
	void OnContextCreateButton();
	void OnContextCopyPressed();
	void OnContextCutPressed();
	void OnContextPastePressed();
	void OnContextDeletePressed();
	void OnContextDuplicatePressed();
	void OnContextExpandAllPressed();
	void OnContextCollapseAllPressed();
	void OnContextLockPressed();
	void OnContextEnablePressed();
};
