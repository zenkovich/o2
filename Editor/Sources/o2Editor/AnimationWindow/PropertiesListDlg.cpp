#include "o2Editor/stdafx.h"
#include "PropertiesListDlg.h"

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

	void PropertiesListDlg::Show(AnimationClip* animation, const Ref<const Actor>& actor)
	{
		Instance().mPropertiesTree->Initialize(animation, actor);
		Instance().mFilter->SetText("");
		Instance().mWindow->ShowModal();
	}

	void PropertiesListDlg::InitializeWindow()
	{
		mWindow = DynamicCast<o2::Window>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Animation properties")));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 17));
		mWindow->SetViewLayout(Layout::BothStretch(-2, -2, 0, 20));
		mWindow->SetIcon(MakeRef<Sprite>("ui/UI4_tree_wnd_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1)));

// 		mWindow->GetInternalWidget("closeButton")->layout->position -= Vec2F(2, -2);
// 		mWindow->GetInternalWidget("optionsButton")->layout->position -= Vec2F(2, -2);

		auto upPanel = MakeRef<Widget>();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, -1);
		upPanel->AddLayer("back", MakeRef<Sprite>("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		auto searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		upPanel->AddChild(searchButton);

		mFilter = o2UI.CreateWidget<EditBox>("backless");
		*mFilter->layout = WidgetLayout::BothStretch(19, 2, 0, -4);
		mFilter->onChanged += [&](const WString& filter) { mPropertiesTree->SetFilter(filter); };
		upPanel->AddChild(mFilter);

		mWindow->AddChild(upPanel);

		mPropertiesTree = o2UI.CreateWidget<AnimationPropertiesTree>();
		*mPropertiesTree->layout = WidgetLayout::BothStretch(0, 0, 0, 17);
		mPropertiesTree->SetRearrangeType(Tree::RearrangeType::Disabled);

		mWindow->AddChild(mPropertiesTree);

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(400, 600);

		mWindow->GetBackCursorListener().onCursorReleased = [=](const Input::Cursor& c) { mWindow->Hide(); };
	}

	AnimationPropertiesTree::AnimationPropertiesTree() :
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

	void AnimationPropertiesTree::Initialize(AnimationClip* animation, const Ref<const Actor>& actor)
	{
		mFilterStr = "";
		mRoot.Clear();
		mPassedObject.Clear();

		mAnimation = animation;
		mActor = actor;

		InitializeTreeNode(&mRoot, actor.Get());
		UpdateNodesStructure();
		UpdateVisibleNodes();
	}

	void AnimationPropertiesTree::SetFilter(const WString& filter)
	{
		mFilterStr = filter.ToLowerCase();

		mRoot.Clear();
		mPassedObject.Clear();

		InitializeTreeNode(&mRoot, mActor.Get());
		UpdateNodesStructure();
		UpdateVisibleNodes();
	}

	String AnimationPropertiesTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void AnimationPropertiesTree::InitializeTreeNode(NodeData* node, IObject* object)
	{
		if (!object)
			return;

		if (mPassedObject.Contains(object))
			return;

		mPassedObject.Add(object);

		auto objectType = DynamicCast<const ObjectType>(&object->GetType());
		auto rawObject = objectType->DynamicCastFromIObject(object);
		ProcessObject(rawObject, objectType, node);
	}

	void AnimationPropertiesTree::ProcessObject(const Ref<void>& object, const ObjectType* type, NodeData* node)
	{
		for (auto& field : type->GetFields())
		{
			bool isPublic = field.GetProtectionSection() == ProtectSection::Public;
			bool isEditorProperty = field.HasAttribute<EditorPropertyAttribute>();
			bool isIgnoreEditorProperty = field.HasAttribute<IgnoreEditorPropertyAttribute>();
			bool isAnimatable = field.HasAttribute<AnimatableAttribute>();

			if (isAnimatable || (isPublic && !isIgnoreEditorProperty) || isEditorProperty) 
				ProcessTreeNode(field.GetValuePtr(object.Get()), field.GetType(), field.GetName(), node);
		}

		for (auto base : type->GetBaseTypes())
		{
			if (base.type->GetUsage() != Type::Usage::Object)
				continue;

			ProcessObject(base.dynamicCastUpFunc(object.Get()), DynamicCast<const ObjectType>(base.type), node);
		}
	}

	void AnimationPropertiesTree::ProcessTreeNode(const Ref<void>& object, const Type* type, const String& name, NodeData* node)
	{
		static Vector<const Type*> availableTypes({ &TypeOf(float), &TypeOf(Color4), &TypeOf(Vec2F), &TypeOf(bool) });

		if (availableTypes.Contains(type) || type->IsEnum() || type->IsClass())
		{
			auto fieldNode = NodeData::CreateObject(node);
			fieldNode->name = name;

			if (type->IsEnum())
			{
				auto enumType = DynamicCast<const EnumType>(type);
				auto comboBox = MakeRef<ComboBox>();

				int selectedValueIndex = 0;

				for (int i = 0; i < enumType->GetCount(); i++)
				{
					auto enumValue = enumType->GetValue(i);

					comboBox->AddItem(enumValue->GetName());
					if (enumValue->GetValue() == *(int*)object.Get())
						selectedValueIndex = i;
				}

				fieldNode->widget = comboBox;
				fieldNode->widget->SetCurSelected(selectedValueIndex);
			}
			else if (type->IsClass())
			{
				auto classType = DynamicCast<const ClassType>(type);
				auto dynamicObject = DynamicCast<IObject>(*static_cast<void**>(object.Get())); // Edited line

				if (dynamicObject)
				{
					if (classType->HasAttribute<AnimatableAttribute>())
					{
						auto animatableAttribute = classType->GetAttribute<AnimatableAttribute>();
						if (animatableAttribute->CanCreate())
						{
							auto createMenuItem = MakeRef<MenuItem>();
							createMenuItem->SetText("Create " + classType->GetName());

							auto contextMenu = MakeRef<ContextMenu>();
							contextMenu->AddItem(createMenuItem);

							auto button = o2UI.CreateWidget<Button>("animatable object");
							*button->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(1, 1));
							button->SetContextMenu(contextMenu);

							fieldNode->widget = button;

							createMenuItem->onItemClicked += [=]() {
								Ref<void> createdObject = mmake<>(*classType);
								auto widget = CreateEditorForNode(createdObject, fieldNode.get(), classType);
								fieldNode->widget = widget;
								UpdateVisibleNodes();
							};
						}
						else
						{
							fieldNode->widget = CreateEditorForNode(dynamicObject, fieldNode.get(), classType);
						}
					}
					else
					{
						fieldNode->widget = CreateEditorForNode(dynamicObject, fieldNode.get(), classType);
					}
				}
			}
			else
			{
				auto widget = CreateEditorForNode(object, fieldNode.get(), type);
				fieldNode->widget = widget;
			}
		}
	}

	Ref<Widget> AnimationPropertiesTree::CreateEditorForNode(const Ref<void>& object, NodeData* node, const Type* type)
	{
		if (type == &TypeOf(float))
		{
			auto editBox = o2UI.CreateWidget<EditBoxFloat>(node->name);
			editBox->SetFloatValue(*(float*)object.Get());
			editBox->onValueChanged += [=](float value) { *(float*)object.Get() = value; };
			editBox->SetTextFormat(L"%.3f");
			*editBox->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(100, 20), Vec2F(1, 1));
			return editBox;
		}
		else if (type == &TypeOf(Color4))
		{
			auto editBox = o2UI.CreateWidget<EditBoxColor>(node->name);
			editBox->SetColorValue(*(Color4*)object.Get());
			editBox->onValueChanged += [=](Color4 value) { *(Color4*)object.Get() = value; };
			*editBox->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(100, 20), Vec2F(1, 1));
			return editBox;
		}
		else if (type == &TypeOf(Vec2F))
		{
			auto editBox = o2UI.CreateWidget<EditBoxVec2F>(node->name);
			editBox->SetVec2FValue(*(Vec2F*)object.Get());
			editBox->onValueChanged += [=](Vec2F value) { *(Vec2F*)object.Get() = value; };
			*editBox->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(100, 20), Vec2F(1, 1));
			return editBox;
		}
		else if (type == &TypeOf(bool))
		{
			auto checkBox = o2UI.CreateWidget<CheckBox>(node->name);
			checkBox->SetChecked(*(bool*)object.Get());
			checkBox->onValueChanged += [=](bool value) { *(bool*)object.Get() = value; };
			*checkBox->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(1, 1));
			return checkBox;
		}

		return nullptr;
	}

	void AnimationPropertiesTree::UpdateVisibleNodes()
	{
		RebuildTreeNodes();

		mRoot.ClearVisibleNodes();
		auto filterStr = mFilterStr.ToLower();

		for (auto& node : mRoot.GetAllNodes())
		{
			if (node.name.Find(filterStr) != String::npos || HasVisibleChild(node))
				node.visible = true;
			else
				node.visible = false;   
		}
	}

	bool AnimationPropertiesTree::HasVisibleChild(const NodeData& node)
	{
		for (auto& child : node.children)
		{
			if (child.visible || HasVisibleChild(child))
				return true;
		}

		return false;
	}

	void AnimationPropertiesTree::UpdateNodesStructure()
	{
		BuildTree();

		for (auto& node : mRoot.GetAllNodes())
		{
			auto#include <Ref.h>
#include <WeakRef.h>

...

void ProcessTreeNode(const Ref<Object>& object, const Ref<Type>& type, const String& name, TreeNode* node)
{
	if (type->GetUsage() == Type::Usage::Object)
	{
		auto fieldObjectType = DynamicCast<ObjectType>(type);
		InitializeObjectTreeNode(fieldObjectType, object, name, node);

	}
	else if (type->GetUsage() == Type::Usage::Pointer)
	{
		auto pointerType = DynamicCast<PointerType>(type);
		auto unpointedType = pointerType->GetBaseType();
		if (unpointedType->GetUsage() == Type::Usage::Object)
		{
			auto fieldObjectType = DynamicCast<ObjectType>(unpointedType);
			InitializeObjectTreeNode(fieldObjectType, object, name, node);
		}
	}
	else if (type->GetUsage() == Type::Usage::Property)
	{
		auto propertyType = DynamicCast<PropertyType>(type);
		ProcessTreeNode(propertyType->GetValueAsPtr(object), propertyType->GetValueType(), name, node);
	}
	else if (type->GetUsage() == Type::Usage::StringAccessor)
	{
		auto accessorType = DynamicCast<StringPointerAccessorType>(type);
		auto newNode = node->AddChild(name, type);
		auto allValues = accessorType->GetAllValues(object);

		for (const auto& kv : allValues)
			ProcessTreeNode(kv.second, accessorType->GetReturnType(), kv.first, newNode);

		if (newNode->children.empty()) {
			node->children.Remove(newNode);
			delete newNode;
		}
	}
	else if (availableTypes.Contains(type))
	{
		InitializePropertyNode(node, name, type);
	}
}

void AnimationPropertiesTree::InitializePropertyNode(NodeData* node, const String& name, const Ref<Type>& type)
{
	if (!mFilterStr.IsEmpty() && !name.ToLowerCase().Contains(mFilterStr))
		return;

	auto newNode = node->AddChild(name, type);
	newNode->used = mAnimation->ContainsTrack(newNode->path);
}

void AnimationPropertiesTree::UpdateVisibleNodes()
{
	PushEditorScopeOnStack scope;
	Tree::UpdateVisibleNodes();
}

TreeNode* AnimationPropertiesTree::CreateTreeNodeWidget()
{
	PushEditorScopeOnStack scope;
	return Tree::CreateTreeNodeWidget();
}

Ref<Object> AnimationPropertiesTree::GetObjectParent(const Ref<Object>& object)
{
	return WeakRef<Object>(((NodeData*)object.Get())->parent);
}

Vector<Ref<Object>> AnimationPropertiesTree::GetObjectChilds(const Ref<Object>& object)
{
	if (object)
		return Vector<Ref<Object>>(((NodeData*)object.Get())->children.Cast<Ref<Object>>());

	return Vector<Ref<Object>>(mRoot.children.Cast<Ref<Object>>());
}

String AnimationPropertiesTree::GetObjectDebug(const Ref<Object>& object)
{
	if (object)
		return ((NodeData*)object.Get())->path;

	return "";
}

void AnimationPropertiesTree::FillNodeDataByObject(TreeNode* nodeWidget, const Ref<Object>& object)
{
	auto propertyNode = DynamicCast<AnimationPropertiesTreeNode>(nodeWidget);
	propertyNode->Setup((NodeData*)object.Get(), this);
}

void AnimationPropertiesTree::OnNodeDblClick(TreeNode* nodeWidget)
{
	if (!nodeWidget)
		return;

	auto propertyNode = DynamicCast<AnimationPropertiesTreeNode>(nodeWidget);
	if (propertyNode->mData->used)
		mAnimation->RemoveTrack(propertyNode->mData->path);
	else
		mAnimation->AddTrack(propertyNode->mData->path, *propertyNode->mData->type);
}

void AnimationPropertiesTree::OnNodesSelectionChanged(const Vector<Ref<Object>>& objects)
{

}

void AnimationPropertiesTree::InitializeObjectTreeNode(const Ref<ObjectType>& fieldObjectType, const Ref<Object>& object, const String& name, NodeData* node)
{
	auto fieldObject = fieldObjectType->DynamicCastToIObject(object);
	auto newNode = node->AddChild(name, fieldObjectType);
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

void AnimationPropertiesTreeNode::Setup(AnimationPropertiesTree::NodeData* data, AnimationPropertiesTree* tree)
{
	static Map<const Ref<Type>, String> icons =
	{
		{ TypeOf<float>(), "ui/UI4_float_type.png" },
		{ TypeOf<Vec2F>(), "ui/UI4_vector_type.png" },
		{ TypeOf<Color4>(), "ui/UI4_color_type.png" },
		{ TypeOf<bool>(), "ui/UI4_bool_type.png" }
	};

	static String otherIcon = "ui/UI4_other_type.png";

	mName->text = data->name;

	String iconPath;
	if (!icons.TryGetValue(data->type, iconPath))
		iconPath = otherIcon;

	*mIcon = Sprite(iconPath);

	mData = data;
	mTree = tree;

	mAddButton->SetEnabledForcible(!data->used && data->children.IsEmpty());
	mRemoveButton->SetEnabledForcible(data->used && data->children.IsEmpty());
}

String AnimationPropertiesTreeNode::GetCreateMenuCategory()
{
	return "UI/Editor";
}

void AnimationPropertiesTreeNode::OnDeserialized(const DataValue& node)
{
	TreeNode::OnDeserialized(node);
	InitializeControls();
}

void AnimationPropertiesTreeNode::InitializeControls()
{
	mName = GetLayerDrawable<Text>("name");
}#include <Ref.hpp>

using namespace Ref;

// Replace raw pointers with Ref<>
Ref<AnimationPropertiesTree> mTree;

class AnimationPropertiesTree {
public:
	struct NodeData {
		String name;
		String path;
		const Type* type;
		NodeData* parent;
		Array<Ref<NodeData>> children; // Use Ref<> for array elements

		// Destructor
		~NodeData() {
			Clear(); // Call Clear() to properly deallocate children
		}

		void Clear() {
			for (const Ref<NodeData>& child : children)
				child.Release(); // Release the reference instead of deleting

			children.Clear();
		}

		Ref<NodeData> AddChild(const String& name, const Type* type) {
			Ref<NodeData> newChild = mmake<NodeData>(); // Use mmake<>() to create Ref<>
			children.Add(newChild);

			newChild->name = name;
			newChild->path = path.IsEmpty() ? name : path + "/" + name;
			newChild->type = type;
			newChild->parent = this;

			return newChild;
		}
	};

	// ...

	Ref<Sprite> mIcon;
	Ref<Button> mAddButton;
	Ref<Button> mRemoveButton;

	// ...
};

// Replace dynamic_cast with DynamicCast
template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& object) {
	return Ref::DynamicCast<T>(object);
}

// Usage
Ref<Sprite> icon = GetLayerDrawable<Sprite>("icon");

mAddButton = GetChildByType<Button>("addButton");
if (mAddButton) {
	mAddButton->onClick = [&]() {
		mTree->mAnimation->AddTrack(mData->path, *mData->type);
		mData->used = true;
		mTree->OnObjectsChanged({ (const void*)mData }); // Use const Ref<>& for function arguments with pointer types
	};
}

mRemoveButton = GetChildByType<Button>("removeButton");
if (mRemoveButton) {
	mRemoveButton->onClick = [&]() {
		mTree->mAnimation->RemoveTrack(mData->path);
		mData->used = false;
		mTree->OnObjectsChanged({ (const void*)mData }); // Use const Ref<>& for function arguments with pointer types
	};
}

// ---
// Meta class declarations (assuming they contain raw pointers, not Ref<>)
DECLARE_CLASS(Editor::AnimationPropertiesTree, Editor__AnimationPropertiesTree);

DECLARE_CLASS(Editor::AnimationPropertiesTreeNode, Editor__AnimationPropertiesTreeNode);
// --- END META ---