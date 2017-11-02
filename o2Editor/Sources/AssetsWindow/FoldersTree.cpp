#include "FoldersTree.h"

#include "Assets/Assets.h"
#include "Assets/AssetsTree.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Render/Sprite.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "Utils/FileSystem/FileSystem.h"

namespace Editor
{
	UIAssetsFoldersTree::UIAssetsFoldersTree():
		UIWidget()
	{
		if (!UIManager::IsSingletonInitialzed())
			return;

		mFoldersTree = o2UI.CreateWidget<UITree>("folders");
		*mFoldersTree->layout = UIWidgetLayout::BothStretch();

		mFoldersTree->SetRearrangeType(UITree::RearrangeType::OnlyReparent);
		mFoldersTree->SetMultipleSelectionAvailable(false);

		mFoldersTree->getObjectParentDelegate = THIS_FUNC(GetFoldersTreeNodeParent);
		mFoldersTree->getObjectChildrenDelegate = THIS_FUNC(GetFoldersTreeNodeChilds);
		mFoldersTree->fillNodeDataByObjectDelegate = THIS_FUNC(SetupFoldersTreeNode);
		mFoldersTree->onNodeDoubleClicked = THIS_FUNC(OnFoldersTreeNodeDblClick);
		mFoldersTree->onObjectsSelectionChanged = THIS_FUNC(OnFoldersTreeSelect);
		mFoldersTree->onNodeRightButtonClicked = THIS_FUNC(OnFoldersTreeRightClick);
		mFoldersTree->UpdateNodesView();

		AddChild(mFoldersTree);

		InitializeContext();
	}

	UIAssetsFoldersTree::UIAssetsFoldersTree(const UIAssetsFoldersTree& other):
		UIWidget(other)
	{
		mFoldersTree = GetChildByType<UITree>();
		RemoveChild(GetChildByType<UIContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIAssetsFoldersTree::~UIAssetsFoldersTree()
	{}

	UIAssetsFoldersTree& UIAssetsFoldersTree::operator=(const UIAssetsFoldersTree& other)
	{
		UIWidget::operator=(other);

		mFoldersTree = GetChildByType<UITree>();
		RemoveChild(GetChildByType<UIContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIAssetsFoldersTree::SelectAndExpandFolder(const String& path)
	{
		mFoldersTree->SelectAndHightlightObject(o2Assets.GetAssetsTree().FindAsset(path));
		mCurrentPath = path;
	}

	void UIAssetsFoldersTree::UpdateView()
	{
		mFoldersTree->UpdateNodesView();
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
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, ImageAssetRef(), ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Expand all", [&]() { OnContextExpandPressed(); });
		mContextMenu->AddItem("Collapse all", [&]() { OnContextCollapsePressed(); });

		mFoldersTree->onFocused = [&]() { mContextMenu->SetItemsMaxPriority(); };
		mFoldersTree->onUnfocused = [&]() { mContextMenu->SetItemsMinPriority(); };

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
				FindAll([](AssetTree::AssetNode* x) { return x->assetType == &TypeOf(FolderAsset); }).
				Select<UnknownPtr>([](AssetTree::AssetNode* x) { return UnknownPtr(x); });
		}
		else
		{
			const AssetTree& assetsTree = o2Assets.GetAssetsTree();

			return assetsTree.mRootAssets.
				FindAll([](AssetTree::AssetNode* x) { return x->assetType == &TypeOf(FolderAsset); }).
				Select<UnknownPtr>([](AssetTree::AssetNode* x) { return UnknownPtr(x); });
		}
	}

	void UIAssetsFoldersTree::SetupFoldersTreeNode(UITreeNode* node, UnknownPtr object)
	{
		AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)object;
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->path);

		node->name = pathName;

		auto nameLayer = node->layer["name"];
		if (nameLayer)
			((Text*)nameLayer->drawable)->text = pathName;
	}

	void UIAssetsFoldersTree::OnFoldersTreeNodeDblClick(UITreeNode* node)
	{
		AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)node->GetObject();
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->path);

		node->SetState("edit", true);

		auto editBox = (UIEditBox*)node->GetChild("nameEditBox");
		editBox->text = (String)pathName;
		editBox->SelectAll();
		editBox->UIWidget::Focus();
		editBox->ResetScroll();

		editBox->onChangeCompleted = [=](const WString& text) {

			String newPathAsset = o2FileSystem.GetParentPath(assetTreeNode->path) + "/" + text;
			o2Assets.RenameAsset(*assetTreeNode, text);

			node->SetState("edit", false);
			//node->UpdateView(false);

			o2EditorAssets.OpenFolder(newPathAsset);
		};
	}

	void UIAssetsFoldersTree::OnFoldersTreeSelect(Vector<UnknownPtr> nodes)
	{
		if (mOpengingFolderFromThis)
			return;

		mOpengingFolderFromThis = true;

		if (nodes.Count() > 0)
		{
			AssetTree::AssetNode* assetTreeNode = nodes.Last();
			mCurrentPath = assetTreeNode->path;
			o2EditorAssets.mAssetsGridScroll->SetViewingPath(mCurrentPath);
		}
		else o2EditorAssets.OpenFolder("");

		mOpengingFolderFromThis = false;
	}

	void UIAssetsFoldersTree::OnFoldersTreeRightClick(UITreeNode* node)
	{
		o2UI.FocusWidget(this);
		mContextMenu->Show();
	}

	void UIAssetsFoldersTree::OnContextCopyPressed()
	{
		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.CopyAssets({ mCurrentPath });
	}

	void UIAssetsFoldersTree::OnContextCutPressed()
	{
		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.CutAssets({ mCurrentPath });
	}

	void UIAssetsFoldersTree::OnContextPastePressed()
	{
		o2EditorAssets.PasteAssets(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextDeletePressed()
	{
		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.DeleteAssets({ mCurrentPath });
	}

	void UIAssetsFoldersTree::OnContextOpenPressed()
	{
		o2EditorAssets.OpenAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextShowInExplorerPressed()
	{
		o2EditorAssets.OpenAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextImportPressed()
	{
		o2EditorAssets.ImportAssets(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreateFolderPressed()
	{
		o2EditorAssets.CreateFolderAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreatePrefabPressed()
	{
		o2EditorAssets.CreatePrefabAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreateScriptPressed()
	{
		o2EditorAssets.CreateScriptAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextCreateAnimationPressed()
	{
		o2EditorAssets.CreateAnimationAsset(mCurrentPath);
	}

	void UIAssetsFoldersTree::OnContextExpandPressed()
	{
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
		auto selectedObjects = mFoldersTree->GetSelectedObjects();

		for (auto obj : selectedObjects)
		{
			auto node = mFoldersTree->GetNode(obj);

// 			if (node)
// 				node->CollapseAll();
		}
	}

	void UIAssetsFoldersTree::OnKeyReleased(const Input::Key& key)
	{
		if (o2UI.GetFocusedWidget() == mFoldersTree && key == VK_BACK)
		{
			if (mCurrentPath.CountOf("/") > 0)
				o2EditorAssets.OpenFolder(o2FileSystem.GetParentPath(mCurrentPath));
			else
				o2EditorAssets.OpenFolder("");
		}
	}

}

DECLARE_CLASS(Editor::UIAssetsFoldersTree);
