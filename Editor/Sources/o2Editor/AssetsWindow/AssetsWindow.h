#pragma once

#include "o2/Application/Input.h"
#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Pair.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

#include <memory>
#include <vector>
#include <string>

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
		void SelectAsset(const std::string& path);

		// Selects assets with ids
		void SelectAsset(const std::vector<UID>& ids);

		// Selects assets by paths
		void SelectAssets(const std::vector<std::string>& paths);

		// Deselects all assets
		void DeselectAssets();

		// Returns selected assets infos
		const std::vector<std::shared_ptr<AssetInfo>>& GetSelectedAssets() const;

		// Opens asset in folder
		void OpenAsset(const UID& id);

		// Opens asset in folder
		void OpenAsset(const std::string& path);

		// Opens asset for editing 
		void OpenAndEditAsset(const UID& id);

		// Opens asset for editing 
		void OpenAndEditAsset(const std::string& path);

		// Returns opened folder path
		std::string GetOpenedFolderPath() const;

		// Opens folder
		void OpenFolder(const std::string& path);

		// Shows asset
		void ShowAssetIcon(const UID& id);

		// Shows asset
		void ShowAssetIcon(const std::string& path);

		// Copy assets in clipboard
		void CopyAssets(const std::vector<std::string>& assetsPaths);

		// Cut assets and put into clipboard
		void CutAssets(const std::vector<std::string>& assetsPaths);

		// Paste assets from clipboard to path
		void PasteAssets(const std::string& targetPath);

		// Removes assets in clipboard
		void DeleteAssets(const std::vector<std::string>& assetsPaths);

		// Creates and returns an icon sprite for the asset
		static Sprite* GetAssetIconSprite(const std::shared_ptr<Asset>& asset);
		 
		IOBJECT(AssetsWindow);

	protected:
		float mFoldersTreeShowCoef = 1.0f; // Animation show folders tree coefficient (0...1)

		std::shared_ptr<Button>  mFilterButton;           // Search filter button
		std::shared_ptr<EditBox> mSearchEditBox;          // Search edit box
		std::shared_ptr<Label>   mSelectedAssetPathLabel; // Selected asset path label

		std::shared_ptr<AssetsFoldersTree>   mFoldersTree;                                    // Folders tree			
		std::shared_ptr<AnimationPlayer> mFoldersTreeShowAnim = mmake<AnimationPlayer>(); // Folders tree visible animation
		bool                 mFoldersTreeVisible;                             // Is folders tree visible

		std::shared_ptr<AssetsIconsScrollArea> mAssetsGridScroll; // Assets grid scroll

		std::shared_ptr<Tree> mAssetsTree; // Assets tree

		CursorEventsArea mSeparatorHandle; // Folders tree and assets tree/grid separator handle
		float            mSeparatorCoef;   // Separator coefficient, means anchors for tree nad assets scroll

		std::vector<Pair<UID, std::string>> mCuttingAssets; // Current cutted assets

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
		void OnAssetsRebuilt(const std::vector<UID>& changedAssets);

		// Shows and highligh asset icon
		void ShowAssetIcon(const std::string& folder, UID assetId);

		// Copies asset folder recursively
		void CopyAssetFolder(const std::string& src, const std::string& dst);

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
END_META;replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>. 

```
ECTED().NAME(mAssetsTree);
    FIELD().PROTECTED().NAME(mSeparatorHandle);
    FIELD().PROTECTED().NAME(mSeparatorCoef);
    FIELD().PROTECTED().NAME(mCuttingAssets);
}
END_META;
CLASS_METHODS_META(Editor::AssetsWindow)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const Ref<UID>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const Ref<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAsset, const Ref<Vector<UID>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SelectAssets, const Ref<Vector<String>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, DeselectAssets);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<Vector<Ref<AssetInfo>>>&, GetSelectedAssets);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAsset, const Ref<UID>&);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAsset, const Ref<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAndEditAsset, const Ref<UID>&);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenAndEditAsset, const Ref<String>&);
    FUNCTION().PUBLIC().SIGNATURE(String, GetOpenedFolderPath);
    FUNCTION().PUBLIC().SIGNATURE(void, OpenFolder, const Ref<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, ShowAssetIcon, const Ref<UID>&);
    FUNCTION().PUBLIC().SIGNATURE(void, ShowAssetIcon, const Ref<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, CopyAssets, const Ref<Vector<String>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, CutAssets, const Ref<Vector<String>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, PasteAssets, const Ref<String>&);
    FUNCTION().PUBLIC().SIGNATURE(void, DeleteAssets, const Ref<Vector<String>>&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Sprite*, GetAssetIconSprite, const Ref<Asset>&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeFoldersTreeSeparator);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeFoldersTreeVisibleState);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeFoldersTree);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeDownPanel);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeUpPanel);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSearchEdited, const Ref<WString>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMenuFilterPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnShowTreePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnAssetsRebuilt, const Ref<Vector<UID>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, ShowAssetIcon, const Ref<String>&, UID);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyAssetFolder, const Ref<String>&, const Ref<String>&);
}
END_META;
// --- END META ---
```