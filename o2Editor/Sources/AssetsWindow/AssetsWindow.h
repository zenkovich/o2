#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIGridLayout;
	class UILabel;
	class UIScrollArea;
	class UITree;
	class UITreeNode;
	class UIWidget;
}

class UIAssetIcon;

#define o2EditorAssets AssetsWindow::Instance()

class AssetsWindow: public IEditorWindow, public Singleton<AssetsWindow>
{
public:
	AssetsWindow();
	~AssetsWindow();

	IOBJECT(AssetsWindow);

protected:
	typedef Dictionary<String, Vector<UIAssetIcon*>> IconArrsDict;

	UIButton*     mFilterButton;
	UIEditBox*    mSearchEditBox;
	UILabel*      mSelectedAssetPathLabel;

	UIScrollArea* mFoldersTreeScroll;
	UITree*       mFoldersTree;

	UIScrollArea* mAssetsScroll;
	UIGridLayout* mAssetsGrid;
	UITree*       mAssetsTree;

	IconArrsDict  mIconsPools;

	AssetInfo     mOpenedAssetFolder;

protected:
	void InitializeWindow();

	void OnSearchEdited(const WString& search);
	void OnMenuFilterPressed();
	void OnShowTreePressed();

	void UpdateAssetsFolder(const String& folder);

	UnknownType* GetFoldersTreeNodeParent(UnknownType* object);
	Vector<UnknownType*> GetFoldersTreeNodeChilds(UnknownType* object);
	void SetupFoldersTreeNode(UITreeNode* node, UnknownType* object);
	void OnFoldersTreeNodeDblClick(UITreeNode* node);
	void OnFoldersTreeClick(UITreeNode* node);

	void PrepareIconsPools();
	UIAssetIcon* GetAssetIconFromPool(const String& style);
	void FreeAssetIconToPool(UIAssetIcon* icon);
};