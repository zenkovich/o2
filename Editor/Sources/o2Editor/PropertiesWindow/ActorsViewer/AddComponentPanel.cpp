#include "o2Editor/stdafx.h"
#include "AddComponentPanel.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/ActorViewer.h"

namespace Editor
{
	AddComponentPanel::AddComponentPanel(ActorViewer* viewer) :
		mViewer(viewer)
	{
		AddLayer("border", mnew Sprite("ui/UI4_shadow_separator.png"), Layout::HorStretch(VerAlign::Top, -2, -2, 5, -4));

		mFilterBox = o2UI.CreateEditBox("actor head name");
		mAddButton = o2UI.CreateWidget<Button>("add component");
		mTree = o2UI.CreateWidget<ComponentsTree>();

		*mFilterBox->layout = WidgetLayout::HorStretch(VerAlign::Top, 10, 40, 20, 10);
		*mAddButton->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(40, 40), Vec2F());
		*mTree->layout = WidgetLayout::BothStretch(0, 0, 0, 40);

		AddChildren({ mFilterBox, mAddButton, mTree });

		mFilterBox->onChanged = [&](const WString& filter) { mTree->SetFilter(filter); };
		mAddButton->onClick = THIS_FUNC(OnAddPressed);

		mTree->onNodeDoubleClicked = THIS_FUNC(OnNodeDblClick);
		mTree->Refresh();
	}

	AddComponentPanel::AddComponentPanel()
	{}

	void AddComponentPanel::Draw()
	{
		Widget::Draw();
		CursorEventsArea::OnDrawn();
	}

	EditBox* AddComponentPanel::GetFilter() const
	{
		return mFilterBox;
	}

	ComponentsTree* AddComponentPanel::GetTree() const
	{
		return mTree;
	}

	bool AddComponentPanel::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	bool AddComponentPanel::IsInputTransparent() const
	{
		return true;
	}

	void AddComponentPanel::OnAddPressed()
	{
		if (mTree->GetSelectedObjects().IsEmpty())
			return;

		ComponentsTree::NodeData* node = (ComponentsTree::NodeData*)(mTree->GetSelectedObjects().Last());

		if (!node->type)
			return;

		const ObjectType* objType = dynamic_cast<const ObjectType*>(node->type);

		for (auto actor : mViewer->mTargetActors)
		{
			auto comp = dynamic_cast<Component*>(objType->DynamicCastToIObject(objType->CreateSample()));
			actor->AddComponent(comp);
			comp->OnAddedFromEditor();
		}

		mViewer->SetTargets(mViewer->mTargetActors.Convert<IObject*>([](auto x) { return dynamic_cast<IObject*>(x); }));
	}

	void AddComponentPanel::OnNodeDblClick(TreeNode* nodeWidget)
	{
		if (!nodeWidget)
			return;

		auto node = dynamic_cast<ComponentsTreeNode*>(nodeWidget);
		if (!node->data->type)
			return;
		const ObjectType* objType = dynamic_cast<const ObjectType*>(node->data->type);

		for (auto actor : mViewer->mTargetActors)
		{
			auto comp = dynamic_cast<Component*>(objType->DynamicCastToIObject(objType->CreateSample()));
			actor->AddComponent(comp);
			comp->OnAddedFromEditor();
		}

		mViewer->SetTargets(mViewer->mTargetActors.Convert<IObject*>([](auto x) { return dynamic_cast<IObject*>(x); }));
	}

	void AddComponentPanel::OnKeyReleased(const Input::Key& key)
	{
		if (key.keyCode != VK_RETURN)
			return; // haha

		if (mFilterBox->IsFocused() || mTree->IsFocused())
			OnAddPressed();
	}

	ComponentsTree::ComponentsTree() :
		Tree()
	{}

	ComponentsTree::ComponentsTree(const ComponentsTree& other) :
		Tree(other)
	{}

	ComponentsTree& ComponentsTree::operator=(const ComponentsTree& other)
	{
		Tree::operator=(other);
		return *this;
	}

	void ComponentsTree::Refresh()
	{
		mRoot.Clear();

		auto availableComponentsTypes = TypeOf(Component).GetDerivedTypes();
		availableComponentsTypes.Remove(&TypeOf(DrawableComponent));

		String filterStrLower = mFilterStr.ToLowerCase();

		for (auto type : availableComponentsTypes)
		{
			String name = type->InvokeStatic<String>("GetName");
			if (name.IsEmpty())
				name = type->GetName();

			if (!filterStrLower.IsEmpty())
			{
				if (name.ToLowerCase().Contains(filterStrLower))
					mRoot.AddChild(name, type)->icon = type->InvokeStatic<String>("GetIcon");
			}
			else
			{
				String category = type->InvokeStatic<String>("GetCategory");
				NodeData* it = &mRoot;
				while (!category.IsEmpty())
				{
					int fnd = category.Find('/');
					String subStr = category.SubStr(0, fnd);
					category = fnd != -1 ? category.SubStr(fnd + 1) : "";

					auto nextIt = it->children.FindOrDefault([&](NodeData* x) { return x->name == subStr; });
					if (!nextIt)
						nextIt = it->AddChild(subStr, nullptr);

					it = nextIt;
				}

				it->AddChild(name, type)->icon = type->InvokeStatic<String>("GetIcon");
			}
		}

		UpdateNodesStructure();
		UpdateVisibleNodes();
	}

	void ComponentsTree::SetFilter(const WString& filter)
	{
		mFilterStr = filter;
		Refresh();

		String lowerFilter = mFilterStr.ToLowerCase();
		int minDist = INT_MAX;
		NodeData* filtered = nullptr;
		Function<void(NodeData*)> recursiveSearch = [&lowerFilter, &recursiveSearch, &minDist, &filtered](NodeData* node)
		{
			if (node->type != nullptr)
			{
				int dist = node->name.ToLowerCase().Find(lowerFilter);
				if (dist >= 0 && dist < minDist)
				{
					minDist = dist;
					filtered = node;
				}
			}

			for (auto child : node->children)
				recursiveSearch(child);
		};

		recursiveSearch(&mRoot);

		if (filtered)
			SelectAndHighlightObject(filtered);
	}

	void ComponentsTree::UpdateVisibleNodes()
	{
		PushEditorScopeOnStack scope;
		Tree::UpdateVisibleNodes();
	}

	TreeNode* ComponentsTree::CreateTreeNodeWidget()
	{
		PushEditorScopeOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	void* ComponentsTree::GetObjectParent(void* object)
	{
		auto node = (NodeData*)object;
		if (node->parent == &mRoot)
			return (NodeData*)nullptr;

		return node->parent;
	}

	Vector<void*> ComponentsTree::GetObjectChilds(void* object)
	{
		if (object)
			return ((NodeData*)object)->children.Cast<void*>();

		return mRoot.children.Cast<void*>();
	}

	String ComponentsTree::GetObjectDebug(void* object)
	{
		if (object)
			return ((NodeData*)object)->path;

		return "";
	}

	void ComponentsTree::FillNodeDataByObject(TreeNode* nodeWidget, void* object)
	{
		auto propertyNode = dynamic_cast<ComponentsTreeNode*>(nodeWidget);
		propertyNode->Setup((NodeData*)object, this);
	}

	void ComponentsTree::OnDeserialized(const DataValue& node)
	{
		Tree::OnDeserialized(node);
	}

	ComponentsTreeNode::ComponentsTreeNode():
		TreeNode()
	{
		InitializeControls();
	}

	ComponentsTreeNode::ComponentsTreeNode(const ComponentsTreeNode& other) :
		TreeNode(other)
	{
		InitializeControls();
	}

	ComponentsTreeNode& ComponentsTreeNode::operator=(const ComponentsTreeNode& other)
	{
		TreeNode::operator=(other);
		InitializeControls();
		return *this;
	}

	void ComponentsTreeNode::Setup(ComponentsTree::NodeData* data, ComponentsTree* tree)
	{
		mName->text = data->name;

		*mIcon = Sprite(data->icon);
		mIcon->enabled = data->type != nullptr;
		mIcon->color = Color4(44, 62, 80);

		this->data = data;
		mTree = tree;
	}

	void ComponentsTreeNode::CopyData(const Actor& otherActor)
	{
		TreeNode::CopyData(otherActor);
		InitializeControls();
	}

	void ComponentsTreeNode::OnDeserialized(const DataValue& node)
	{
		TreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void ComponentsTreeNode::InitializeControls()
	{
		mName = GetLayerDrawable<Text>("name");
		mIcon = GetLayerDrawable<Sprite>("icon");
	}

	ComponentsTree::NodeData::~NodeData()
	{
		Clear();
	}

	void ComponentsTree::NodeData::Clear()
	{
		for (auto child : children)
			delete child;

		children.Clear();
	}

	ComponentsTree::NodeData* ComponentsTree::NodeData::AddChild(const String& name, const Type* type)
	{
		NodeData* newChild = mnew NodeData();
		children.Add(newChild);

		newChild->name = name;
		newChild->path = path.IsEmpty() ? name : path + "/" + name;
		newChild->type = type;
		newChild->parent = this;

		return newChild;
	}
}

DECLARE_CLASS(Editor::AddComponentPanel);

DECLARE_CLASS(Editor::ComponentsTree);

DECLARE_CLASS(Editor::ComponentsTreeNode);
