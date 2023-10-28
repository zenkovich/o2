#include "o2Editor/stdafx.h"
#include "FoldersTree.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/Types/FolderAsset.h"
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
#include "o2/Utils/Editor/EditorScope.h"

namespace Editor
{
	AssetsFoldersTree::AssetsFoldersTree() :
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

	AssetsFoldersTree::AssetsFoldersTree(const AssetsFoldersTree& other) :
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

	String AssetsFoldersTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	AssetsFoldersTree& AssetsFoldersTree::operator=(const AssetsFoldersTree& other)
	{
		Widget::operator=(other);

		mFoldersTree = FindChildByType<Tree>();
		RemoveChild(FindChildByType<ContextMenu>());

		InitializeContext();

		RetargetStatesAnimations();
		SetLayoutDirty();

		return *this;
	}

	void AssetsFoldersTree::SelectAndExpandFolder(const String& path)
	{
		mOpengingFolderFromThis = true;

		mFoldersTree->SelectAndHighlightObject(o2Assets.GetAssetsTree().Find(path));
		mCurrentPath = path;

		mOpengingFolderFromThis = false;
	}

	void AssetsFoldersTree::UpdateView()
	{
		mFoldersTree->UpdateNodesView();
	}

	void AssetsFoldersTree::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("New folder", [&]() { OnContextCreateFolderPressed(); }, ImageAssetRef(), ShortcutKeys('N', true));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, ImageAssetRef(), ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));

		mFoldersTree->onFocused = [&]() { mContextMenu->SetItemsMaxPriority(); };
		mFoldersTree->onUnfocused = [&]() { mContextMenu->SetItemsMinPriority(); };

		AddChild(mContextMenu);
	}

	void* AssetsFoldersTree::GetFoldersTreeNodeParent(void* object)
	{
		if (!object)
			return nullptr;

		AssetInfo* assetTreeNode = (AssetInfo*)(void*)object;
		return (void*)(void*)(assetTreeNode->parent);
	}

	Vector<void*> AssetsFoldersTree::GetFoldersTreeNodeChilds(void* object)
	{
		AssetInfo* assetTreeNode = (AssetInfo*)object;

		if (assetTreeNode)
		{
			return assetTreeNode->GetChildren()
				.FindAll([](AssetInfo* x) { return x->meta->GetAssetType() == &TypeOf(FolderAsset); })
				.Sorted([](AssetInfo* a, AssetInfo* b) { return a->path < b->path; })
				.Convert<void*>([](AssetInfo* x) { return (void*)x; });
		}
		else
		{
			const AssetsTree& assetsTree = o2Assets.GetAssetsTree();

			return assetsTree.rootAssets
				.FindAll([](AssetInfo* x) { return x->meta->GetAssetType() == &TypeOf(FolderAsset); })
				.Sorted([](AssetInfo* a, AssetInfo* b) { return a->path < b->path; })
				.Convert<void*>([](AssetInfo* x) { return (void*)x; });
		}
	}

	void AssetsFoldersTree::SetupFoldersTreeNode(TreeNode* node, void* object)
	{
		AssetInfo* assetTreeNode = (AssetInfo*)(void*)object;
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->path);

		node->name = pathName;

		auto nameLayer = node->layer["name"];
		if (nameLayer)
			((Text*)nameLayer->GetDrawable())->text = pathName;
	}

	void AssetsFoldersTree::OnFoldersTreeNodeDblClick(TreeNode* node)
	{
		AssetInfo* assetTreeNode = (AssetInfo*)(void*)node->GetObject();
		String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->path);

		node->SetState("edit", true);

		auto editBox = (EditBox*)node->GetChild("nameEditBox");
		editBox->text = (String)pathName;
		editBox->SelectAll();
		editBox->Widget::Focus();
		editBox->ResetScroll();

		editBox->onChangeCompleted = [=](const WString& text) {
			String newPathAsset = o2FileSystem.GetParentPath(assetTreeNode->path) + "/" + text;
			o2Assets.RenameAsset(assetTreeNode->meta->ID(), text);

			node->SetState("edit", false);
			//node->UpdateView(false);

			o2EditorAssets.OpenFolder(newPathAsset.Trimed(" /\\"));
		};
	}

	void AssetsFoldersTree::OnFoldersTreeSelect(Vector<void*> nodes)
	{
		if (mOpengingFolderFromThis)
			return;

		mOpengingFolderFromThis = true;

		if (nodes.Count() > 0)
		{
			AssetInfo* assetTreeNode = (AssetInfo*)nodes.Last();
			mCurrentPath = assetTreeNode->path;
			o2EditorAssets.mAssetsGridScroll->SetViewingPath(mCurrentPath);
		}
		else
			o2EditorAssets.OpenFolder("");

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

	void AssetsFoldersTree::OnContextCreateFolderPressed()
	{
		o2EditorAssets.mAssetsGridScroll->CreateAsset(&TypeOf(FolderAsset));
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

	String FoldersTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

}
// --- META ---

DECLARE_CLASS(Editor::AssetsFoldersTree);

DECLARE_CLASS(Editor::FoldersTree);
// --- END META ---
