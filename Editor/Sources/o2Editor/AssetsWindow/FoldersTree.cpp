#include "o2Editor/stdafx.h"
#include "FoldersTree.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/FolderAsset.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/EditorScope.h"

namespace Editor
{
	AssetsFoldersTree::AssetsFoldersTree():
		Widget()
	{
		if (!UIManager::IsSingletonInitialzed())
			return;

		mFoldersTree = o2UI.CreateWidget<Tree>("folders");
		*mFoldersTree->layout = WidgetLayout::BothStretch();

		mFoldersTree->SetRearrangeType(Tree::RearrangeType::OnlyReparent);
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

	AssetsFoldersTree::AssetsFoldersTree(const AssetsFoldersTree& other):
		Widget(other)
	{
		mFoldersTree = FindChildByType<Tree>();
		RemoveChild(FindChildByType<ContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	AssetsFoldersTree::~AssetsFoldersTree()
	{}

	AssetsFoldersTree& AssetsFoldersTree::operator=(const AssetsFoldersTree& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void AssetsFoldersTree::CopyData(const Actor& otherActor)
	{
		const AssetsFoldersTree& other = dynamic_cast<const AssetsFoldersTree&>(otherActor);

		Widget::CopyData(other);

		mFoldersTree = FindChildByType<Tree>();
		RemoveChild(FindChildByType<ContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void AssetsFoldersTree::SelectAndExpandFolder(const String& path)
	{
		mFoldersTree->SelectAndHightlightObject(o2Assets.GetAssetsTree().Find(path));
		mCurrentPath = path;
	}

	void AssetsFoldersTree::UpdateView()
	{
		mFoldersTree->UpdateNodesView();
	}

	void AssetsFoldersTree::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItem("Import", [&]() { OnContextImportPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("New folder", [&]() { OnContextCreateFolderPressed(); }, ImageAssetRef(), ShortcutKeys('N', true));
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

	UnknownPtr AssetsFoldersTree::GetFoldersTreeNodeParent(UnknownPtr object)
	{
		if (!object)
			return UnknownPtr();

		AssetsTree::AssetInfo* assetTreeNode = (AssetsTree::AssetInfo*)(void*)object;
		return (UnknownPtr)(void*)(assetTreeNode->mParent);
	}

	Vector<UnknownPtr> AssetsFoldersTree::GetFoldersTreeNodeChilds(UnknownPtr object)
	{
		AssetsTree::AssetInfo* assetTreeNode = object;

		if (assetTreeNode)
		{
			return assetTreeNode->mChildren.
				FindAll([](AssetsTree::AssetInfo* x) { return x->mAssetType == &TypeOf(FolderAsset); }).
				Select<UnknownPtr>([](AssetsTree::AssetInfo* x) { return UnknownPtr(x); });
		}
		else
		{
			const AssetsTree& assetsTree = o2Assets.GetAssetsTree();

			return assetsTree.rootAssets.
				FindAll([](AssetsTree::AssetInfo* x) { return x->mAssetType == &TypeOf(FolderAsset); }).
				Select<UnknownPtr>([](AssetsTree::AssetInfo* x) { return UnknownPtr(x); });
		}
	}

	void AssetsFoldersTree::SetupFoldersTreeNode(TreeNode* node, UnknownPtr object)
	{
		AssetsTree::AssetInfo* assetTreeNode = (AssetsTree::AssetInfo*)(void*)object;
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->mPath);

		node->name = pathName;

		auto nameLayer = node->layer["name"];
		if (nameLayer)
			((Text*)nameLayer->GetDrawable())->text = pathName;
	}

	void AssetsFoldersTree::OnFoldersTreeNodeDblClick(TreeNode* node)
	{
		AssetsTree::AssetInfo* assetTreeNode = (AssetsTree::AssetInfo*)(void*)node->GetObject();
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->mPath);

		node->SetState("edit", true);

		auto editBox = (EditBox*)node->GetChild("nameEditBox");
		editBox->text = (String)pathName;
		editBox->SelectAll();
		editBox->Widget::Focus();
		editBox->ResetScroll();

		editBox->onChangeCompleted = [=](const WString& text) {

			String newPathAsset = o2FileSystem.GetParentPath(assetTreeNode->mPath) + "/" + text;
			o2Assets.RenameAsset(*assetTreeNode, text);

			node->SetState("edit", false);
			//node->UpdateView(false);

			o2EditorAssets.OpenFolder(newPathAsset.Trimed(" /\\"));
		};
	}

	void AssetsFoldersTree::OnFoldersTreeSelect(Vector<UnknownPtr> nodes)
	{
		if (mOpengingFolderFromThis)
			return;

		mOpengingFolderFromThis = true;

		if (nodes.Count() > 0)
		{
			AssetsTree::AssetInfo* assetTreeNode = nodes.Last();
			mCurrentPath = assetTreeNode->mPath;
			o2EditorAssets.mAssetsGridScroll->SetViewingPath(mCurrentPath);
		}
		else o2EditorAssets.OpenFolder("");

		mOpengingFolderFromThis = false;
	}

	void AssetsFoldersTree::OnFoldersTreeRightClick(TreeNode* node)
	{
		o2UI.FocusWidget(this);
		mContextMenu->Show();
	}

	void AssetsFoldersTree::OnContextCopyPressed()
	{
		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.CopyAssets({ mCurrentPath });
	}

	void AssetsFoldersTree::OnContextCutPressed()
	{
		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.CutAssets({ mCurrentPath });
	}

	void AssetsFoldersTree::OnContextPastePressed()
	{
		o2EditorAssets.PasteAssets(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextDeletePressed()
	{
		if (!mCurrentPath.IsEmpty())
			o2EditorAssets.DeleteAssets({ mCurrentPath });
	}

	void AssetsFoldersTree::OnContextOpenPressed()
	{
		o2EditorAssets.OpenAsset(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextShowInExplorerPressed()
	{
		o2EditorAssets.OpenAsset(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextImportPressed()
	{
		o2EditorAssets.ImportAssets(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextCreateFolderPressed()
	{
		o2EditorAssets.CreateFolderAsset(mCurrentPath, "New folder");
	}

	void AssetsFoldersTree::OnContextCreatePrefabPressed()
	{
		o2EditorAssets.CreatePrefabAsset(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextCreateScriptPressed()
	{
		o2EditorAssets.CreateScriptAsset(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextCreateAnimationPressed()
	{
		o2EditorAssets.CreateAnimationAsset(mCurrentPath);
	}

	void AssetsFoldersTree::OnContextExpandPressed()
	{
		auto selectedObjects = mFoldersTree->GetSelectedObjects();

		for (auto obj : selectedObjects)
		{
			auto node = mFoldersTree->GetNode(obj);

// 			if (node)
// 				node->ExpandAll();
		}
	}

	void AssetsFoldersTree::OnContextCollapsePressed()
	{
		auto selectedObjects = mFoldersTree->GetSelectedObjects();

		for (auto obj : selectedObjects)
		{
			auto node = mFoldersTree->GetNode(obj);

// 			if (node)
// 				node->CollapseAll();
		}
	}

	void AssetsFoldersTree::OnKeyReleased(const Input::Key& key)
	{
		if (o2UI.GetFocusedWidget() == mFoldersTree && key == VK_BACK)
		{
			if (mCurrentPath.CountOf("/") > 0)
				o2EditorAssets.OpenFolder(o2FileSystem.GetParentPath(mCurrentPath));
			else
				o2EditorAssets.OpenFolder("");
		}
	}

	void FoldersTree::UpdateVisibleNodes()
	{
		PushEditorScopeOnStack scope;
		Tree::UpdateNodesStructure();
	}

}

DECLARE_CLASS(Editor::AssetsFoldersTree);

DECLARE_CLASS(Editor::FoldersTree);
