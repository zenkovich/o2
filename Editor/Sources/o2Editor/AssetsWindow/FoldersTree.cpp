Ref<AssetInfo> assetTreeNode = DynamicCast<AssetInfo>(node->GetObject());
String pathName = o2FileSystem.GetPathWithoutDirectories(assetTreeNode->path);

node->SetState("edit", true);

#include <iostream>
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T>
using DynamicCast = std::dynamic_pointer_cast<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class WString
{
};

class String
{
public:
    String(const char* str)
    {

    }
};

class FolderAsset
{
};

class EditBox
{
public:
    String text;

    void SelectAll()
    {

    }

    void Widget::Focus()
    {

    }

    void ResetScroll()
    {

    }
};

class TreeNode
{
};

namespace Editor
{
    class AssetInfo
    {
    public:
        String path;
        Ref<void> meta;
    };

    class AssetsFoldersTree
    {
    private:
        Ref<EditBox> mCurrentPath;
        bool mOpengingFolderFromThis;
        Ref<void> mContextMenu;

    public:
        void Init()
        {

        }

        void OnFoldersTreeSelect(const Vector<Ref<void>>& nodes)
        {
            if (mOpengingFolderFromThis)
                return;

            mOpengingFolderFromThis = true;

            if (nodes.size() > 0)
            {
                Ref<AssetInfo> assetTreeNode = DynamicCast<AssetInfo>(nodes.back());
                mCurrentPath = assetTreeNode->path;
                o2EditorAssets.mAssetsGridScroll->SetViewingPath(mCurrentPath);
            }
            else
                o2EditorAssets.OpenFolder("");

            mOpengingFolderFromThis = false;
        }

        void OnFoldersTreeRightClick(Ref<TreeNode> node)
        {
            o2UI.FocusWidget(mmake<AssetsFoldersTree>());
            mContextMenu->Show();
        }

        void OnContextCopyPressed()
        {
            if (!mCurrentPath.IsNull())
                o2EditorAssets.CopyAssets({ mCurrentPath });
        }

        void OnContextCutPressed()
        {
            if (!mCurrentPath.IsNull())
                o2EditorAssets.CutAssets({ mCurrentPath });
        }

        void OnContextPastePressed()
        {
            o2EditorAssets.PasteAssets(mCurrentPath);
        }

        void OnContextDeletePressed()
        {
            if (!mCurrentPath.IsNull())
            {
                auto deletingPath = mCurrentPath;

                o2EditorAssets.OpenFolder("");
                o2EditorAssets.DeleteAssets({ deletingPath });
            }
        }

        void OnContextOpenPressed()
        {
            o2EditorAssets.OpenAsset(mCurrentPath);
        }

        void OnContextShowInExplorerPressed()
        {
            o2EditorAssets.OpenAsset(mCurrentPath);
        }

        void OnContextCreateFolderPressed()
        {
            o2EditorAssets.mAssetsGridScroll->CreateAsset(mmake<FolderAsset>());
        }

        void OnKeyReleased(const Input::Key& key)
        {
            if (o2UI.GetFocusedWidget() == mFoldersTree && key == VK_BACK)
            {
                if (mCurrentPath.CountOf("/") > 0)
                    o2EditorAssets.OpenFolder(o2FileSystem.GetParentPath(mCurrentPath));
                else
                    o2EditorAssets.OpenFolder("");
            }
        }
    };

    class FoldersTree : public Tree
    {
    public:
        void UpdateVisibleNodes() override
        {
            PushEditorScopeOnStack scope;
            Tree::UpdateNodesStructure();
        }

        String GetCreateMenuCategory() override
        {
            return "UI/Editor";
        }
    };

}

// --- META ---

DECLARE_CLASS(Editor::AssetsFoldersTree, Editor__AssetsFoldersTree);

DECLARE_CLASS(Editor::FoldersTree, Editor__FoldersTree);
// --- END META ---

int main()
{
    return 0;
}