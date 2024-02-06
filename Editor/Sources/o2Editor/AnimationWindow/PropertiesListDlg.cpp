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

	void PropertiesListDlg::Show(AnimationClip* animation, Ref<Actor> actor)
	{
		Instance().mPropertiesTree->Initialize(animation, actor);
		Instance().mFilter->SetText("");
		Instance().mWindow->ShowModal();
	}

	void PropertiesListDlg::InitializeWindow()
	{
		mWindow = dynamic_cast<o2::Window*>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Animation properties")));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 17));
		mWindow->SetViewLayout(Layout::BothStretch(-2, -2, 0, 20));
		mWindow->SetIcon(mnew Sprite("ui/UI4_tree_wnd_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1)));

// 		mWindow->GetInternalWidget("closeButton")->layout->position -= Vec2F(2, -2);
// 		mWindow->GetInternalWidget("optionsButton")->layout->position -= Vec2F(2, -2);

		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, -1);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		Button* searchButton = o2UI.CreateWidget<Button>("search");
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

	void AnimationPropertiesTree::Initialize(AnimationClip* animation, Ref<Actor> actor)
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

		auto objectType = dynamic_cast<const ObjectType*>(&object->GetType());
		auto rawObject = objectType->DynamicCastFromIObject(object);
		ProcessObject(rawObject, objectType, node);
	}

	void AnimationPropertiesTree::ProcessObject(void* object, const ObjectType* type, NodeData* node)
	{
		for (auto& field : type->GetFields())
		{
			bool isPublic = field.GetProtectionSection() == ProtectSection::Public;
			bool isEditorProperty = field.HasAttribute<EditorPropertyAttribute>();
			bool isIgnoreEditorProperty = field.HasAttribute<IgnoreEditorPropertyAttribute>();
			bool isAnimatable = field.HasAttribute<AnimatableAttribute>();

			if (isAnimatable || (isPublic && !isIgnoreEditorProperty) || isEditorProperty) 
				ProcessTreeNode(field.GetValuePtr(object), field.GetType(), field.GetName(), node);
		}

		for (auto& base : type->GetBaseTypes())
		{
			if (base.type->GetUsage() != Type::Usage::Object)
				continue;

			ProcessObject(base.dynamicCastUpFunc(object), dynamic_cast<const ObjectType*>(base.type), node);
		}
	}

	void AnimationPropertiesTree::ProcessTreeNode(void* object, const Type* type, const String& name, NodeData* node)
	{
		static Vector<const Type*> availableTypes({ &TypeOf(float), &TypeOf(Color4), &TypeOf(Vec2F), &TypeOf(bool) });

		if (type->GetUsage() == Type::Usage::Object)
		{
			auto fieldObjectType = dynamic_cast<const ObjectType*>(type);
			InitializeObjectTreeNode(fieldObjectType, object, name, node);

		}
		else if (type->GetUsage() == Type::Usage::Pointer)
		{
			auto pointerType = dynamic_cast<const PointerType*>(type);
			auto unpointedType = pointerType->GetBaseType();
			if (unpointedType->GetUsage() == Type::Usage::Object)
			{
				auto fieldObjectType = dynamic_cast<const ObjectType*>(unpointedType);
				InitializeObjectTreeNode(fieldObjectType, object, name, node);
			}
		}
		else if (type->GetUsage() == Type::Usage::Property)
		{
			auto propertyType = dynamic_cast<const PropertyType*>(type);
			ProcessTreeNode(propertyType->GetValueAsPtr(object), propertyType->GetValueType(), name, node);
		}
		else if (type->GetUsage() == Type::Usage::StringAccessor)
		{
			auto accessorType = dynamic_cast<const StringPointerAccessorType*>(type);
			auto newNode = node->AddChild(name, type);
			auto allValues = accessorType->GetAllValues(object);

			for (auto& kv : allValues)
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

	void AnimationPropertiesTree::InitializePropertyNode(NodeData* node, const String& name, const Type* type)
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

	void* AnimationPropertiesTree::GetObjectParent(void* object)
	{
		return ((NodeData*)object)->parent;
	}

	Vector<void*> AnimationPropertiesTree::GetObjectChilds(void* object)
	{
		if (object)
			return ((NodeData*)object)->children.Cast<void*>();

		return mRoot.children.Cast<void*>();
	}

	String AnimationPropertiesTree::GetObjectDebug(void* object)
	{
		if (object)
			return ((NodeData*)object)->path;

		return "";
	}

	void AnimationPropertiesTree::FillNodeDataByObject(TreeNode* nodeWidget, void* object)
	{
		auto propertyNode = dynamic_cast<AnimationPropertiesTreeNode*>(nodeWidget);
		propertyNode->Setup((NodeData*)object, this);
	}

	void AnimationPropertiesTree::OnNodeDblClick(const Ref<TreeNode>& nodeWidget)
	{
		if (!nodeWidget)
			return;

		auto propertyNode = dynamic_cast<AnimationPropertiesTreeNode*>(nodeWidget);
		if (propertyNode->mData->used)
			mAnimation->RemoveTrack(propertyNode->mData->path);
		else
			mAnimation->AddTrack(propertyNode->mData->path, *propertyNode->mData->type);
	}

	void AnimationPropertiesTree::OnNodesSelectionChanged(Vector<void*> objects)
	{

	}

	void AnimationPropertiesTree::InitializeObjectTreeNode(const ObjectType* fieldObjectType, void* object, const String& name, NodeData* node)
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
		static Map<const Type*, String> icons = 
		{ 
			{ (const Type*)&TypeOf(float), "ui/UI4_float_type.png" },
			{ (const Type*)&TypeOf(Vec2F), "ui/UI4_vector_type.png" },
			{ (const Type*)&TypeOf(Color4), "ui/UI4_color_type.png" },
			{ (const Type*)&TypeOf(bool), "ui/UI4_bool_type.png" }
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
		mIcon = GetLayerDrawable<Sprite>("icon");

		mAddButton = GetChildByType<Button>("addButton");
		if (mAddButton)
		{
			mAddButton->onClick = [&]()
			{
				mTree->mAnimation->AddTrack(mData->path, *mData->type); 
				mData->used = true; 
				mTree->OnObjectsChanged({ (void*)mData }); 
			};
		}

		mRemoveButton = GetChildByType<Button>("removeButton");
		if (mRemoveButton)
		{
			mRemoveButton->onClick = [&]() { 
				mTree->mAnimation->RemoveTrack(mData->path);
				mData->used = false;
				mTree->OnObjectsChanged({ (void*)mData });
			};
		}
	}

	AnimationPropertiesTree::NodeData::~NodeData()
	{
		Clear();
	}

	void AnimationPropertiesTree::NodeData::Clear()
	{
		for (auto& child : children)
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
// --- META ---

DECLARE_CLASS(Editor::AnimationPropertiesTree, Editor__AnimationPropertiesTree);

DECLARE_CLASS(Editor::AnimationPropertiesTreeNode, Editor__AnimationPropertiesTreeNode);
// --- END META ---
