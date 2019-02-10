#pragma once

#include "Events/KeyboardEventsListener.h"
#include "Scene/UI/Widgets/Tree.h"
#include "Scene/UI/Widget.h"
#include "Utils/Types/UnknownPtr.h"

using namespace o2;

namespace o2
{
	class UIContextMenu;
}

namespace Editor
{
	class UIFoldersTree;

	// --------------------------
	// Assets folders tree widget
	// --------------------------
	class UIAssetsFoldersTree: public UIWidget, public KeyboardEventsListener
	{
	public:
		// Default constructor
		UIAssetsFoldersTree();

		// Copy-constructor
		UIAssetsFoldersTree(const UIAssetsFoldersTree& other);

		// Destructor
		~UIAssetsFoldersTree();

		// Copy-operator
		UIAssetsFoldersTree& operator=(const UIAssetsFoldersTree& other);

		SERIALIZABLE(UIAssetsFoldersTree);

	protected:
		UITree*        mFoldersTree; // Folders tree
		UIContextMenu* mContextMenu; // Context menu
		String         mCurrentPath; // Current viewing path

		bool           mOpengingFolderFromThis = false;

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Selects and expands folder
		void SelectAndExpandFolder(const String& path);

		// Rebuilds folders tree
		void UpdateView();

		// Initializes assets context menu
		void InitializeContext();

		// Returns folder parent (for folders tree)
		UnknownPtr GetFoldersTreeNodeParent(UnknownPtr object);

		// Returns folder childs (for folders tree)
		Vector<UnknownPtr> GetFoldersTreeNodeChilds(UnknownPtr object);

		// Setups tree node by folder (for folders tree)
		void SetupFoldersTreeNode(UITreeNode* node, UnknownPtr object);

		// It is called when folder item double clicked (for folders tree)
		void OnFoldersTreeNodeDblClick(UITreeNode* node);

		// It is called when folder item clicked (for folders tree)
		void OnFoldersTreeSelect(Vector<UnknownPtr> nodes);

		// It is called when folders tree clicked by right button
		void OnFoldersTreeRightClick(UITreeNode* node);

		// It is called when context copy pressed
		void OnContextCopyPressed();

		// It is called when context cut pressed
		void OnContextCutPressed();

		// It is called when context paste pressed
		void OnContextPastePressed();

		// It is called when context delete pressed
		void OnContextDeletePressed();

		// It is called when context open pressed
		void OnContextOpenPressed();

		// It is called when context show in explorer pressed
		void OnContextShowInExplorerPressed();

		// It is called when context import pressed
		void OnContextImportPressed();

		// It is called when context create folder pressed
		void OnContextCreateFolderPressed();

		// It is called when context create prefab pressed
		void OnContextCreatePrefabPressed();

		// It is called when context create script pressed
		void OnContextCreateScriptPressed();

		// It is called when context create animation pressed
		void OnContextCreateAnimationPressed();

		// It is called when context expand pressed
		void OnContextExpandPressed();

		// It is called when context expand pressed
		void OnContextCollapsePressed();

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		friend class AssetsWindow;
	};

	class UIFoldersTree : public UITree
	{
	public:
		SERIALIZABLE(UIFoldersTree);

	protected:
		// Updates visible nodes (calculates range and initializes nodes), enables editor mode
		void UpdateVisibleNodes() override;
	};
}

CLASS_BASES_META(Editor::UIAssetsFoldersTree)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::UIAssetsFoldersTree)
{
	PROTECTED_FIELD(mFoldersTree);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mCurrentPath);
	PROTECTED_FIELD(mOpengingFolderFromThis);
}
END_META;
CLASS_METHODS_META(Editor::UIAssetsFoldersTree)
{

	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, SelectAndExpandFolder, const String&);
	PROTECTED_FUNCTION(void, UpdateView);
	PROTECTED_FUNCTION(void, InitializeContext);
	PROTECTED_FUNCTION(UnknownPtr, GetFoldersTreeNodeParent, UnknownPtr);
	PROTECTED_FUNCTION(Vector<UnknownPtr>, GetFoldersTreeNodeChilds, UnknownPtr);
	PROTECTED_FUNCTION(void, SetupFoldersTreeNode, UITreeNode*, UnknownPtr);
	PROTECTED_FUNCTION(void, OnFoldersTreeNodeDblClick, UITreeNode*);
	PROTECTED_FUNCTION(void, OnFoldersTreeSelect, Vector<UnknownPtr>);
	PROTECTED_FUNCTION(void, OnFoldersTreeRightClick, UITreeNode*);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextOpenPressed);
	PROTECTED_FUNCTION(void, OnContextShowInExplorerPressed);
	PROTECTED_FUNCTION(void, OnContextImportPressed);
	PROTECTED_FUNCTION(void, OnContextCreateFolderPressed);
	PROTECTED_FUNCTION(void, OnContextCreatePrefabPressed);
	PROTECTED_FUNCTION(void, OnContextCreateScriptPressed);
	PROTECTED_FUNCTION(void, OnContextCreateAnimationPressed);
	PROTECTED_FUNCTION(void, OnContextExpandPressed);
	PROTECTED_FUNCTION(void, OnContextCollapsePressed);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
}
END_META;

CLASS_BASES_META(Editor::UIFoldersTree)
{
	BASE_CLASS(o2::UITree);
}
END_META;
CLASS_FIELDS_META(Editor::UIFoldersTree)
{
}
END_META;
CLASS_METHODS_META(Editor::UIFoldersTree)
{

	PROTECTED_FUNCTION(void, UpdateVisibleNodes);
}
END_META;
