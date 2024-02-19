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
		const Vector<Ref<AssetInfo>>& GetSelectedAssets() const;

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

		// Creates and returns an icon sprite for the asset
		static Ref<Sprite> GetAssetIconSprite(const Ref<Asset>& asset);
		 
		IOBJECT(AssetsWindow);

		protected:
		float mFoldersTreeShowCoef = 1.0f; // Animation show folders tree coefficient (0...1)

		Ref<Button>  mFilterButton;           // Search filter button
		Ref<EditBox> mSearchEditBox;          // Search edit box
		Ref<Label>   mSelectedAssetPathLabel; // Selected asset path label

		Ref<AssetsFoldersTree>   mFoldersTree;                                    // Folders tree			
		Ref<AnimationPlayer> mFoldersTreeShowAnim = mmake<AnimationPlayer>(); // Folders tree visible animation
		bool                 mFoldersTreeVisible;                             // Is folders tree visible

		Ref<AssetsIconsScrollArea> mAssetsGridScroll; // Assets grid scroll

		Ref<Tree> mAssetsTree; // Assets tree

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

		// Called when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// Called when menu filter button was pressed
		void OnMenuFilterPressed();

		// Called when show folders tree button pressed
		void OnShowTreePressed();

		// Called when assets was rebuilt
		void OnAssetsRebuilt(const Vector<UID>& changedAssets);

		// Shows and highligh asset icon
		void ShowAssetIcon(const String& folder, UID assetId);

		// Copies asset folder recursively
		void CopyAssetFolder(const String& src, const String& dst);

		friend class AssetsFoldersTree;
		friend class AssetsIconsScrollArea;
	};
}
// --- META ---

CLASS_BASES_META(Editor::AssetsWindow)
{
    BASE_CLASS(Editor::IEditorWindow);
    BASE_CLASS(o2::Singleton<AssetsWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::AssetsWindow)
{
    FIELD().PROTECTED().DEFAULT_VALUE(1.0f).NAME(mFoldersTreeShowCoef);
    FIELD().PROTECTED().NAME(mFilterButton);
    FIELD().PROTECTED().NAME(mSearchEditBox);
    FIELD().PROTECTED().NAME(mSelectedAssetPathLabel);
    FIELD().PROTECTED().NAME(mFoldersTree);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<AnimationPlayer>()).NAME(mFoldersTreeShowAnim);
    FIELD().PROTECTED().NAME(mFoldersTreeVisible);
    FIELD().PROTECTED().NAME(mAssetsGridScroll);
    FIELD().PROTECTED().NAME(mAssetsTree);
    FIELD().PROTECTED().NAME(mSeparatorHandle);
    FIELD().PROTECTED().NAME(mSeparatorCoef);
    FIELD().PROTECTED().NAME(mCuttingAssets);
}
END_META;
CLASS_METHODS_META(Editor::AssetsWindow)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const Vector<UID>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAssets, const Vector<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, DeselectAssets);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<AssetInfo>>&, GetSelectedAssets);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAsset, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAsset, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAndEditAsset, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAndEditAsset, const String&);
    FUNCTION().PUBLIC().SIGNATURE(String, GetOpenedFolderPath);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenFolder, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, ShowAssetIcon, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(void, ShowAssetIcon, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, CopyAssets, const Vector<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, CutAssets, const Vector<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, PasteAssets, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, DeleteAssets, const Vector<String>&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<Sprite>, GetAssetIconSprite, const Ref<Asset>&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeFoldersTreeSeparator);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeFoldersTreeVisibleState);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeFoldersTree);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeDownPanel);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeUpPanel);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSearchEdited, const WString&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMenuFilterPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnShowTreePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnAssetsRebuilt, const Vector<UID>&);
    FUNCTION().PROTECTED().SIGNATURE(void, ShowAssetIcon, const String&, UID);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyAssetFolder, const String&, const String&);
}
END_META;
// --- END META ---
