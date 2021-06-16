#pragma once

#include "o2/Application/Input.h"
#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Pair.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class Button;
	class EditBox;
	class GridLayout;
	class Label;
	class ScrollArea;
	class Tree;
	class TreeNode;
	class Widget;
}

// Editor assets window accessor macros
#define o2EditorAssets AssetsWindow::Instance()

namespace Editor
{
	class AssetsIconsScrollArea;
	class AssetsFoldersTree;

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
		void SelectAsset(const UID& id);

		// Selects asset by path
		void SelectAsset(const String& path);

		// Selects assets with ids
		void SelectAsset(const Vector<UID>& ids);

		// Selects assets by paths
		void SelectAssets(const Vector<String>& paths);

		// Deselects all assets
		void DeselectAssets();

		// Returns selected assets infos
		const Vector<const AssetInfo*>& GetSelectedAssets() const;

		// Opens asset in folder
		void OpenAsset(const UID& id);

		// Opens asset in folder
		void OpenAsset(const String& path);

		// Opens asset for editing 
		void OpenAndEditAsset(const UID& id);

		// Opens asset for editing 
		void OpenAndEditAsset(const String& path);

		// Returns opened folder path
		String GetOpenedFolderPath() const;

		// Opens folder
		void OpenFolder(const String& path);

		// Shows asset
		void ShowAssetIcon(const UID& id);

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

		// Creates and returns icon sprite for asset
		static Sprite* GetAssetIconSprite(const AssetRef& asset);
		 
		IOBJECT(AssetsWindow);

	protected:
		float mFoldersTreeShowCoef; // Animating show folders tree coefficient (0...1)

		Button*  mFilterButton;           // Search filter button
		EditBox* mSearchEditBox;          // Search edit box
		Label*   mSelectedAssetPathLabel; // Selected asset path label

		AssetsFoldersTree* mFoldersTree;         // Folders tree			
		AnimationPlayer    mFoldersTreeShowAnim; // Folders tree visible animation
		bool               mFoldersTreeVisible;  // Is folders tree visible

		AssetsIconsScrollArea* mAssetsGridScroll; // Assets grid scroll

		Tree* mAssetsTree; // Assets tree

		CursorEventsArea mSeparatorHandle; // Folders tree and assets tree/grid separator handle
		float            mSeparatorCoef;   // Separator coefficient, means anchors for tree nad assets scroll

		Vector<Pair<UID, String>> mCuttingAssets; // Current cutted assets

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

		// It is called when assets was rebuilt
		void OnAssetsRebuilt(const Vector<UID>& changedAssets);

		// Shows and highligh asset icon
		void ShowAssetIcon(const String& folder, UID assetId);

		// Copies asset folder recursively
		void CopyAssetFolder(const String& src, const String& dst);

		friend class AssetsFoldersTree;
		friend class AssetsIconsScrollArea;
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
	FIELD().NAME(mFoldersTreeShowCoef).PROTECTED();
	FIELD().NAME(mFilterButton).PROTECTED();
	FIELD().NAME(mSearchEditBox).PROTECTED();
	FIELD().NAME(mSelectedAssetPathLabel).PROTECTED();
	FIELD().NAME(mFoldersTree).PROTECTED();
	FIELD().NAME(mFoldersTreeShowAnim).PROTECTED();
	FIELD().NAME(mFoldersTreeVisible).PROTECTED();
	FIELD().NAME(mAssetsGridScroll).PROTECTED();
	FIELD().NAME(mAssetsTree).PROTECTED();
	FIELD().NAME(mSeparatorHandle).PROTECTED();
	FIELD().NAME(mSeparatorCoef).PROTECTED();
	FIELD().NAME(mCuttingAssets).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::AssetsWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SelectAsset, const UID&);
	PUBLIC_FUNCTION(void, SelectAsset, const String&);
	PUBLIC_FUNCTION(void, SelectAsset, const Vector<UID>&);
	PUBLIC_FUNCTION(void, SelectAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, DeselectAssets);
	PUBLIC_FUNCTION(const Vector<const AssetInfo*>&, GetSelectedAssets);
	PUBLIC_FUNCTION(void, OpenAsset, const UID&);
	PUBLIC_FUNCTION(void, OpenAsset, const String&);
	PUBLIC_FUNCTION(void, OpenAndEditAsset, const UID&);
	PUBLIC_FUNCTION(void, OpenAndEditAsset, const String&);
	PUBLIC_FUNCTION(String, GetOpenedFolderPath);
	PUBLIC_FUNCTION(void, OpenFolder, const String&);
	PUBLIC_FUNCTION(void, ShowAssetIcon, const UID&);
	PUBLIC_FUNCTION(void, ShowAssetIcon, const String&);
	PUBLIC_FUNCTION(void, CopyAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, CutAssets, const Vector<String>&);
	PUBLIC_FUNCTION(void, PasteAssets, const String&);
	PUBLIC_FUNCTION(void, DeleteAssets, const Vector<String>&);
	PUBLIC_STATIC_FUNCTION(Sprite*, GetAssetIconSprite, const AssetRef&);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeFoldersTreeSeparator);
	PROTECTED_FUNCTION(void, InitializeFoldersTreeVisibleState);
	PROTECTED_FUNCTION(void, InitializeFoldersTree);
	PROTECTED_FUNCTION(void, InitializeDownPanel);
	PROTECTED_FUNCTION(void, InitializeUpPanel);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, OnMenuFilterPressed);
	PROTECTED_FUNCTION(void, OnShowTreePressed);
	PROTECTED_FUNCTION(void, OnAssetsRebuilt, const Vector<UID>&);
	PROTECTED_FUNCTION(void, ShowAssetIcon, const String&, UID);
	PROTECTED_FUNCTION(void, CopyAssetFolder, const String&, const String&);
}
END_META;
