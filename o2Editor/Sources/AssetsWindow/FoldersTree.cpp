#include "FoldersTree.h"

#include "Assets/Assets.h"
#include "Assets/AssetsTree.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Render/Sprite.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "Utils/FileSystem/FileSystem.h"

namespace Editor
{
	UIAssetsFoldersTree::UIAssetsFoldersTree():
		UIWidget()
	{
		mFoldersTree = o2UI.CreateWidget<UITree>("folders");
		mFoldersTree->layout = UIWidgetLayout::BothStretch();

		mFoldersTree->SetRearrangeType(UITree::RearrangeType::OnlyReparent);
		mFoldersTree->SetMultipleSelectionAvailable(false);

		mFoldersTree->getParentFunc = Function<UnknownPtr(UnknownPtr)>(this, &UIAssetsFoldersTree::GetFoldersTreeNodeParent);
		mFoldersTree->getChildsFunc = Function<Vector<UnknownPtr>(UnknownPtr)>(this, &UIAssetsFoldersTree::GetFoldersTreeNodeChilds);
		mFoldersTree->setupNodeFunc = Function<void(UITreeNode*, UnknownPtr)>(this, &UIAssetsFoldersTree::SetupFoldersTreeNode);
		mFoldersTree->onItemDblClick = Function<void(UITreeNode*)>(this, &UIAssetsFoldersTree::OnFoldersTreeNodeDblClick);
		mFoldersTree->onItemClick = Function<void(UITreeNode*)>(this, &UIAssetsFoldersTree::OnFoldersTreeClick);
		mFoldersTree->onItemRBClick = Function<void(UITreeNode*)>(this, &UIAssetsFoldersTree::OnFoldersTreeRightClick);
		mFoldersTree->UpdateView();

		AddChild(mFoldersTree);

		InitializeContext();
	}

	UIAssetsFoldersTree::UIAssetsFoldersTree(const UIAssetsFoldersTree& other):
		UIWidget(other)
	{
		mFoldersTree = FindChild<UITree>();
		RemoveChild(FindChild<UIContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIAssetsFoldersTree::~UIAssetsFoldersTree()
	{}

	UIAssetsFoldersTree& UIAssetsFoldersTree::operator=(const UIAssetsFoldersTree& other)
	{
		UIWidget::operator=(other);

		mFoldersTree = FindChild<UITree>();
		RemoveChild(FindChild<UIContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIAssetsFoldersTree::SelectAndExpandFolder(const String& path)
	{
		mCurrentPath = path;
		mFoldersTree->SelectAndExpandObject(o2Assets.GetAssetsTree().FindAsset(path));
	}

	void UIAssetsFoldersTree::UpdateView()
	{
		mFoldersTree->UpdateView();
	}

	void UIAssetsFoldersTree::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<UIContextMenu>();

		mContextMenu->AddItem("Import", [&]() { OnContextImportPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Create/Folder", [&]() { OnContextCreateFolderPressed(); });
		mContextMenu->AddItem("Create/Prefab", [&]() { OnContextCreatePrefabPressed(); });
		mContextMenu->AddItem("Create/Script", [&]() { OnContextCreateScriptPressed(); });
		mContextMenu->AddItem("Create/Animation", [&]() { OnContextCreateAnimationPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, nullptr, ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, nullptr, ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, nullptr, ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, nullptr, ShortcutKeys(VK_DELETE));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Expand all", [&]() { OnContextExpandPressed(); });
		mContextMenu->AddItem("Collapse all", [&]() { OnContextCollapsePressed(); });

		AddChild(mContextMenu);
	}

	UnknownPtr UIAssetsFoldersTree::GetFoldersTreeNodeParent(UnknownPtr object)
	{
		if (!object)
			return UnknownPtr();

		AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)object;
		return (UnknownPtr)(void*)(assetTreeNode->GetParent());
	}

	Vector<UnknownPtr> UIAssetsFoldersTree::GetFoldersTreeNodeChilds(UnknownPtr object)
	{
		AssetTree::AssetNode* assetTreeNode = object;

		if (assetTreeNode)
		{
			return assetTreeNode->GetChilds().
				FindAll([](AssetTree::AssetNode* x) { return x->mType == TypeOf(FolderAsset).ID(); }).
				Select<UnknownPtr>([](AssetTree::AssetNode* x) { return UnknownPtr(x); });
		}
		else
		{
			const AssetTree& assetsTree = o2Assets.GetAssetsTree();

			return assetsTree.mRootAssets.
				FindAll([](AssetTree::AssetNode* x) { return x->mType == TypeOf(FolderAsset).ID(); }).
				Select<UnknownPtr>([](AssetTree::AssetNode* x) { return UnknownPtr(x); });
		}
	}

	void UIAssetsFoldersTree::SetupFoldersTreeNode(UITreeNode* node, UnknownPtr object)
	{
		AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)object;
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->mPath);

		node->name = pathName;

		auto nameLayer = node->layer["name"];
		if (nameLayer)
			((Text*)nameLayer->drawable)->text = pathName;
	}

	void UIAssetsFoldersTree::OnFoldersTreeNodeDblClick(UITreeNode* node)
	{
		AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)node->GetObject();
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->mPath);

		node->SetState("edit", true);

		auto editBox = (UIEditBox*)node->GetChild("nameEditBox");
		editBox->text = (String)pathName;
		editBox->SelectAll();
		editBox->UIWidget::Focus();
		editBox->ResetSroll();

		editBox->onChangeCompleted = [=](const WString& text) {

			String newPathAsset = o2FileSystem.GetParentPath(assetTreeNode->mPath) + "/" + text;
			o2Assets.RenameAsset(*assetTreeNode, text);

			node->SetState("edit", false);
			//node->UpdateView(false);

			o2EditorAssets.OpenFolder(newPathAsset);
		};
	}

	void UIAssetsFoldersTree::OnFoldersTreeClick(UITreeNode* node)
	{
		if (node)
		{
			AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)node->GetObject();
			o2EditorAssets.OpenFolder(assetTreeNode->mPath);
		}
		else o2EditorAssets.OpenFolder("");
	}

	void UIAssetsFoldersTree::OnFoldersTreeRightClick(UITreeNode* node)
	{
		o2UI.FocusWidget(this);
		mContextMenu->Show();
	}

	void UIAssetsFoldersTree::OnContextCopyPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.CopyAssets({ mCurrentPath });
	}

	void UIAssetsFoldersTree::OnContextCutPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.CutAssets({ mCurrentPath });
	}

	void UIAssetsFoldersTree::OnContextPastePressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.PasteAssets(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextDeletePressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.DeleteAssets({ mCurrentPath });
	}

	void UIAssetsFoldersTree::OnContextOpenPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.OpenAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextShowInExplorerPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.OpenAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextImportPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.ImportAssets(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreateFolderPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.CreateFolderAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreatePrefabPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.CreatePrefabAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreateScriptPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.CreateScriptAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreateAnimationPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		o2EditorAssets.CreateAnimationAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextExpandPressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		auto selectedObjects = mFoldersTree->GetSelectedObjects();

		for (auto obj : selectedObjects)
		{
			auto node = mFoldersTree->GetNode(obj);

// 			if (node)
// 				node->ExpandAll();
		}
	}

	void UIAssetsFoldersTree::OnContextCollapsePressed()
	{
		if (!mFoldersTree->IsFocused())
			return;

		auto selectedObjects = mFoldersTree->GetSelectedObjects();

		for (auto obj : selectedObjects)
		{
			auto node = mFoldersTree->GetNode(obj);

// 			if (node)
// 				node->CollapseAll();
		}
	}

}