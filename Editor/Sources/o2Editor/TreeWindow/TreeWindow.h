#pragma once

#include "o2/Utils/Singleton.h"
#include "o2/Utils/Editor/EditorScope.h"
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
	class SceneHierarchyTree;
	class DrawOrderTree;

	// -----------------------------------------------------------------
	// Actors tree window. Shows scene hierarchy tree or draw order tree
	// -----------------------------------------------------------------
	class TreeWindow: public IEditorWindow, public Singleton<TreeWindow>
	{
		IOBJECT(TreeWindow);

	public:
		// Returns actors tree widget
		SceneHierarchyTree* GetSceneTree() const;

		// Returns draw order tree widget
		DrawOrderTree* GetDrawOrderTree() const;

		// Enables focus on current tree
		void FocusTree();

		// Returns is current tree focused
		bool IsTreeFocused() const;

		// Sets selected objects
		void SetSelectedObjects(const Vector<SceneEditableObject*>& objects);

		// Updates current tree
		void UpdateTreeView();

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
		Toggle* mListTreeToggle = nullptr; // TOggle between list and tree views
		EditBox* mSearchEditBox = nullptr; // Search actors edit box

		SceneHierarchyTree* mSceneTree = nullptr; // Scene hierarchy tree

		DrawOrderTree* mDrawOrderTree = nullptr; // Draw order tree

		ContextMenu* mTreeContextMenu = nullptr;// Context menu

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

		// Initializes actors tree widget
		void InitializeSceneTree();

		// Initializes draw order tree widget
		void InitializeDrawOrderTree();

		// Initializes top panel with filter
		void InitializeTopPanel();

		// Initializes context menu 
		void InitializeContextMenu();

		// Gets all actor types and creates create sub menu
		void InitializeCreateMenu();

		// Creates creation menu items for UI style
		void InitUIStyleCreateMenu();

		// Creates creation menu for UI layers
		void InitUILayersCreateMenu();

		// Called after that all windows was created
		void PostInitializeWindow() override;

		// Called when search button pressed
		void OnSearchPressed();

		// Called when list-tree toggle changed value
		void OnListTreeToggled(bool value);

		// Called when search field was changed
		void OnSearchEdited(const WString& searchStr);

		// Searches actors
		void SearchObjectsRecursive(SceneEditableObject* object, const String& searchStr);

		// Called when tree node clicked by right button (For tree widget)
		void OnTreeRBPressed(TreeNode* node);

		template<typename _type>
		void CreateObject(const String& name);

		// Called when some object needs to be created and registers object insert action
		void OnCreateObject(SceneEditableObject* newObject);

		// Called when pressed "Create new" in context menu
		void OnContextCreateNewPressed();

		// Called when pressed "Create sprite" in context menu
		void OnContextCreateSprite();

		// Called when pressed "Create button" in context menu
		void OnContextCreateButton();

		// Called when pressed "Copy" in context menu
		void OnContextCopyPressed();

		// Called when pressed "Cut" in context menu
		void OnContextCutPressed();

		// Called when pressed "Paste" in context menu
		void OnContextPastePressed();

		// Called when pressed "Delete" in context menu
		void OnContextDeletePressed();

		// Called when pressed "Duplicate" in context menu
		void OnContextDuplicatePressed();

		// Called when pressed "Expand all" in context menu
		void OnContextExpandAllPressed();

		// Called when pressed "Collapse all" in context menu
		void OnContextCollapseAllPressed();

		// Called when pressed "Lock/unlock" in context menu
		void OnContextLockPressed();

		// Called when pressed "Enable/disable" in context menu
		void OnContextEnablePressed();

		// Called when view layers menu item toggled
		void OnViewLayersToggled(bool view);

		// Called when view internal children menu item toggled
		void OnViewInternalChildrenToggled(bool view);

		// Called when actor was created
		void OnActorCreated(SceneEditableObject* object);

		// Called when actor was destroyed
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
// --- META ---

CLASS_BASES_META(Editor::TreeWindow)
{
    BASE_CLASS(Editor::IEditorWindow);
    BASE_CLASS(o2::Singleton<TreeWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::TreeWindow)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mListTreeToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSearchEditBox);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSceneTree);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDrawOrderTree);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTreeContextMenu);
    FIELD().PROTECTED().NAME(mSearchObjects);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mInSearch);
}
END_META;
CLASS_METHODS_META(Editor::TreeWindow)
{

    FUNCTION().PUBLIC().SIGNATURE(SceneHierarchyTree*, GetSceneTree);
    FUNCTION().PUBLIC().SIGNATURE(DrawOrderTree*, GetDrawOrderTree);
    FUNCTION().PUBLIC().SIGNATURE(void, FocusTree);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsTreeFocused);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSelectedObjects, const Vector<SceneEditableObject*>&);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateTreeView);
    FUNCTION().PUBLIC().SIGNATURE(void, HighlightObjectTreeNode, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetWidgetsLayersVisible, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsWidgetsLayersVisible);
    FUNCTION().PUBLIC().SIGNATURE(void, SetWidgetsInternalChildrenVisible, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsWidgetsInternalChildrenVisible);
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const TreeWindow&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeSceneTree);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeDrawOrderTree);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeTopPanel);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeContextMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeCreateMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, InitUIStyleCreateMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, InitUILayersCreateMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, PostInitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSearchPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnListTreeToggled, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSearchEdited, const WString&);
    FUNCTION().PROTECTED().SIGNATURE(void, SearchObjectsRecursive, SceneEditableObject*, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTreeRBPressed, TreeNode*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCreateObject, SceneEditableObject*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCreateNewPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCreateSprite);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCreateButton);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCopyPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCutPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextPastePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextDeletePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextDuplicatePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextExpandAllPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCollapseAllPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextLockPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextEnablePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnViewLayersToggled, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnViewInternalChildrenToggled, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnActorCreated, SceneEditableObject*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnActorDestroyed, SceneEditableObject*);
}
END_META;
// --- END META ---
