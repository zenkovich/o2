#pragma once

#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

namespace o2
{
	class SceneEditableObject;
	class UIButtonGroup;
	class ContextMenu;
	class EditBox;
	class Toggle;
	class ToggleGroup;
}

using namespace o2;

// Editor actors tree access macros
#define o2EditorTree TreeWindow::Instance()

namespace Editor
{
	class SceneTree;

	// ------------------
	// Actors tree window
	// ------------------
	class TreeWindow: public IEditorWindow, public Singleton<TreeWindow>
	{
		IOBJECT(TreeWindow);

	public:
		// Returns actors tree widget
		SceneTree* GetSceneTree() const;

		// Expands all actor's parents nodes in tree and highlights actor
		void HighlightObjectTreeNode(SceneEditableObject* targetObject);

		// Sets widgets layers visibility in hierarchy
		void SetWidgetsLayersVisible(bool visible);

		// Returns widgets layers visibility in hierarchy
		bool IsWidgetsLayersVisible() const;

		// Sets widgets internal children visibility in hierarchy
		void SetWidgetsInternalChildrenVisible(bool visible);

		// Returns widgets internal children visibility in hierarchy
		bool IsWidgetsInternalChildrenVisible() const;

	protected:
		Toggle*      mListTreeToggle;		// TOggle between list and tree views
		EditBox*     mSearchEditBox;		// Search actors edit box
		SceneTree*   mSceneTree;			// Main actors tree
		ContextMenu* mTreeContextMenu;	// Context menu

		Vector<SceneEditableObject*> mSearchObjects;    // Array of searched objects
		bool                         mInSearch = false;	// True when searching objects (mSearchEditBox isn't empty)

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

		// Initializes test scene with test tags and actors
		void InitializeTestScene();

		// Initializes actors tree widget
		void InitializeSceneTree();

		// Initializes top panel with filter
		void InitializeTopPanel();

		// Initializes context menu 
		void InitializeContextMenu();

		// Gets all actor types and creates create sub menu
		void InitializeOtherCreateMenu();

		// Creates creation menu items for UI style
		void InitUIStyleCreateMenu();

		// Creates creation menu for UI layers
		void InitUILayersCreateMenu();

		// It is called after that all windows was created
		void PostInitializeWindow();

		// It is called when search button pressed
		void OnSearchPressed();

		// It is called when list-tree toggle changed value
		void OnListTreeToggled(bool value);

		// It is called when search field was changed
		void OnSearchEdited(const WString& searchStr);

		// Searches actors
		void SearchObjectsRecursive(SceneEditableObject* object, const String& searchStr);

		// It is called when tree node clicked by right button (For tree widget)
		void OnTreeRBPressed(TreeNode* node);

		template<typename _type>
		void CreateObject(const String& name);

		// It is called when some object needs to be created and registers object insert action
		void OnCreateObject(SceneEditableObject* newObject);

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

		// It is called when view layers menu item toggled
		void OnViewLayersToggled(bool view);

		// It is called when view internal children menu item toggled
		void OnViewInternalChildrenToggled(bool view);

		// It is called when actor was created
		void OnActorCreated(SceneEditableObject* object);

		// It is called when actor was destroyed
		void OnActorDestroyed(SceneEditableObject* object);

		friend class SceneEditWidget;
	};

	template<typename _type>
	void TreeWindow::CreateObject(const String& name)
	{
		ForcePopEditorScopeOnStack scope;

		SceneEditableObject* object = new _type();
		object->SetName(name);
		OnCreateObject(object);
	}

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
	PROTECTED_FIELD(mSceneTree);
	PROTECTED_FIELD(mTreeContextMenu);
	PROTECTED_FIELD(mSearchObjects);
	PROTECTED_FIELD(mInSearch).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(Editor::TreeWindow)
{

	PUBLIC_FUNCTION(SceneTree*, GetSceneTree);
	PUBLIC_FUNCTION(void, HighlightObjectTreeNode, SceneEditableObject*);
	PUBLIC_FUNCTION(void, SetWidgetsLayersVisible, bool);
	PUBLIC_FUNCTION(bool, IsWidgetsLayersVisible);
	PUBLIC_FUNCTION(void, SetWidgetsInternalChildrenVisible, bool);
	PUBLIC_FUNCTION(bool, IsWidgetsInternalChildrenVisible);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeTestScene);
	PROTECTED_FUNCTION(void, InitializeSceneTree);
	PROTECTED_FUNCTION(void, InitializeTopPanel);
	PROTECTED_FUNCTION(void, InitializeContextMenu);
	PROTECTED_FUNCTION(void, InitializeOtherCreateMenu);
	PROTECTED_FUNCTION(void, InitUIStyleCreateMenu);
	PROTECTED_FUNCTION(void, InitUILayersCreateMenu);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
	PROTECTED_FUNCTION(void, OnSearchPressed);
	PROTECTED_FUNCTION(void, OnListTreeToggled, bool);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, SearchObjectsRecursive, SceneEditableObject*, const String&);
	PROTECTED_FUNCTION(void, OnTreeRBPressed, TreeNode*);
	PROTECTED_FUNCTION(void, OnCreateObject, SceneEditableObject*);
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
	PROTECTED_FUNCTION(void, OnViewLayersToggled, bool);
	PROTECTED_FUNCTION(void, OnViewInternalChildrenToggled, bool);
	PROTECTED_FUNCTION(void, OnActorCreated, SceneEditableObject*);
	PROTECTED_FUNCTION(void, OnActorDestroyed, SceneEditableObject*);
}
END_META;
