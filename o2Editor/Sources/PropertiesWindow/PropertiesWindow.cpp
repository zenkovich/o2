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
#include "Utils/EditorPropertyAttribute.h"
#include "Utils/Timer.h"

DECLARE_SINGLETON(Editor::PropertiesWindow);

namespace Editor
{
	PropertiesWindow::PropertiesWindow():
		mCurrentViewer(nullptr)
	{
		InitializeWindow();
		InitializeViewers();
		InitializePropertiesFields();
	}

	PropertiesWindow::~PropertiesWindow()
	{
		for (auto viewer : mViewers)
			delete viewer;

		for (auto field : mAvailablePropertiesFields)
			delete field;
	}

	void PropertiesWindow::InitializeWindow()
	{
		mWindow->caption = "Properties";
		mWindow->name = "properties window";
		if (auto iconLayer = mWindow->GetLayer("icon"))
		{
			*((Sprite*)iconLayer->drawable) = Sprite("ui/UI2_gear_icon.png");
			iconLayer->layout = Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2));
		}

		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, -2, 0, 15));
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

	void PropertiesWindow::SetTarget(IObject* target)
	{
		SetTargets({ target });
	}

	void PropertiesWindow::SetTargets(const Vector<IObject*> targets)
	{
		Timer timer;

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
				mCurrentViewer->mContentWidget->layout = UIWidgetLayout::BothStretch();
				mCurrentViewer->mContentWidget->Show(true);
				mCurrentViewer->OnEnabled();
			}
		}

		mTargets = targets;

		if (mCurrentViewer)
			mCurrentViewer->SetTargets(mTargets);

		o2Debug.Log("Set targets time: %f sec", timer.GetTime());
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

	void PropertiesWindow::BuildTypeViewer(UIVerticalLayout* layout, const Type* type, FieldPropertiesInfo& propertiesInfo)
	{
		auto fields = type->GetFieldsWithBaseClasses();
		for (auto fieldInfo : fields)
		{
			const Type* fieldType = fieldInfo->GetType();

			if (fieldInfo->GetProtectionSection() != ProtectSection::Public &&
				!fieldInfo->HasAttribute<SerializableAttribute>() && !fieldInfo->HasAttribute<EditorPropertyAttribute>())
			{
				continue;
			}

			auto fieldWidgetPair = CreateFieldProperty(fieldInfo->GetType());
			if (!fieldWidgetPair.mFirst)
				continue;

			fieldWidgetPair.mFirst->SpecializeType(fieldType);

			auto nameLabel = fieldWidgetPair.mSecond->FindChild<UILabel>();
			nameLabel->text = MakeSmartFieldName(fieldInfo->GetName());

			layout->AddChild(fieldWidgetPair.mSecond, false);
			propertiesInfo.properties.Add(fieldInfo, fieldWidgetPair.mFirst);
		}
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateFieldProperty(const Type* type)
	{
		if (type->GetUsage() == Type::Usage::Vector)
			return CreateVectorField(type);

		IPropertyField* fieldSample = GetFieldPropertyPrototype(type);
		if (fieldSample)
			return CreateRegularField(&fieldSample->GetType(), type->GetName());

		if (type->IsBasedOn(TypeOf(IObject)))
			return CreateObjectField(type);

		if (type->GetUsage() == Type::Usage::Pointer && ((PointerType*)type)->GetUnpointedType()->IsBasedOn((TypeOf(IObject))))
			return CreateObjectPtrField(type);

		if (type->GetUsage() == Type::Usage::Enumeration)
			return CreateRegularField(&TypeOf(EnumProperty), type->GetName());

		if (type->GetUsage() == Type::Usage::Property)
		{
			auto valueType = ((const PropertyType*)type)->GetValueType();

			if (valueType->GetUsage() == Type::Usage::Enumeration)
				return CreateRegularField(&TypeOf(EnumProperty), valueType->GetName());

			fieldSample = GetFieldPropertyPrototype(valueType);
			if (fieldSample)
				return CreateRegularField(&fieldSample->GetType(), valueType->GetName());
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

	IPropertyField* PropertiesWindow::GetFieldPropertyPrototype(const Type* type)
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
		horLayout->spacing = 5.0f;
		horLayout->borderLeft = 10;
		horLayout->expandHeight = true;
		horLayout->expandWidth = true;
		horLayout->fitByChildren = true;
		horLayout->baseCorner = BaseCorner::Left;
		horLayout->layout = UIWidgetLayout::BothStretch();
		horLayout->layout.minHeight = 20;

		UILabel* label = o2UI.CreateWidget<UILabel>();
		label->horAlign = HorAlign::Left;
		label->layout.minWidth = 100;
		label->layout.widthWeight = 0.5f;
		label->text = MakeSmartFieldName(name);

		IPropertyField* fieldProperty = (IPropertyField*)fieldPropertyType->CreateSample();

		horLayout->AddChild(label, false);
		horLayout->AddChild(fieldProperty->GetWidget(), false);

		return Pair<IPropertyField*, UIWidget*>(fieldProperty, horLayout);
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateObjectField(const Type* type)
	{
		IPropertyField* fieldProperty = mnew ObjectProperty();
		//fieldProperty->SpecializeType(type);
		return Pair<IPropertyField*, UIWidget*>(fieldProperty, fieldProperty->GetWidget());
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateObjectPtrField(const Type* type)
	{
		IPropertyField* fieldProperty = mnew ObjectPtrProperty();
		//fieldProperty->SpecializeType(type);
		return Pair<IPropertyField*, UIWidget*>(fieldProperty, fieldProperty->GetWidget());
	}

	Pair<IPropertyField*, UIWidget*> PropertiesWindow::CreateVectorField(const Type* type)
	{
		IPropertyField* fieldProperty = mnew VectorProperty();
		//fieldProperty->SpecializeType(type);
		return Pair<IPropertyField*, UIWidget*>(fieldProperty, fieldProperty->GetWidget());
	}
}
 
CLASS_META(Editor::PropertiesWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::Singleton<PropertiesWindow>);

	PROTECTED_FIELD(mPropertyFieldsPoolStep);
	PROTECTED_FIELD(mTargets);
	PROTECTED_FIELD(mCurrentViewer);
	PROTECTED_FIELD(mViewers);
	PROTECTED_FIELD(mAvailablePropertiesFields);

	typedef Pair<IPropertyField*, UIWidget*> _tmp1;
	typedef Pair<IPropertyField*, UIWidget*> _tmp2;
	typedef Pair<IPropertyField*, UIWidget*> _tmp3;
	typedef Pair<IPropertyField*, UIWidget*> _tmp4;
	typedef Pair<IPropertyField*, UIWidget*> _tmp5;

	PUBLIC_FUNCTION(void, SetTarget, IObject*);
	PUBLIC_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PUBLIC_FUNCTION(Vector<IObject*>, GetTargets);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, BuildTypeViewer, UIVerticalLayout*, const Type*, FieldPropertiesInfo&);
	PUBLIC_FUNCTION(_tmp1, CreateFieldProperty, const Type*);
	PUBLIC_FUNCTION(IPropertyField*, GetFieldPropertyPrototype, const Type*);
	PUBLIC_FUNCTION(_tmp2, CreateRegularField, const Type*, const String&);
	PUBLIC_FUNCTION(_tmp3, CreateObjectField, const Type*);
	PUBLIC_FUNCTION(_tmp4, CreateObjectPtrField, const Type*);
	PUBLIC_FUNCTION(_tmp5, CreateVectorField, const Type*);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeViewers);
	PROTECTED_FUNCTION(void, InitializePropertiesFields);
}
END_META;
 