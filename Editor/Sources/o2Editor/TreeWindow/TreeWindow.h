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
		void InitializeCreateMenu();

		// Creates creation menu items for UI style
		void InitUIStyleCreateMenu();

		// Creates creation menu for UI layers
		void InitUILayersCreateMenu();

		// It is called after that all windows was created
		void PostInitializeWindow() override;

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
	FIELD().PROTECTED().NAME(mListTreeToggle);
	FIELD().PROTECTED().NAME(mSearchEditBox);
	FIELD().PROTECTED().NAME(mSceneTree);
	FIELD().PROTECTED().NAME(mTreeContextMenu);
	FIELD().PROTECTED().NAME(mSearchObjects);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mInSearch);
}
END_META;
CLASS_METHODS_META(Editor::TreeWindow)
{

	FUNCTION().PUBLIC().SIGNATURE(SceneTree*, GetSceneTree);
	FUNCTION().PUBLIC().SIGNATURE(void, HighlightObjectTreeNode, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWidgetsLayersVisible, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsWidgetsLayersVisible);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWidgetsInternalChildrenVisible, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsWidgetsInternalChildrenVisible);
	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const TreeWindow&);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeTestScene);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeSceneTree);
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
