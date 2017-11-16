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

		// Updates window logic
		void Update(float dt) override;

		// Selects asset with id
		void SelectAsset(UID id);

		// Selects asset by path
		void SelectAsset(const String& path);

		// Selects assets with ids
		void SelectAsset(const Vector<UID>& ids);

		// Selects assets by paths
		void SelectAssets(const Vector<String>& paths);

		// Opens asset in folder
		void OpenAsset(UID id);

		// Opens asset in folder
		void OpenAsset(const String& path);

		// Opens asset for editing 
		void OpenAndEditAsset(UID id);

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
		void ShowAssetIcon(UID id);

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

		// It is called when context import pressed
		void ImportAssets(const String& targetPath);

		// It is called when context create folder pressed
		void CreateFolderAsset(const String& targetPath);

		// It is called when context create prefab pressed
		void CreatePrefabAsset(const String& targetPath);

		// It is called when context create script pressed
		void CreateScriptAsset(const String& targetPath);

		// It is called when context create animation pressed
		void CreateAnimationAsset(const String& targetPath);

		// Creates and returns icon sprite for asset
		static Sprite* GetAssetIconSprite(const AssetRef& asset);
		 
		IOBJECT(AssetsWindow);

	protected:
		typedef Vector<Pair<UID, String>> AssetIdPathVec;

		float                    mFoldersTreeShowCoef;    // Animating show folders tree coefficient (0...1)

		UIButton*                mFilterButton;           // Search filter button
		UIEditBox*               mSearchEditBox;          // Search edit box
		UILabel*                 mSelectedAssetPathLabel; // Selected asset path label

		UIAssetsFoldersTree*     mFoldersTree;            // Folders tree			
		Animation                mFoldersTreeShowAnim;    // Folders tree visible animation
		bool                     mFoldersTreeVisible;     // Is folders tree visible

		UIAssetsIconsScrollArea* mAssetsGridScroll;       // Assets grid scroll

		UITree*                  mAssetsTree;             // Assets tree

		CursorEventsArea         mSeparatorHandle;        // Folders tree and assets tree/grid separator handle
		float                    mSeparatorCoef;          // Separator coefficient, means anchors for tree nad assets scroll

		AssetIdPathVec           mCuttingAssets;          // Current cutted assets

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes folders tree separator
		void InitializeFoldersTreeSeparator();

		// Initializes folders tree visible state
		void InitializeFoldersTreeVisibleState();

		// Initializes folders tree
		void InitializeFoldersTree();

		// Initializes down panel
		void InitializeDownPanel();

		// Initializes up search panel
		void InitializeUpPanel();

		// It is called when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// It is called when menu filter button was pressed
		void OnMenuFilterPressed();

		// It is called when show folders tree button pressed
		void OnShowTreePressed();

		// It is called when assets was rebuilded
		void OnAssetsRebuilded(const Vector<UID>& changedAssets);

		// Copies asset folder recursively
		void CopyAssetFolder(const String& src, const String& dst);

		friend class UIAssetsFoldersTree;
		friend class UIAssetsIconsScrollArea;
	};
}

CLASS_BASES_META(Editor::AssetsWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::Singleton<AssetsWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::AssetsWindow)
{
	PROTECTED_FIELD(mFoldersTreeShowCoef);
	PROTECTED_FIELD(mFilterButton);
	PROTECTED_FIELD(mSearchEditBox);
	PROTECTED_FIELD(mSelectedAssetPathLabel);
	PROTECTED_FIELD(mFoldersTree);
	PROTECTED_FIELD(mFoldersTreeShowAnim);
	PROTECTED_FIELD(mFoldersTreeVisible);
	PROTECTED_FIELD(mAssetsGridScroll);
	PROTECTED_FIELD(mAssetsTree);
	PROTECTED_FIELD(mSeparatorHandle);
	PROTECTED_FIELD(mSeparatorCoef);
	PROTECTED_FIELD(mCuttingAssets);
}
END_META;
CLASS_METHODS_META(Editor::AssetsWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SelectAsset, UID);
	PUBLIC_FUNCTION(void, SelectAsset, const String&);
	PUBLIC_FUNCTION(void, SelectAsset, const Vector<UID>&);
	PUBLIC_FUNCTION(void, SelectAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, OpenAsset, UID);
	PUBLIC_FUNCTION(void, OpenAsset, const String&);
	PUBLIC_FUNCTION(void, OpenAndEditAsset, UID);
	PUBLIC_FUNCTION(void, OpenAndEditAsset, const String&);
	PUBLIC_FUNCTION(void, DeselectAssets);
	PUBLIC_FUNCTION(Vector<AssetInfo>, GetSelectedAssets);
	PUBLIC_FUNCTION(String, GetOpenedFolderPath);
	PUBLIC_FUNCTION(void, OpenFolder, const String&);
	PUBLIC_FUNCTION(void, ShowAssetIcon, UID);
	PUBLIC_FUNCTION(void, ShowAssetIcon, const String&);
	PUBLIC_FUNCTION(void, CopyAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, CutAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, PasteAssets, const String&);
	PUBLIC_FUNCTION(void, DeleteAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, ImportAssets, const String&);
	PUBLIC_FUNCTION(void, CreateFolderAsset, const String&);
	PUBLIC_FUNCTION(void, CreatePrefabAsset, const String&);
	PUBLIC_FUNCTION(void, CreateScriptAsset, const String&);
	PUBLIC_FUNCTION(void, CreateAnimationAsset, const String&);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeFoldersTreeSeparator);
	PROTECTED_FUNCTION(void, InitializeFoldersTreeVisibleState);
	PROTECTED_FUNCTION(void, InitializeFoldersTree);
	PROTECTED_FUNCTION(void, InitializeDownPanel);
	PROTECTED_FUNCTION(void, InitializeUpPanel);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, OnMenuFilterPressed);
	PROTECTED_FUNCTION(void, OnShowTreePressed);
	PROTECTED_FUNCTION(void, OnAssetsRebuilded, const Vector<UID>&);
	PROTECTED_FUNCTION(void, CopyAssetFolder, const String&, const String&);
}
END_META;
