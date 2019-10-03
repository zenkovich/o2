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
#include "Core/UIRoot.h"

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
		Instance().mPropertiesTree->Initialize(animation, actor);
		Instance().mWindow->ShowModal();
	}

	void PropertiesListDlg::InitializeWindow()
	{
		mWindow = dynamic_cast<Window*>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Animation properties")));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 17));

		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, -5, -5, 20, -1);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		Button* searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		//searchButton->onClick += THIS_FUNC(OnSearchPressed);
		upPanel->AddChild(searchButton);

		mFilter = o2UI.CreateWidget<EditBox>("backless");
		*mFilter->layout = WidgetLayout::BothStretch(19, 2, 0, -4);
		//mFilter->onChanged += THIS_FUNC(OnSearchEdited);
		upPanel->AddChild(mFilter);

		mWindow->AddChild(upPanel);

		mPropertiesTree = o2UI.CreateWidget<AnimationPropertiesTree>();
		*mPropertiesTree->layout = WidgetLayout::BothStretch(-7, -7, -5, 17);
		mPropertiesTree->SetRearrangeType(Tree::RearrangeType::Disabled);

		mWindow->AddChild(mPropertiesTree);

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(200, 300);

		mWindow->GetBackCursorListener().onCursorReleased = [=](const Input::Cursor& c) { mWindow->Hide(); };
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
		mRoot.Clear();
		mPassedObject.Clear();

		InitializeTreeNode(&mRoot, actor.Get());
		UpdateNodesStructure();
		UpdateVisibleNodes();
	}

	void AnimationPropertiesTree::InitializeTreeNode(NodeData* node, IObject* object)
	{
		static Vector<const Type*> availableTypes({ &TypeOf(float), &TypeOf(Color4), &TypeOf(Vec2F), &TypeOf(bool), &TypeOf(RectF) });

		if (!object)
			return;

		if (mPassedObject.Contains(object))
			return;

		mPassedObject.Add(object);

		auto objectType = dynamic_cast<const ObjectType*>(&object->GetType());
		auto rawObject = objectType->DynamicCastFromIObject(object);
		for (auto field : objectType->GetFields())
		{
			if (field->GetProtectionSection() != ProtectSection::Public)
				continue;

			if (field->GetType()->GetUsage() == Type::Usage::Object)
			{
				auto fieldObjectType = dynamic_cast<const ObjectType*>(field->GetType());
				InitializeObjectTreeNode(fieldObjectType, field, rawObject, node);

			}
			else if (field->GetType()->GetUsage() == Type::Usage::Pointer)
			{
				auto pointerType = dynamic_cast<const PointerType*>(field->GetType());
				auto unpointedType = pointerType->GetUnpointedType();
				if (unpointedType->GetUsage() == Type::Usage::Object)
				{
					auto fieldObjectType = dynamic_cast<const ObjectType*>(unpointedType);
					InitializeObjectTreeNode(fieldObjectType, field, rawObject, node);
				}
			}
			else if (field->GetType()->GetUsage() == Type::Usage::Property)
			{
				auto propertyType = dynamic_cast<const PropertyType*>(field->GetType()); 
				if (availableTypes.Contains(propertyType->GetValueType()))
					InitializePropertyNode(node, field, propertyType->GetValueType());
			}
			else if (field->GetType()->GetUsage() == Type::Usage::StringAccessor)
			{
			}
			else if (availableTypes.Contains(field->GetType()))
			{
				InitializePropertyNode(node, field, field->GetType());
			}
		}
	}

	void AnimationPropertiesTree::InitializePropertyNode(NodeData* node, FieldInfo* field, const Type* type)
	{
		auto newNode = node->AddChild(field->GetName(), type);
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

		return mRoot.children.Cast<UnknownPtr>();
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

	void AnimationPropertiesTree::InitializeObjectTreeNode(const ObjectType* fieldObjectType, FieldInfo* field, void* rawObject, NodeData* node)
	{
		auto fieldObject = fieldObjectType->DynamicCastToIObject(field->GetValuePtr(rawObject));
		auto newNode = node->AddChild(field->GetName(), fieldObjectType);
		InitializeTreeNode(newNode, fieldObject);

		if (newNode->children.IsEmpty())
		{
			node->children.Remove(newNode);
			delete newNode;
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
		static Dictionary<const Type*, String> icons = 
		{ 
			{ &TypeOf(float), "ui/UI4_float_type.png" },
			{ &TypeOf(Vec2F), "ui/UI4_vector_type.png" },
			{ &TypeOf(Color4), "ui/UI4_color_type.png" },
			{ &TypeOf(bool), "ui/UI4_bool_type.png" }
		};

		static String otherIcon = "ui/UI4_other_type.png";

		mName->text = data.name;

		String iconPath;
		if (!icons.TryGetValue(data.type, iconPath))
			iconPath = otherIcon;

		*mIcon = Sprite(iconPath);

		mAddButton->enabled = !data.used && data.children.IsEmpty();
		mRemoveButton->enabled = data.used && data.children.IsEmpty();
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
		mIcon = GetLayerDrawable<Sprite>("icon");
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

DECLARE_CLASS(Editor::AnimationPropertiesTree);

DECLARE_CLASS(Editor::AnimationPropertiesTreeNode);
