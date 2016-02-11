#include "AssetsWindow.h"

#include "Assets/Assets.h"
#include "Assets/AssetsTree.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/UIAssetIcon.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/GridLayout.h"
#include "UI/Label.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "Utils/Delegates.h"
#include "Utils/FileSystem/FileSystem.h"

DECLARE_SINGLETON(AssetsWindow);

AssetsWindow::AssetsWindow()
{
	InitializeWindow();
	PrepareIconsPools();
}

AssetsWindow::~AssetsWindow()
{}

void AssetsWindow::InitializeWindow()
{
	mWindow->caption = "Assets";
	mWindow->name = "assets window";
	if (auto iconLayer = mWindow->GetLayer("icon"))
	{
		*((Sprite*)iconLayer->drawable) = Sprite("ui/UI2_folder_icon.png");
		iconLayer->layout = Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1));
	}

	mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
	mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

	// up panel
	UIWidget* upPanel = mnew UIWidget();
	upPanel->name = "up panel";
	upPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
	upPanel->AddLayer("back", mnew Sprite("ui/UI_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

	UIButton* searchButton = o2UI.CreateWidget<UIButton>("search");
	searchButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
	upPanel->AddChild(searchButton);

	mFilterButton = o2UI.CreateWidget<UIButton>("menu filter");
	mFilterButton->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
	mFilterButton->onClick += Function<void()>(this, &AssetsWindow::OnMenuFilterPressed);
	upPanel->AddChild(mFilterButton);

	mSearchEditBox = o2UI.CreateWidget<UIEditBox>("backless");
	mSearchEditBox->layout = UIWidgetLayout::BothStretch(19, 2, 21, -2);
	mSearchEditBox->onChanged += Function<void(const WString&)>(this, &AssetsWindow::OnSearchEdited);
	upPanel->AddChild(mSearchEditBox);

	mWindow->AddChild(upPanel);

	// down panel
	UIWidget* downPanel = mnew UIWidget();
	downPanel->AddLayer("back", mnew Sprite("ui/UI2_small_panel_down_back.png"),
						Layout::BothStretch(-4, -5, -4, -5));
	downPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20, 0);

	auto showTreeBtn = o2UI.CreateWidget<UIButton>("menu tree");
	showTreeBtn->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0));
	showTreeBtn->onClick += Function<void()>(this, &AssetsWindow::OnShowTreePressed);
	downPanel->AddChild(showTreeBtn);

	mSelectedAssetPathLabel = o2UI.CreateWidget<UILabel>();
	mSelectedAssetPathLabel->layout = UIWidgetLayout::BothStretch(20, 0, 0, 0);
	mSelectedAssetPathLabel->text = "Assets/Folder/Image.png";
	mSelectedAssetPathLabel->horOverflow = UILabel::HorOverflow::Dots;
	mSelectedAssetPathLabel->horAlign = HorAlign::Left;
	downPanel->AddChild(mSelectedAssetPathLabel);

	mWindow->AddChild(downPanel);

	// folders tree
	mFoldersTree = o2UI.CreateWidget<UITree>("folders");
	mFoldersTree->layout = UIWidgetLayout(0.0f, 1.0f, 0.5f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);

	mFoldersTree->AddLayer("separator", mnew Sprite("ui/UI_Ver_separator.png"),
						   Layout::VerStretch(HorAlign::Right, -2, 0, 5, 0));
	mFoldersTree->SetRearrangeType(UITree::RearrangeType::OnlyReparent);

	mFoldersTree->getParentFunc = Function<UnknownType*(UnknownType*)>(this, &AssetsWindow::GetFoldersTreeNodeParent);
	mFoldersTree->getChildsFunc = Function<Vector<UnknownType*>(UnknownType*)>(this, &AssetsWindow::GetFoldersTreeNodeChilds);
	mFoldersTree->setupNodeFunc = Function<void(UITreeNode*, UnknownType*)>(this, &AssetsWindow::SetupFoldersTreeNode);
	mFoldersTree->onItemDblClick = Function<void(UITreeNode*)>(this, &AssetsWindow::OnFoldersTreeNodeDblClick);
	mFoldersTree->onItemClick = Function<void(UITreeNode*)>(this, &AssetsWindow::OnFoldersTreeClick);
	mFoldersTree->RebuildTree();

	mWindow->AddChild(mFoldersTree);

	// assets scroll & grid
	mAssetsScroll = o2UI.CreateWidget<UIScrollArea>("backless");
	mAssetsScroll->layout = UIWidgetLayout(0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);

	mAssetsGrid = mnew UIGridLayout();
	mAssetsGrid->layout = UIWidgetLayout::BothStretch();
	mAssetsGrid->baseCorner = BaseCorner::LeftTop;
	mAssetsGrid->fitByChildren = true;
	mAssetsGrid->cellSize = Vec2F(40, 50);
	mAssetsGrid->border = RectF(5, 5, 5, 5);
	mAssetsGrid->spacing = 5;
	mAssetsGrid->arrangeAxis = TwoDirection::Horizontal;
	mAssetsGrid->arrangeAxisMaxCells = 5;

	mAssetsScroll->AddChild(mAssetsGrid);

	mWindow->AddChild(mAssetsScroll);
}

void AssetsWindow::OnSearchEdited(const WString& search)
{

}

void AssetsWindow::OnMenuFilterPressed()
{

}

void AssetsWindow::OnShowTreePressed()
{

}

void AssetsWindow::UpdateAssetsFolder(const String& folder)
{
	o2Debug.LogWarning("Update assets folder: %s", folder);

	auto prevIcons = mAssetsGrid->GetChilds();
	for (auto child : prevIcons)
	{
		mAssetsGrid->RemoveChild(child, false);
		FreeAssetIconToPool((UIAssetIcon*)child);
	}

	FolderAsset* folderAsset = mnew FolderAsset(folder);

	Vector<UIWidget*> addingIcons;
	for (auto asset : folderAsset->GetContainingAssetsInfos())
	{
		o2Debug.Log(asset.mPath);

		String assetIconStyle = "standard";

		if (asset.mType == FolderAsset::type.ID())
			assetIconStyle = "folder";

		UIAssetIcon* assetIcon = GetAssetIconFromPool(assetIconStyle);
		assetIcon->SetAssetInfo(asset);
		assetIcon->name = assetIconStyle;

		addingIcons.Add(assetIcon);
	}

	mAssetsGrid->AddChilds(addingIcons);

	delete folderAsset;
}

UnknownType* AssetsWindow::GetFoldersTreeNodeParent(UnknownType* object)
{
	AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)object;
	return (UnknownType*)(void*)(assetTreeNode->GetParent());
}

Vector<UnknownType*> AssetsWindow::GetFoldersTreeNodeChilds(UnknownType* object)
{
	AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)object;

	if (assetTreeNode)
	{
		return assetTreeNode->GetChilds().
			FindAll([](AssetTree::AssetNode* x) { return x->mType == FolderAsset::type.ID(); }).
			Select<UnknownType*>([](AssetTree::AssetNode* x) { return (UnknownType*)(void*)x; });
	}
	else
	{
		const AssetTree& assetsTree = o2Assets.GetAssetsTree();

		return assetsTree.mRootAssets.
			FindAll([](AssetTree::AssetNode* x) { return x->mType == FolderAsset::type.ID(); }).
			Select<UnknownType*>([](AssetTree::AssetNode* x) { return (UnknownType*)(void*)x; });
	}
}

void AssetsWindow::SetupFoldersTreeNode(UITreeNode* node, UnknownType* object)
{
	AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)object;
	String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->mPath);

	node->name = pathName;

	auto nameLayer = node->layer["name"];
	((Text*)nameLayer->drawable)->text = pathName;
}

void AssetsWindow::OnFoldersTreeNodeDblClick(UITreeNode* node)
{
	AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)node->GetObject();
	String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->mPath);

	node->SetState("edit", true);

	auto editBox = (UIEditBox*)node->GetChild("nameEditBox");
	editBox->text = (String)pathName;
	editBox->SelectAll();
	editBox->UIWidget::Select();

	editBox->onChangeCompleted = [=](const WString& text) {
		//actor->SetName(text);
		node->SetState("edit", false);
		node->Rebuild(false);
	};
}

void AssetsWindow::OnFoldersTreeClick(UITreeNode* node)
{
	AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)node->GetObject();

	UpdateAssetsFolder(assetTreeNode->mPath);
}

void AssetsWindow::PrepareIconsPools()
{
	int poolSize = 10;
	String iconsStyles[] = { "standard", "folder", "prefab", "prefab preview", "image preview", "text", "animation" };

	for (auto style : iconsStyles)
	{
		mIconsPools.Add(style, Vector<UIAssetIcon*>());

		for (int i = 0; i < poolSize; i++)
		{
			UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>(style);
			mIconsPools[style].Add(sample);
		}
	}
}

UIAssetIcon* AssetsWindow::GetAssetIconFromPool(const String& style)
{
	if (!mIconsPools.ContainsKey(style))
		return o2UI.CreateWidget<UIAssetIcon>(style);

	int poolResizeStep = 10;
	if (mIconsPools[style].Count() == 0)
	{
		for (int i = 0; i < poolResizeStep; i++)
		{
			UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>(style);
			mIconsPools[style].Add(sample);
		}
	}

	UIAssetIcon* sample = mIconsPools[style].PopBack();
	return sample;
}

void AssetsWindow::FreeAssetIconToPool(UIAssetIcon* icon)
{
	if (mIconsPools.ContainsKey(icon->name))
		mIconsPools[icon->name].Add(icon);
	else
		delete icon;
}
