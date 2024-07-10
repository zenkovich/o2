#pragma once

#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/Singleton.h"

namespace o2
{
	class Button;
	class EditBox;
	class Sprite;
	class Text;
	class Window;
}

using namespace o2;

namespace Editor
{
	FORWARD_CLASS_REF(MemoryAnalyzeTree);

	class MemoryAnalyzerWindow : public Singleton<MemoryAnalyzerWindow>
	{
	public:
		MemoryAnalyzerWindow();
		~MemoryAnalyzerWindow();

		static void Show(MemoryAnalyzer::MemoryNode* data);

	private:
        Ref<o2::Window> mWindow;
        Ref<EditBox>    mFilter;

		Ref<MemoryAnalyzeTree> mMemoryTree;

	private:
		// Initializes window and controls
		void InitializeWindow();
	};

	class MemoryAnalyzeTree : public Tree
	{
	public:
		// Default constructor
        MemoryAnalyzeTree(RefCounter* refCounter);

		// Copy-constructor
        MemoryAnalyzeTree(RefCounter* refCounter, const MemoryAnalyzeTree& other);

        // Copy-constructor
        MemoryAnalyzeTree(const MemoryAnalyzeTree& other);

		// Copy operator
		MemoryAnalyzeTree& operator=(const MemoryAnalyzeTree& other);

		// Initializes data tree
		void Initialize(MemoryAnalyzer::MemoryNode* data);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

        SERIALIZABLE(MemoryAnalyzeTree);
        CLONEABLE_REF(MemoryAnalyzeTree);

    private:
        WString mFilterStr; // Filtering string

		MemoryAnalyzer::MemoryNode* mRoot = nullptr; // Root data node

	private:
		// Updates visible nodes (calculates range and initializes nodes), enables editor mode
		void UpdateVisibleNodes() override;

		// Gets tree node from pool or creates new, enables editor mode
		Ref<TreeNode> CreateTreeNodeWidget() override;

		// Returns object's parent
		void* GetObjectParent(void* object) override;

		// Returns object's children
		Vector<void*> GetObjectChilds(void* object) override;

		// Returns debugging string for object
		String GetObjectDebug(void* object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(const Ref<TreeNode>& nodeWidget, void* object) override;

		// Called when tree node was double clicked
		void OnNodeDblClick(const Ref<TreeNode>& nodeWidget) override;

		// Called when list of selected objects was changed
		void OnNodesSelectionChanged(Vector<void*> objects) override;

		friend class MemoryAnalyzeTreeNode;
	};

	class MemoryAnalyzeTreeNode : public TreeNode
	{
	public:
		// Default constructor
        MemoryAnalyzeTreeNode(RefCounter* refCounter);

		// Copy-constructor
        MemoryAnalyzeTreeNode(RefCounter* refCounter, const MemoryAnalyzeTreeNode& other);

		// Copy operator
		MemoryAnalyzeTreeNode& operator=(const MemoryAnalyzeTreeNode& other);

		// Initializes node by data
		void Setup(MemoryAnalyzer::MemoryNode* data, bool owner, const Ref<MemoryAnalyzeTree>& tree);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

        SERIALIZABLE(MemoryAnalyzeTreeNode);
        CLONEABLE_REF(MemoryAnalyzeTreeNode);

    private:
        Ref<Text> mName;
        Ref<Text> mAddress;
        Ref<Text> mSize;

		MemoryAnalyzer::MemoryNode* mData; // Data node pointer

		WeakRef<MemoryAnalyzeTree> mTree; // Owner tree

	private:
		// Called on deserialization, initializes controls
		void OnDeserialized(const DataValue& node) override;

		// initializes controls and widgets
		void InitializeControls();

		friend class MemoryAnalyzeTree;
	};
}
// --- META ---

CLASS_BASES_META(Editor::MemoryAnalyzeTree)
{
    BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::MemoryAnalyzeTree)
{
    FIELD().PRIVATE().NAME(mFilterStr);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mRoot);
}
END_META;
CLASS_METHODS_META(Editor::MemoryAnalyzeTree)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*);
    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*, const MemoryAnalyzeTree&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const MemoryAnalyzeTree&);
    FUNCTION().PUBLIC().SIGNATURE(void, Initialize, MemoryAnalyzer::MemoryNode*);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateVisibleNodes);
    FUNCTION().PRIVATE().SIGNATURE(Ref<TreeNode>, CreateTreeNodeWidget);
    FUNCTION().PRIVATE().SIGNATURE(void*, GetObjectParent, void*);
    FUNCTION().PRIVATE().SIGNATURE(Vector<void*>, GetObjectChilds, void*);
    FUNCTION().PRIVATE().SIGNATURE(String, GetObjectDebug, void*);
    FUNCTION().PRIVATE().SIGNATURE(void, FillNodeDataByObject, const Ref<TreeNode>&, void*);
    FUNCTION().PRIVATE().SIGNATURE(void, OnNodeDblClick, const Ref<TreeNode>&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnNodesSelectionChanged, Vector<void*>);
}
END_META;

CLASS_BASES_META(Editor::MemoryAnalyzeTreeNode)
{
    BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::MemoryAnalyzeTreeNode)
{
    FIELD().PRIVATE().NAME(mName);
    FIELD().PRIVATE().NAME(mAddress);
    FIELD().PRIVATE().NAME(mSize);
    FIELD().PRIVATE().NAME(mData);
    FIELD().PRIVATE().NAME(mTree);
}
END_META;
CLASS_METHODS_META(Editor::MemoryAnalyzeTreeNode)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*);
    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*, const MemoryAnalyzeTreeNode&);
    FUNCTION().PUBLIC().SIGNATURE(void, Setup, MemoryAnalyzer::MemoryNode*, bool, const Ref<MemoryAnalyzeTree>&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, OnDeserialized, const DataValue&);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
}
END_META;
// --- END META ---
