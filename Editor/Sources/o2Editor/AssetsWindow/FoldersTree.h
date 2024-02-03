#pragma once

#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Tree.h"

using namespace o2;

namespace o2
{
    class ContextMenu;
}

namespace Editor
{
    class FoldersTree;

    // --------------------------
    // Assets folders tree widget
    // --------------------------
    class AssetsFoldersTree : public Widget, public KeyboardEventsListener
    {
    public:
        // Default constructor
        AssetsFoldersTree();

        // Copy-constructor
        AssetsFoldersTree(const AssetsFoldersTree& other);

        // Destructor
        ~AssetsFoldersTree();

        // Copy-operator
        AssetsFoldersTree& operator=(const AssetsFoldersTree& other);

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(AssetsFoldersTree);

    protected:
        Ref<Tree> mFoldersTree;             // Folders tree
        Ref<ContextMenu> mContextMenu;      // Context menu
        String mCurrentPath;                // Current viewing path

        bool mOpengingFolderFromThis = false;

    protected:
        // Selects and expands folder
        void SelectAndExpandFolder(const String& path);

        // Rebuilds folders tree
        void UpdateView();

        // Initializes assets context menu
        void InitializeContext();

        // Returns folder parent (for folders tree)
        RefBase GetFoldersTreeNodeParent(const RefBase& object);

        // Returns folder childs (for folders tree)
        Vector<RefBase> GetFoldersTreeNodeChilds(const RefBase& object);

        // Setups tree node by folder (for folders tree)
        void SetupFoldersTreeNode(const Ref<TreeNode>& node, const RefBase& object);

        // Called when folder item double clicked (for folders tree)
        void OnFoldersTreeNodeDblClick(const Ref<TreeNode>& node);

        // Called when folder item clicked (for folders tree)
        void OnFoldersTreeSelect(const Vector<RefBase>& nodes);

        // Called when folders tree clicked by right button
        void OnFoldersTreeRightClick(const Ref<TreeNode>& node);

        // Called when context copy pressed
        void OnContextCopyPressed();

        // Called when context cut pressed
        void OnContextCutPressed();

        // Called when context paste pressed
        void OnContextPastePressed();

        // Called when context delete pressed
        void OnContextDeletePressed();

        // Called when context open pressed
        void OnContextOpenPressed();

        // Called when context show in explorer pressed
        void OnContextShowInExplorerPressed();

        // Called when context create folder pressed
        void OnContextCreateFolderPressed();

        // Called when key was released
        void OnKeyReleased(const Input::Key& key) override;

        friend class AssetsWindow;
    };

    class FoldersTree : public Tree
    {
    public:
        SERIALIZABLE(FoldersTree);

    protected:
        // Updates visible nodes (calculates range and initializes nodes), enables editor mode
        void UpdateVisibleNodes() override;

        // Returns create menu category in editor
        static String GetCreateMenuCategory();
    };
}

// --- META ---

CLASS_BASES_META(Editor::AssetsFoldersTree)
{
    BASE_CLASS(o2::Widget);
    BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;

CLASS_FIELDS_META(Editor::AssetsFoldersTree)
{
    FIELD().PROTECTED().NAME(mFoldersTree).REF();
    FIELD().PROTECTED().NAME(mContextMenu).REF();
    FIELD().PROTECTED().NAME(mCurrentPath);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mOpengingFolderFromThis);
}
END_META;

CLASS_METHODS_META(Editor::AssetsFoldersTree)
{
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<Editor::AssetsFoldersTree>&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, SelectAndExpandFolder, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateView);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeContext);
    FUNCTION().PROTECTED().SIGNATURE(RefBase, GetFoldersTreeNodeParent, const RefBase&);
    FUNCTION().PROTECTED().SIGNATURE(Vector<RefBase>, GetFoldersTreeNodeChilds, const RefBase&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetupFoldersTreeNode, const Ref<TreeNode>&, const RefBase&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFoldersTreeNodeDblClick, const Ref<TreeNode>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFoldersTreeSelect, const Vector<RefBase>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFoldersTreeRightClick, const Ref<TreeNode>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCopyPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCutPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextPastePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextDeletePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextOpenPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextShowInExplorerPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnContextCreateFolderPressed);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
}
END_META;

CLASS_BASES_META(Editor::FoldersTree)
{
    BASE_CLASS(o2::Tree);
}
END_META;

CLASS_FIELDS_META(Editor::FoldersTree)
{
}
END_META;

CLASS_METHODS_META(Editor::FoldersTree)
{
}
END_META;#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& base)
{
  return std::dynamic_pointer_cast<Derived>(base);
}

class Tree
{
public:
  void UpdateVisibleNodes();

protected:
  static std::string GetCreateMenuCategory();
};

// --- END META ---