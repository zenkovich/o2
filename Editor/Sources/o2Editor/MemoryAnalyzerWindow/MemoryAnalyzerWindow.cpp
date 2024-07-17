#include "o2Editor/stdafx.h"
#include "MemoryAnalyzerWindow.h"

#include "o2/Render/Sprite.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2/Utils/Editor/Attributes/AnimatableAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UIRoot.h"

DECLARE_SINGLETON(Editor::MemoryAnalyzerWindow);

namespace Editor
{	
	MemoryAnalyzerWindow::MemoryAnalyzerWindow()
	{
		InitializeWindow();
	}

	MemoryAnalyzerWindow::~MemoryAnalyzerWindow()
	{}

	void MemoryAnalyzerWindow::Show(MemoryAnalyzer::MemoryNode* data)
	{
		Instance().mMemoryTree->Initialize(data);
		Instance().mWindow->ShowModal();
	}

	void MemoryAnalyzerWindow::InitializeWindow()
	{
		mWindow = DynamicCast<o2::Window>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Memory analyzer")));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 17));
		mWindow->SetViewLayout(Layout::BothStretch(-2, -2, 0, 20));
		mWindow->SetIcon(mmake<Sprite>("ui/UI4_tree_wnd_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1)));

		auto upPanel = mmake<Widget>();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, -1);
		upPanel->AddLayer("back", mmake<Sprite>("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		auto searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		upPanel->AddChild(searchButton);

		mFilter = o2UI.CreateWidget<EditBox>("backless");
		*mFilter->layout = WidgetLayout::BothStretch(19, 2, 0, -4);
		mFilter->onChanged += [&](const WString& filter) {  };
		upPanel->AddChild(mFilter);

		mWindow->AddChild(upPanel);

		mMemoryTree = o2UI.CreateWidget<MemoryAnalyzeTree>();
		*mMemoryTree->layout = WidgetLayout::BothStretch(0, 0, 0, 17);
		mMemoryTree->SetRearrangeType(Tree::RearrangeType::Disabled);

		mWindow->AddChild(mMemoryTree);

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(400, 600);

		mWindow->GetBackCursorListener().onCursorReleased = [=](const Input::Cursor& c) { mWindow->Hide(); };
	}

    MemoryAnalyzeTree::MemoryAnalyzeTree(RefCounter* refCounter) :
		Tree(refCounter)
	{}

	MemoryAnalyzeTree::MemoryAnalyzeTree(RefCounter* refCounter, const MemoryAnalyzeTree& other):
		Tree(refCounter, other)
	{}

    MemoryAnalyzeTree::MemoryAnalyzeTree(const MemoryAnalyzeTree& other):
		MemoryAnalyzeTree(nullptr, other)
    {}

    MemoryAnalyzeTree& MemoryAnalyzeTree::operator=(const MemoryAnalyzeTree& other)
	{
		Tree::operator=(other);
		return *this;
	}

	void MemoryAnalyzeTree::Initialize(MemoryAnalyzer::MemoryNode* data)
	{
		mFilterStr = "";
		mRoot = data;

		UpdateNodesStructure();
		UpdateVisibleNodes();
	}

	String MemoryAnalyzeTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void MemoryAnalyzeTree::UpdateVisibleNodes()
	{
		PushEditorScopeOnStack scope;
		Tree::UpdateVisibleNodes();
	}

	Ref<TreeNode> MemoryAnalyzeTree::CreateTreeNodeWidget()
	{
		PushEditorScopeOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	void* MemoryAnalyzeTree::GetObjectParent(void* object)
	{
		return ((MemoryAnalyzer::MemoryNode*)object)->mainParent;
	}

	Vector<void*> MemoryAnalyzeTree::GetObjectChilds(void* object)
	{
		Vector<void*> result;
		if (object)
		{
			for (auto& child : ((MemoryAnalyzer::MemoryNode*)object)->children)
                result.push_back(child);
		}
        else
        {
            for (auto& child : mRoot->children)
                result.push_back(child);
        }

		return result;
	}

	String MemoryAnalyzeTree::GetObjectDebug(void* object)
	{
		return "unknown";
	}

	void MemoryAnalyzeTree::FillNodeDataByObject(const Ref<TreeNode>& nodeWidget, void* object)
	{
		auto propertyNode = DynamicCast<MemoryAnalyzeTreeNode>(nodeWidget);
		propertyNode->Setup((MemoryAnalyzer::MemoryNode*)object, Ref(this));
	}

	void MemoryAnalyzeTree::OnNodeDblClick(const Ref<TreeNode>& nodeWidget)
	{
		if (!nodeWidget)
			return;

		auto propertyNode = DynamicCast<MemoryAnalyzeTreeNode>(nodeWidget);
	}

	void MemoryAnalyzeTree::OnNodesSelectionChanged(Vector<void*> objects)
	{

	}

	MemoryAnalyzeTreeNode::MemoryAnalyzeTreeNode(RefCounter* refCounter):
        TreeNode(refCounter)
	{
		InitializeControls();
	}

	MemoryAnalyzeTreeNode::MemoryAnalyzeTreeNode(RefCounter* refCounter, const MemoryAnalyzeTreeNode& other) :
		TreeNode(refCounter, other)
	{
		InitializeControls();
	}

	MemoryAnalyzeTreeNode& MemoryAnalyzeTreeNode::operator=(const MemoryAnalyzeTreeNode& other)
	{
		TreeNode::operator=(other);
		InitializeControls();
		return *this;
	}

	void MemoryAnalyzeTreeNode::Setup(MemoryAnalyzer::MemoryNode* data, const Ref<MemoryAnalyzeTree>& tree)
	{
		String address = String::Format("%p", data->memory);

		String size;
		auto summarySize = data->summarySize;
		if (summarySize > 1024 * 1024)
            size = String::Format("%.2f Mb", summarySize / (1024.0 * 1024.0));
        else if (summarySize > 1024)
            size = String::Format("%.2f Kb", summarySize / 1024.0);
        else
            size = String::Format("%d b", summarySize);

		bool isOwner = data->mainParent == nullptr;

        String type = data->type;
		type.ReplaceAll("class ", "");
		type.ReplaceAll("> >", ">>");
		
		auto& name = data->name;

		if (name.empty())
			mName->text = type;
		else if (type.IsEmpty())
			mName->text = (String)name;
		else
			mName->text = type + " " + (String)name;

		//mName->transparency = owner ? 1.0f : 0.5f;

		mAddress->text = address;
		mSize->text = size;

		mData = data;
		mTree = tree;
	}

	String MemoryAnalyzeTreeNode::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void MemoryAnalyzeTreeNode::OnDeserialized(const DataValue& node)
	{
		TreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void MemoryAnalyzeTreeNode::InitializeControls()
    {
        mName = GetLayerDrawable<Text>("root/name");
        mAddress = GetLayerDrawable<Text>("root/address");
		mSize = GetLayerDrawable<Text>("root/size");

		auto tree = mTree.Lock();
	}
}

DECLARE_TEMPLATE_CLASS(o2::LinkRef<Editor::MemoryAnalyzeTreeNode>);
// --- META ---

DECLARE_CLASS(Editor::MemoryAnalyzeTree, Editor__MemoryAnalyzeTree);

DECLARE_CLASS(Editor::MemoryAnalyzeTreeNode, Editor__MemoryAnalyzeTreeNode);
// --- END META ---
