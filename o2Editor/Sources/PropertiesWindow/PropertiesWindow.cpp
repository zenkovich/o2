#include "PropertiesWindow.h"

#include "PropertiesWindow/IObjectPropertiesViewer.h"
#include "PropertiesWindow/Properties/AssetProperty.h"
#include "PropertiesWindow/Properties/BooleanProperty.h"
#include "PropertiesWindow/Properties/FloatProperty.h"
#include "PropertiesWindow/Properties/IntegerProperty.h"
#include "PropertiesWindow/Properties/StringProperty.h"
#include "PropertiesWindow/Properties/WStringProperty.h"
#include "PropertiesWindow/Properties/FieldPropertiesInfo.h"
#include "UI/HorizontalLayout.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

DECLARE_SINGLETON(Editor::PropertiesWindow);

namespace Editor
{
	PropertiesWindow::PropertiesWindow():
		mCurrentViewer(nullptr)
	{
		InitializeWindow();
		InitializeViewers();
		InitializePropertiesFields();
		InitializePools();
	}

	PropertiesWindow::~PropertiesWindow()
	{
		for (auto viewer : mViewers)
			delete viewer;

		for (auto field : mAvailablePropertiesFields)
			delete field;

		for (auto kv : mFieldPropertiesPool)
		{
			for (auto field : kv.Value())
				delete field;
		}
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
		auto viewersTypes = TypeOf(IObjectPropertiesViewer).DerivedTypes();

		for (auto type : viewersTypes)
			mViewers.Add((IObjectPropertiesViewer*)type->CreateSample());
	}

	void PropertiesWindow::InitializePropertiesFields()
	{
		auto a = TypeOf(IAssetProperty).DerivedTypes();
		auto b = TypeOf(IPropertyField).DerivedTypes();
		auto avaialbleTypes = a + b;

		avaialbleTypes.Remove(&TypeOf(IAssetProperty));

		const int initialPoolSize = 5;
		for (auto x : avaialbleTypes)
		{
			auto sample = (IAssetProperty*)x->CreateSample();
			mAvailablePropertiesFields.Add(sample);
		}
	}

	void PropertiesWindow::InitializePools()
	{
		return;

		const int initialPoolSize = 15;

		// properties pool
		for (auto fieldSample : mAvailablePropertiesFields)
		{
			mFieldPropertiesPool.Add(fieldSample->GetFieldType(), PropertiesFieldsVec());

			for (int i = 0; i < initialPoolSize; i++)
				mFieldPropertiesPool[fieldSample->GetFieldType()].Add((IAssetProperty*)fieldSample->GetType().CreateSample());
		}

		// horizontal layouts pool
		for (int i = 0; i < initialPoolSize; i++)
			mHorLayoutsPool.Add(CreatePropertyHorLayout());

		// labels pool
		for (int i = 0; i < initialPoolSize; i++)
			mLabelsPool.Add(CreatePropertyLabel());
	}

	UIHorizontalLayout* PropertiesWindow::CreatePropertyHorLayout()
	{
		UIHorizontalLayout* horLayout = mnew UIHorizontalLayout();
		horLayout->spacing = 5.0f;
		horLayout->border = RectF();
		horLayout->expandHeight = true;
		horLayout->expandWidth = true;
		horLayout->fitByChildren = true;
		horLayout->baseCorner = BaseCorner::Left;
		horLayout->layout = UIWidgetLayout::BothStretch();
		horLayout->layout.minHeight = 20;

		return horLayout;
	}

	UILabel* PropertiesWindow::CreatePropertyLabel()
	{
		auto newLabel = o2UI.CreateWidget<UILabel>();
		newLabel->horAlign = HorAlign::Left;
		newLabel->layout.minWidth = 100;
		newLabel->layout.widthWeight = 0.5f;

		return newLabel;
	}

	void PropertiesWindow::SetTarget(IObject* target)
	{
		SetTargets({ target });
	}

	void PropertiesWindow::SetTargets(const Vector<IObject*> targets)
	{
		IObjectPropertiesViewer* objectViewer = nullptr;
		if (!targets.IsEmpty())
		{
			auto type = &targets[0]->GetType();
			objectViewer = mViewers.FindMatch([&](auto x) { return x->GetViewingObjectType() == type; });
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
		// clear and fill pools
		Vector<UIWidget*> unknownTypeChilds;
		for (auto child : layout->GetChilds())
		{
			if (child->GetType() != TypeOf(UIHorizontalLayout))
			{
				unknownTypeChilds.Add(child);
				continue;
			}

			UIHorizontalLayout* childHorLayout = (UIHorizontalLayout*)child;
			if (childHorLayout->GetChilds()[0]->GetType() == TypeOf(UILabel))
				mLabelsPool.Add((UILabel*)childHorLayout->GetChilds()[0]);

			childHorLayout->RemoveAllChilds(false, false);

			mHorLayoutsPool.Add(childHorLayout);
		}

		layout->RemoveAllChilds(false, false);
		for (auto widget : unknownTypeChilds)
			delete widget;

		for (auto prop : propertiesInfo.properties)
			mFieldPropertiesPool[prop.Value()->GetFieldType()].Add(prop.Value());

		propertiesInfo.properties.Clear();

		// and build
		for (auto fieldInfo : type->Fields())
		{
			const Type* fieldType = &fieldInfo->GetType();

			if (fieldInfo->GetProtectionSection() != ProtectSection::Public)
				continue;

			IPropertyField* fieldSample = GetAvailableField(fieldType);

			if (!fieldSample)
				continue;

			const Type* fieldPropertyType = fieldSample->GetFieldType();

			if (mHorLayoutsPool.IsEmpty())
			{
				for (int i = 0; i < mPropertyFieldsPoolStep; i++)
					mHorLayoutsPool.Add(CreatePropertyHorLayout());
			}

			UIHorizontalLayout* horLayout = mHorLayoutsPool.PopBack();

			//add label
			if (mLabelsPool.IsEmpty())
			{
				for (int i = 0; i < mPropertyFieldsPoolStep; i++)
					mLabelsPool.Add(CreatePropertyLabel());
			}

			UILabel* label = mLabelsPool.PopBack();
			label->text = MakeSmartFieldName(fieldInfo->Name());

			// add property
			if (!mFieldPropertiesPool.ContainsKey(fieldPropertyType))
				mFieldPropertiesPool.Add(fieldPropertyType, Vector<IPropertyField*>());

			if (mFieldPropertiesPool[fieldPropertyType].IsEmpty())
			{
				for (int i = 0; i < mPropertyFieldsPoolStep; i++)
					mFieldPropertiesPool[fieldPropertyType].Add((IPropertyField*)fieldSample->GetType().CreateSample());
			}

			IPropertyField* fieldProperty = mFieldPropertiesPool[fieldPropertyType].PopBack();
			fieldProperty->SpecializeType(fieldType);

			propertiesInfo.properties.Add(fieldInfo, fieldProperty);

			// add to layout
			horLayout->AddChild(label, false);
			horLayout->AddChild(fieldProperty->GetWidget(), false);

			layout->AddChild(horLayout, false);
		}
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
			else
			{
				res += begn[i];
			}

			newWord = begn[i] >= '0' && begn[i] <= '9';
		}

		return res;
	}

	IPropertyField* PropertiesWindow::CreatePropertyField(const Type* type)
	{
		auto sample = GetAvailableField(type);
		if (sample)
			return (IPropertyField*)sample->GetType().CreateSample();

		return nullptr;
	}

	IPropertyField* PropertiesWindow::GetAvailableField(const Type* type)
	{
		for (auto field : mAvailablePropertiesFields)
		{
			if (type->IsPointer())
			{
				if (type->GetUnpointedType()->IsBasedOn(*field->GetFieldType()->GetUnpointedType()))
					return field;
			}
			else if (type->IsBasedOn(*field->GetFieldType()))
				return field;
		}

		return nullptr;
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
	PROTECTED_FIELD(mFieldPropertiesPool);
	PROTECTED_FIELD(mLabelsPool);
	PROTECTED_FIELD(mHorLayoutsPool);

	PUBLIC_FUNCTION(void, SetTarget, IObject*);
	PUBLIC_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PUBLIC_FUNCTION(Vector<IObject*>, GetTargets);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, BuildTypeViewer, UIVerticalLayout*, const Type*, FieldPropertiesInfo&);
	PUBLIC_FUNCTION(String, MakeSmartFieldName, const String&);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeViewers);
	PROTECTED_FUNCTION(void, InitializePropertiesFields);
	PROTECTED_FUNCTION(void, InitializePools);
	PROTECTED_FUNCTION(UIHorizontalLayout*, CreatePropertyHorLayout);
	PROTECTED_FUNCTION(UILabel*, CreatePropertyLabel);
	PROTECTED_FUNCTION(IPropertyField*, CreatePropertyField, const Type*);
	PROTECTED_FUNCTION(IPropertyField*, GetAvailableField, const Type*);
}
END_META;
 