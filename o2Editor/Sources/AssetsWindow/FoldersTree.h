#pragma once

#include "Events/KeyboardEventsListener.h"
#include "UI/Widget.h"
#include "Utils/UnknownPtr.h"

using namespace o2;

namespace o2
{
	class UITree;
	class UIContextMenu;
}

namespace Editor
{
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

		// Calls when folder item double clicked (for folders tree)
		void OnFoldersTreeNodeDblClick(UITreeNode* node);

		// Calls when folder item clicked (for folders tree)
		void OnFoldersTreeSelect(Vector<UnknownPtr> nodes);

		// Calls when folders tree clicked by right button
		void OnFoldersTreeRightClick(UITreeNode* node);

		// Calls when context copy pressed
		void OnContextCopyPressed();

		// Calls when context cut pressed
		void OnContextCutPressed();

		// Calls when context paste pressed
		void OnContextPastePressed();

		// Calls when context delete pressed
		void OnContextDeletePressed();

		// Calls when context open pressed
		void OnContextOpenPressed();

		// Calls when context show in explorer pressed
		void OnContextShowInExplorerPressed();

		// Calls when context import pressed
		void OnContextImportPressed();

		// Calls when context create folder pressed
		void OnContextCreateFolderPressed();

		// Calls when context create prefab pressed
		void OnContextCreatePrefabPressed();

		// Calls when context create script pressed
		void OnContextCreateScriptPressed();

		// Calls when context create animation pressed
		void OnContextCreateAnimationPressed();

		// Calls when context expand pressed
		void OnContextExpandPressed();

		// Calls when context expand pressed
		void OnContextCollapsePressed();

		// Calls when key was released
		void OnKeyReleased(const Input::Key& key);

		friend class AssetsWindow;
	};
}