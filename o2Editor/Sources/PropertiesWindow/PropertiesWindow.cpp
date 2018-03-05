#include "stdafx.h"
#include "PropertiesWindow.h"

#include "Properties/EnumProperty.h"
#include "Properties/ObjectPtrProperty.h"
#include "PropertiesWindow/IObjectPropertiesViewer.h"
#include "PropertiesWindow/Properties/AssetProperty.h"
#include "PropertiesWindow/Properties/BooleanProperty.h"
#include "PropertiesWindow/Properties/FieldPropertiesInfo.h"
#include "PropertiesWindow/Properties/FloatProperty.h"
#include "PropertiesWindow/Properties/IntegerProperty.h"
#include "PropertiesWindow/Properties/ObjectProperty.h"
#include "PropertiesWindow/Properties/StringProperty.h"
#include "PropertiesWindow/Properties/VectorProperty.h"
#include "PropertiesWindow/Properties/WStringProperty.h"
#include "UI/HorizontalLayout.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"
#include "Utils/EditorPropertyAttribute.h"
#include "Utils/Timer.h"

DECLARE_SINGLETON(Editor::PropertiesWindow);

namespace Editor
{
	PropertiesWindow::PropertiesWindow():
		mCurrentViewer(nullptr)
	{
		InitializeWindow();
		InitializePropertiesFields();
		InitializeViewers();
	}

	PropertiesWindow::~PropertiesWindow()
	{
		for (auto viewer : mViewers)
			delete viewer;

		for (auto field : mAvailablePropertiesFields)
			delete field;
	}

	void PropertiesWindow::ResetTargets()
	{
		SetTarget(nullptr);
	}

	void PropertiesWindow::InitializeWindow()
	{
		mWindow->caption = "Properties";
		mWindow->name = "properties window";
		mWindow->SetIcon(mnew Sprite("ui/UI2_gear_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 15));

		InitializeWindowContext();
	}

	void PropertiesWindow::InitializeWindowContext()
	{
		auto context = mWindow->GetOptionsMenu();
		if (!context)
		{
			o2Debug.LogError("Failed to initialize properties window context menu: not found menu on window");
			return;
		}

		context->AddItem(UIContextMenu::Item::Separator());
		context->AddItem(UIContextMenu::Item("Private visible", false, THIS_FUNC(SetPrivateFieldsVisible)));
	}

	void PropertiesWindow::InitializeViewers()
	{
		auto viewersTypes = TypeOf(IObjectPropertiesViewer).GetDerivedTypes();

		for (auto type : viewersTypes)
			mViewers.Add((IObjectPropertiesViewer*)type->CreateSample());
	}

	void PropertiesWindow::InitializePropertiesFields()
	{
		auto a = TypeOf(IAssetProperty).GetDerivedTypes();
		auto b = TypeOf(IPropertyField).GetDerivedTypes();
		auto avaialbleTypes = a + b;

		avaialbleTypes.Remove(&TypeOf(IAssetProperty));
		avaialbleTypes.Remove(&TypeOf(ObjectProperty));

		for (auto x : avaialbleTypes)
		{
			auto sample = (IPropertyField*)x->CreateSample();
			mAvailablePropertiesFields.Add(sample);
		}
	}

	void PropertiesWindow::BuildFields(UIVerticalLayout* layout, Vector<FieldInfo*> fields, 
									   FieldPropertiesInfo& propertiesInfo, const String& path)
	{
		Timer t;
		Timer t2;
		for (auto fieldInfo : fields)
		{
			const Type* fieldType = fieldInfo->GetType();

			String propertyName;
			if (fieldInfo->GetProtectionSection() == ProtectSection::Public)
				propertyName = MakeSmartFieldName(fieldInfo->GetName());
			else
				propertyName = "[" + MakeSmartFieldName(fieldInfo->GetName()) + "]";

			auto fieldWidgetPair = CreateFieldProperty(fieldInfo->GetType(), propertyName);
			if (!fieldWidgetPair.first)
				continue;

			fieldWidgetPair.first->SetValuePath(path + fieldInfo->GetName());
			fieldWidgetPair.first->SpecializeType(fieldType);

			layout->AddChild(fieldWidgetPair.second, false);
			propertiesInfo.properties.Add(fieldInfo, fieldWidgetPair.first);

			o2Debug.Log("Field " + path + "/" + fieldInfo->GetName() + " for " + (String)t.GetDeltaTime());
		}

		o2Debug.Log(">>> Fields created for " + (String)t2.GetDeltaTime());
	}

	void PropertiesWindow::OnPropertyChanged(IPropertyField* field)
	{
		onFieldChanged(field);
		mTargetsChanged = true;
	}

	void PropertiesWindow::SetTarget(IObject* target)
	{
		if (target == nullptr)
			SetTargets(Vector<IObject*>());
		else
			SetTargets({ target });
	}

	void PropertiesWindow::SetTargets(const Vector<IObject*> targets, const Function<void()>& targetsChangedDelegate /*= Function<void()>()*/)
	{
		onTargetsChanged();

		if (mTargetsChanged)
			mOnTargetsChangedDelegate();

		IObjectPropertiesViewer* objectViewer = nullptr;
		if (!targets.IsEmpty())
		{
			auto type = &targets[0]->GetType();
			objectViewer = mViewers.FindMatch([&](auto x) { return type->IsBasedOn(*x->GetViewingObjectType()); });
		}

		if (objectViewer != mCurrentViewer)
		{
			if (mCurrentViewer)
			{
				mCurrentViewer->mContentWidget->Hide(true);
				mCurrentViewer->OnDisabled();
			}

			mCurrentViewer = objectViewer;

			if (mCurrentViewer)
			{
				mCurrentViewer->mContentWidget->SetParent(mWindow);
				*mCurrentViewer->mContentWidget->layout = UIWidgetLayout::BothStretch();
				mCurrentViewer->mContentWidget->Show(true);
				mCurrentViewer->OnEnabled();
			}
		}

		mTargets = targets;

		if (mCurrentViewer)
			mCurrentViewer->SetTargets(mTargets);

		mOnTargetsChangedDelegate = targetsChangedDelegate;
		mTargetsChanged = false;
	}

	Vector<IObject*> PropertiesWindow::GetTargets() const
	{
		return mTargets;
	}

	void PropertiesWindow::Update(float dt)
	{
		if (mCurrentViewer)
			mCurrentViewer->Update(dt);
	}

	void PropertiesWindow::Draw()
	{
		if (mCurrentViewer)
			mCurrentViewer->Draw();
	}

	bool PropertiesWindow::IsTargetsChanged() const
	{
		return mTargetsChanged;
	}

	void PropertiesWindow::SetPrivateFieldsVisible(bool visible)
	{
		mPrivateVisible = true;
		SetTargets(mTargets, mOnTargetsChangedDelegate);
	}

	bool PropertiesWindow::IsPrivateFieldsVisible() const
	{
		return mPrivateVisible;
	}

	bool PropertiesWindow::IsFieldTypeSupported(const Type* type) const
	{
		if (type->GetUsage() == Type::Usage::Vector)
			return IsFieldTypeSupported(dynamic_cast<const VectorType*>(type)->GetElementType());

		IPropertyField* fieldSample = GetFieldPropertyPrototype(type);
		if (fieldSample)
			return true;

		if (type->IsBasedOn(TypeOf(IObject)))
			return true;

		if (type->GetUsage() == Type::Usage::Pointer && ((PointerType*)type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			return true;

		if (type->GetUsage() == Type::Usage::Enumeration)
			return true;

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = ((const PropertyType*)type)->GetValueType();

			if (valueType->GetUsage() == Type::Usage::Enumeration)
				return true;

			fieldSample = GetFieldPropertyPrototype(valueType);
			if (fieldSample)
				return true;
		}

		return false;
	}	
	
	bool PropertiesWindow::IsPropertyVisible(FieldInfo* info, bool allowPrivate) const
	{
		if (info->HasAttribute<IgnoreEditorPropertyAttribute>())
			return false;

		if (info->GetProtectionSection() == ProtectSection::Public || allowPrivate)
			return true;

		if (info->HasAttribute<EditorPropertyAttribute>())
			return true;

		return false;
	}

	bool PropertiesWindow::IsPropertyVisible(FieldInfo* info) const
	{
		return IsPropertyVisible(info, mPrivateVisible);
	}

	void PropertiesWindow::BuildObjectProperties(UIVerticalLayout* layout, const Type* type,
												 FieldPropertiesInfo& propertiesInfo, const String& path)
	{
		BuildObjectProperties(layout, type->GetFieldsWithBaseClasses(), propertiesInfo, path);
	}

	void PropertiesWindow::BuildObjectProperties(UIVerticalLayout* layout, Vector<FieldInfo*> fields,
												 FieldPropertiesInfo& propertiesInfo, const String& path)
	{
		Vector<FieldInfo*> regularFields = fields.FindAll(
			[&](FieldInfo* x) { return IsPropertyVisible(x, false); });

		BuildFields(layout, regularFields, propertiesInfo, path);

		if (mPrivateVisible)
		{
			Vector<FieldInfo*> privateFields = fields.FindAll(
				[&](FieldInfo* x) { return IsPropertyVisible(x, true) && !regularFields.Contains(x); });

			if (!privateFields.IsEmpty())
			{
				UISpoiler* privates = o2UI.CreateWidget<UISpoiler>("expand with caption");
				privates->SetCaption("Private");

				BuildFields(privates, privateFields, propertiesInfo, path);

				layout->AddChild(privates);
			}
		}
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateFieldProperty(const Type* type, const String& name)
	{
		if (type->GetUsage() == Type::Usage::Vector)
			return CreateVectorField(type, name);

		IPropertyField* fieldSample = GetFieldPropertyPrototype(type);
		if (fieldSample)
			return CreateRegularField(&fieldSample->GetType(), name);

		if (type->IsBasedOn(TypeOf(IObject)))
			return CreateObjectField(type, name);

		if (type->GetUsage() == Type::Usage::Pointer && ((PointerType*)type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			return CreateObjectPtrField(type, name);

		if (type->GetUsage() == Type::Usage::Enumeration)
			return CreateRegularField(&TypeOf(EnumProperty), name);

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = ((const PropertyType*)type)->GetValueType();

			if (valueType->GetUsage() == Type::Usage::Enumeration)
				return CreateRegularField(&TypeOf(EnumProperty), name);

			fieldSample = GetFieldPropertyPrototype(valueType);
			if (fieldSample)
				return CreateRegularField(&fieldSample->GetType(), name);
		}

		return Pair<IPropertyField*, UIWidget*>(nullptr, nullptr);
	}

	String PropertiesWindow::MakeSmartFieldName(const String& fieldName)
	{
		String begn;

		if (fieldName[0] == 'm' && fieldName[1] >= 'A' && fieldName[1] <= 'Z')
			begn = fieldName.SubStr(1);
		else if (fieldName[0] == 'm' && fieldName[1] == '_')
			begn = fieldName.SubStr(2);
		else if (fieldName[0] == '_')
			begn = fieldName.SubStr(1);
		else
			begn = fieldName;

		if (begn.StartsWith("o2::"))
			begn.Erase(0, 4);

		if (begn.StartsWith("Editor::"))
			begn.Erase(0, 9);

		String res;
		int len = begn.Length();
		bool newWord = true;
		for (int i = 0; i < len; i++)
		{
			if (begn[i] == '_')
			{
				res += ' ';
				newWord = true;
			}
			else if (newWord && begn[i] >= 'a' && begn[i] <= 'z')
			{
				res += begn[i] + ('A' - 'a');
			}
			else if (!newWord && begn[i] >= 'A' && begn[i] <= 'Z')
			{
				res += ' ';
				res += begn[i];
			}
			else if (i < len - 1 && begn[i] == ':' && begn[i + 1] == ':')
			{
				res += ": ";
				i++;
			}
			else
			{
				res += begn[i];
			}

			newWord = begn[i] >= '0' && begn[i] <= '9';
		}

		return res;
	}

	IPropertyField* PropertiesWindow::GetFieldPropertyPrototype(const Type* type) const
	{
		for (auto field : mAvailablePropertiesFields)
		{
			auto fieldType = field->GetFieldType();
			if (type->GetUsage() == Type::Usage::Pointer && fieldType->GetUsage() == Type::Usage::Pointer)
			{
				if (((PointerType*)type)->GetUnpointedType()->IsBasedOn(*((PointerType*)fieldType)->GetUnpointedType()))
					return field;
			}
			else if (type->IsBasedOn(*fieldType))
				return field;
		}

		return nullptr;
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateRegularField(const Type* fieldPropertyType,
																		  const String& name)
	{
		UIHorizontalLayout* horLayout = mnew UIHorizontalLayout();
		horLayout->name = name + " - " + fieldPropertyType->GetName();
		horLayout->spacing = 5.0f;
		horLayout->borderLeft = 10;
		horLayout->expandHeight = true;
		horLayout->expandWidth = true;
		horLayout->fitByChildren = true;
		horLayout->baseCorner = BaseCorner::Left;
		*horLayout->layout = UIWidgetLayout::BothStretch();
		horLayout->layout->minHeight = 20;

		UILabel* label = o2UI.CreateWidget<UILabel>();
		label->name = "propertyName";
		label->horAlign = HorAlign::Left;
		label->layout->widthWeight = 3.0f;
		label->horOverflow = UILabel::HorOverflow::Dots;
		label->text = name;

		IPropertyField* fieldProperty = (IPropertyField*)fieldPropertyType->CreateSample();
		fieldProperty->onChanged = [=]() { OnPropertyChanged(fieldProperty); };
		fieldProperty->GetWidget()->layout->minWidth = 200.0f;
		fieldProperty->SetCaptionLabel(label);

		horLayout->AddChild(label, false);
		horLayout->AddChild(fieldProperty->GetWidget(), false);

		return Pair<IPropertyField*, UIWidget*>(fieldProperty, horLayout);
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateObjectField(const Type* type, const String& name)
	{
		IPropertyField* fieldProperty = mnew ObjectProperty();		
		fieldProperty->onChanged = [=]() { OnPropertyChanged(fieldProperty); };
		fieldProperty->SetCaption(name);

		return Pair<IPropertyField*, UIWidget*>(fieldProperty, fieldProperty->GetWidget());
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateObjectPtrField(const Type* type, const String& name)
	{
		IPropertyField* fieldProperty = mnew ObjectPtrProperty();
		fieldProperty->onChanged = [=]() { OnPropertyChanged(fieldProperty); };
		fieldProperty->SetCaption(name);

		return Pair<IPropertyField*, UIWidget*>(fieldProperty, fieldProperty->GetWidget());
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateVectorField(const Type* type, const String& name)
	{
		if (!IsFieldTypeSupported(type))
			return Pair<IPropertyField*, UIWidget*>(nullptr, nullptr);

		IPropertyField* fieldProperty = mnew VectorProperty();
		fieldProperty->onChanged = [=]() { OnPropertyChanged(fieldProperty); };
		fieldProperty->SetCaption(name);

		return Pair<IPropertyField*, UIWidget*>(fieldProperty, fieldProperty->GetWidget());
	}
}

DECLARE_CLASS(Editor::PropertiesWindow);
