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
}
