#pragma once

#include "Application/Input.h"
#include "Core/WindowsSystem/IEditorWindow.h"
#include "Utils/Containers/Pair.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class UIButton;
	class UIEditBox;
	class UIGridLayout;
	class UILabel;
	class UIScrollArea;
	class UITree;
	class UITreeNode;
	class UIWidget;
}

// Editor assets window accessor macros
#define o2EditorAssets AssetsWindow::Instance()

namespace Editor
{
	class UIAssetsIconsScrollArea;
	class UIAssetsFoldersTree;

	// -------------
	// Assets window
	// -------------
	class AssetsWindow: public IEditorWindow, public Singleton<AssetsWindow>
	{
	public:
		// Default constructor. Initializes window
		AssetsWindow();

		// Destructor
		~AssetsWindow();

		// Selects asset with id
		void SelectAsset(AssetId id);

		// Selects asset by path
		void SelectAsset(const String& path);

		// Selects assets with ids
		void SelectAsset(const Vector<AssetId>& ids);

		// Selects assets by paths
		void SelectAssets(const Vector<String>& paths);

		// Opens asset in folder
		void OpenAsset(AssetId id);

		// Opens asset in folder
		void OpenAsset(const String& path);

		// Opens asset for editing 
		void OpenAndEditAsset(AssetId id);

		// Opens asset for editing 
		void OpenAndEditAsset(const String& path);

		// Deselects all assets
		void DeselectAssets();

		// Returns selected assets infos
		Vector<AssetInfo> GetSelectedAssets() const;

		// Returns opened folder path
		String GetOpenedFolderPath() const;

		// Opens folder
		void OpenFolder(const String& path);

		// Shows asset
		void ShowAssetIcon(AssetId id);

		// Shows asset
		void ShowAssetIcon(const String& path);

		// Copy assets in clipboard
		void CopyAssets(const Vector<String>& assetsPaths);

		// Cut assets and put into clipboard
		void CutAssets(const Vector<String>& assetsPaths);

		// Paste assets from clipboard to path
		void PasteAssets(const String& targetPath);

		// Removes assets in clipboard
		void DeleteAssets(const Vector<String>& assetsPaths);

		// Calls when context import pressed
		void ImportAssets(const String& targetPath);

		// Calls when context create folder pressed
		void CreateFolderAsset(const String& targetPath);

		// Calls when context create prefab pressed
		void CreatePrefabAsset(const String& targetPath);

		// Calls when context create script pressed
		void CreateScriptAsset(const String& targetPath);

		// Calls when context create animation pressed
		void CreateAnimationAsset(const String& targetPath);

		IOBJECT(AssetsWindow);

	protected:
		typedef Vector<Pair<AssetId, String>> AssetIdPathVec;

		UIButton*                mFilterButton;           // Search filter button
		UIEditBox*               mSearchEditBox;          // Search edit box
		UILabel*                 mSelectedAssetPathLabel; // Selected asset path label

		UIAssetsFoldersTree*     mFoldersTree;            // Folders tree				         
		UIAssetsIconsScrollArea* mAssetsGridScroll;       // Assets grid scroll
		UITree*                  mAssetsTree;             // Assets tree

		CursorEventsArea         mSeparatorHandle;        // Folders tree and assets tree/grid separator handle

		AssetIdPathVec           mCuttingAssets;          // Current cutted assets

	protected:
		// Initializes window
		void InitializeWindow();

		// Calls when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// Calls when menu filter button was pressed
		void OnMenuFilterPressed();

		// Calls when show folders tree button pressed
		void OnShowTreePressed();

		// Calls when assets was rebuilded
		void OnAssetsRebuilded(const Vector<AssetId>& changedAssets);

		// Copies asset folder recursively
		void CopyAssetFolder(const String& src, const String& dst);

		friend class UIAssetsFoldersTree;
		friend class UIAssetsIconsScrollArea;
	};
}