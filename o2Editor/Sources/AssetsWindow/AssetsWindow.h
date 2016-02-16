#pragma once

#include "Application/Input.h"
#include "Core/WindowsSystem/IEditorWindow.h"
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

class UIAssetsIconsScrollArea;

// Editor assets window accessor macros
#define o2EditorAssets AssetsWindow::Instance()

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

	IOBJECT(AssetsWindow);

protected:				             
	UIButton*                mFilterButton;           // Search filter button
	UIEditBox*               mSearchEditBox;          // Search edit box
	UILabel*                 mSelectedAssetPathLabel; // Selected asset path label
				             
	UITree*                  mFoldersTree;            // Folders tree				         
	UIAssetsIconsScrollArea* mAssetsGridScroll;       // Assets grid scroll
 	UITree*                  mAssetsTree;             // Assets tree
				             
	AssetInfo                mOpenedAssetFolder;      // Current viewing assets path

	CursorEventsArea         mSeparatorHandle;        // Folders tree and assets tree/grid separator handle

protected:
	// Initializes window
	void InitializeWindow();

	// Calls when search edit box text was changed
	void OnSearchEdited(const WString& search);

	// Calls when menu filter button was pressed
	void OnMenuFilterPressed();

	// Calls when show folders tree button pressed
	void OnShowTreePressed();

	// Returns folder parent (for folders tree)
	UnknownType* GetFoldersTreeNodeParent(UnknownType* object);

	// Returns folder childs (for folders tree)
	Vector<UnknownType*> GetFoldersTreeNodeChilds(UnknownType* object);

	// Setups tree node by folder (for folders tree)
	void SetupFoldersTreeNode(UITreeNode* node, UnknownType* object);

	// Calls when folder item double clicked (for folders tree)
	void OnFoldersTreeNodeDblClick(UITreeNode* node);

	// Calls when folder item clicked (for folders tree)
	void OnFoldersTreeClick(UITreeNode* node);
};