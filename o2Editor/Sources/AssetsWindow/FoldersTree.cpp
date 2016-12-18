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

		mFoldersTree->getObjectParentDelegate = Function<UnknownPtr(UnknownPtr)>(this, &UIAssetsFoldersTree::GetFoldersTreeNodeParent);
		mFoldersTree->getObjectChildrenDelegate = Function<Vector<UnknownPtr>(UnknownPtr)>(this, &UIAssetsFoldersTree::GetFoldersTreeNodeChilds);
		mFoldersTree->fillNodeDataByObjectDelegate = Function<void(UITreeNode*, UnknownPtr)>(this, &UIAssetsFoldersTree::SetupFoldersTreeNode);
		mFoldersTree->onNodeDoubleClicked = Function<void(UITreeNode*)>(this, &UIAssetsFoldersTree::OnFoldersTreeNodeDblClick);
		mFoldersTree->onObjectsSelectionChanged = Function<void(Vector<UnknownPtr>)>(this, &UIAssetsFoldersTree::OnFoldersTreeSelect);
		mFoldersTree->onNodeRightButtonClicked = Function<void(UITreeNode*)>(this, &UIAssetsFoldersTree::OnFoldersTreeRightClick);
		mFoldersTree->UpdateNodesView();

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
		editBox->ResetScroll();

		editBox->onChangeCompleted = [=](const WString& text) {

			String newPathAsset = o2FileSystem.GetParentPath(assetTreeNode->mPath) + "/" + text;
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
			mCurrentPath = assetTreeNode->mPath;
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
 
CLASS_META(Editor::UIAssetsFoldersTree)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::KeyboardEventsListener);

	PROTECTED_FIELD(mFoldersTree);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mCurrentPath);
	PROTECTED_FIELD(mOpengingFolderFromThis);

	PROTECTED_FUNCTION(void, SelectAndExpandFolder, const String&);
	PROTECTED_FUNCTION(void, UpdateView);
	PROTECTED_FUNCTION(void, InitializeContext);
	PROTECTED_FUNCTION(UnknownPtr, GetFoldersTreeNodeParent, UnknownPtr);
	PROTECTED_FUNCTION(Vector<UnknownPtr>, GetFoldersTreeNodeChilds, UnknownPtr);
	PROTECTED_FUNCTION(void, SetupFoldersTreeNode, UITreeNode*, UnknownPtr);
	PROTECTED_FUNCTION(void, OnFoldersTreeNodeDblClick, UITreeNode*);
	PROTECTED_FUNCTION(void, OnFoldersTreeSelect, Vector<UnknownPtr>);
	PROTECTED_FUNCTION(void, OnFoldersTreeRightClick, UITreeNode*);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextOpenPressed);
	PROTECTED_FUNCTION(void, OnContextShowInExplorerPressed);
	PROTECTED_FUNCTION(void, OnContextImportPressed);
	PROTECTED_FUNCTION(void, OnContextCreateFolderPressed);
	PROTECTED_FUNCTION(void, OnContextCreatePrefabPressed);
	PROTECTED_FUNCTION(void, OnContextCreateScriptPressed);
	PROTECTED_FUNCTION(void, OnContextCreateAnimationPressed);
	PROTECTED_FUNCTION(void, OnContextExpandPressed);
	PROTECTED_FUNCTION(void, OnContextCollapsePressed);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
}
END_META;
 