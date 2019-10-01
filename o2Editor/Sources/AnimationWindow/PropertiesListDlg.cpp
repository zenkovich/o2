#include "stdafx.h"
#include "PropertiesListDlg.h"

#include "Core/EditorScope.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/Window.h"

DECLARE_SINGLETON(Editor::PropertiesListDlg);

namespace Editor
{

	PropertiesListDlg::PropertiesListDlg()
	{
		InitializeWindow();
	}

	PropertiesListDlg::~PropertiesListDlg()
	{
		delete mWindow;
	}

	void PropertiesListDlg::Show(Animation* animation, ActorRef actor)
	{
		mPropertiesTree->Initialize(animation, actor);
		mWindow->ShowModal();
	}

	void PropertiesListDlg::InitializeWindow()
	{
		mWindow = o2UI.CreateWindow("Animation properties");

		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		Button* searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		//searchButton->onClick += THIS_FUNC(OnSearchPressed);
		upPanel->AddChild(searchButton);

		mFilter = o2UI.CreateWidget<EditBox>("backless");
		*mFilter->layout = WidgetLayout::BothStretch(19, 2, 0, -2);
		//mFilter->onChanged += THIS_FUNC(OnSearchEdited);
		upPanel->AddChild(mFilter);

		mWindow->AddChild(upPanel);

		mPropertiesTree = o2UI.CreateWidget<AnimationPropertiesTree>();
		*mPropertiesTree->layout = WidgetLayout::BothStretch(0, 0, 0, 20);

		mWindow->AddChild(mPropertiesTree);
	}

	AnimationPropertiesTree::AnimationPropertiesTree():
		Tree()
	{}

	AnimationPropertiesTree::AnimationPropertiesTree(const AnimationPropertiesTree& other):
		Tree(other)
	{ }

	AnimationPropertiesTree& AnimationPropertiesTree::operator=(const AnimationPropertiesTree& other)
	{
		Tree::operator=(other);
		return *this;
	}

	void AnimationPropertiesTree::Initialize(Animation* animation, ActorRef actor)
	{
		mRoot.children.Clear();

		InitializeTreeNode(&mRoot, actor.Get());
	}

	void AnimationPropertiesTree::InitializeTreeNode(NodeData* node, IObject* object)
	{
		static Vector<const Type*> availableTypes({ &TypeOf(float), &TypeOf(Color4), &TypeOf(Vec2F), &TypeOf(bool), &TypeOf(RectF) });

		for (auto field : object->GetType().GetFields()) 
		{
			if (field->GetType()->GetUsage() == Type::Usage::Object)
			{
				InitializeSubTreeNode(field, dynamic_cast<const ObjectType*>(field->GetType()), object, node);
			}
			else if (field->GetType()->GetUsage() == Type::Usage::Pointer)
			{
				auto pointerType = dynamic_cast<const PointerType*>(field->GetType());
				if (pointerType->GetUnpointedType()->GetUsage() == Type::Usage::Object)
					InitializeSubTreeNode(field, dynamic_cast<const ObjectType*>(field->GetType()), object, node);
			}
			else if (availableTypes.Contains(field->GetType()))
			{
				auto newNode = node->AddChild(field->GetName(), field->GetType());
			}
		}
	}

	void AnimationPropertiesTree::UpdateVisibleNodes()
	{
		PushScopeEnterOnStack scope;
		Tree::UpdateVisibleNodes();
	}

	TreeNode* AnimationPropertiesTree::CreateTreeNodeWidget()
	{
		PushScopeEnterOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	UnknownPtr AnimationPropertiesTree::GetObjectParent(UnknownPtr object)
	{
		return ((NodeData*)object)->parent;
	}

	Vector<UnknownPtr> AnimationPropertiesTree::GetObjectChilds(UnknownPtr object)
	{
		if (object)
			return ((NodeData*)object)->children.Cast<UnknownPtr>();

		return { &mRoot };
	}

	String AnimationPropertiesTree::GetObjectDebug(UnknownPtr object)
	{
		if (object)
			return ((NodeData*)object)->path;

		return "";
	}

	void AnimationPropertiesTree::FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object)
	{
		auto propertyNode = dynamic_cast<AnimationPropertiesTreeNode*>(nodeWidget);
		propertyNode->Setup(*((NodeData*)object));
	}

	void AnimationPropertiesTree::OnNodeDblClick(TreeNode* nodeWidget)
	{

	}

	void AnimationPropertiesTree::OnNodesSelectionChanged(UnknownPtrsVec objects)
	{

	}

	void AnimationPropertiesTree::InitializeSubTreeNode(FieldInfo* fieldInfo, const ObjectType* type, IObject* object, NodeData* node)
	{
		if (auto fieldAsObject = type->DynamicCastToIObject(fieldInfo->GetValuePtr(object)))
		{
			auto newNode = node->AddChild(fieldInfo->GetName(), fieldInfo->GetType());
			InitializeTreeNode(newNode, fieldAsObject);
		}
	}

	AnimationPropertiesTreeNode::AnimationPropertiesTreeNode():
		TreeNode()
	{
		InitializeControls();
	}

	AnimationPropertiesTreeNode::AnimationPropertiesTreeNode(const AnimationPropertiesTreeNode& other) :
		TreeNode(other)
	{
		InitializeControls();
	}

	AnimationPropertiesTreeNode& AnimationPropertiesTreeNode::operator=(const AnimationPropertiesTreeNode& other)
	{
		TreeNode::operator=(other);
		InitializeControls();
		return *this;
	}

	void AnimationPropertiesTreeNode::Setup(const AnimationPropertiesTree::NodeData& data)
	{
		mName->text = data.name;
		//mIcon
		mAddButton->enabled = !data.used;
		mRemoveButton->enabled = data.used;
	}

	void AnimationPropertiesTreeNode::CopyData(const Actor& otherActor)
	{
		TreeNode::CopyData(otherActor);
		InitializeControls();
	}

	void AnimationPropertiesTreeNode::OnDeserialized(const DataNode& node)
	{
		InitializeControls();
	}

	void AnimationPropertiesTreeNode::InitializeControls()
	{
		mName = GetLayerDrawable<Text>("name");
		mIcon = GetLayerDrawable<Sprite>("name");
		mAddButton = GetChildByType<Button>("addButton");
		mRemoveButton = GetChildByType<Button>("removeButton");
	}

	AnimationPropertiesTree::NodeData::~NodeData()
	{
		Clear();
	}

	void AnimationPropertiesTree::NodeData::Clear()
	{
		for (auto child : children)
			delete child;

		children.Clear();
	}

	AnimationPropertiesTree::NodeData* AnimationPropertiesTree::NodeData::AddChild(const String& name, const Type* type)
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
