#include "PropertiesWindow.h"

#include "PropertiesWindow/IObjectPropertiesViewer.h"
#include "PropertiesWindow/Properties/AssetProperty.h"
#include "PropertiesWindow/Properties/BooleanProperty.h"
#include "PropertiesWindow/Properties/FieldPropertiesInfo.h"
#include "PropertiesWindow/Properties/FloatProperty.h"
#include "PropertiesWindow/Properties/IntegerProperty.h"
#include "PropertiesWindow/Properties/StringProperty.h"
#include "PropertiesWindow/Properties/WStringProperty.h"
#include "UI/HorizontalLayout.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
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
		for (auto fieldInfo : type->Fields())
		{
			const Type* fieldType = &fieldInfo->GetType();

			if (fieldInfo->GetProtectionSection() != ProtectSection::Public)
				continue;

			if (IPropertyField* fieldSample = GetAvailableField(fieldType))
				CreateRegularField(fieldSample, fieldInfo, propertiesInfo, layout);
			else if (fieldType->IsBasedOn(TypeOf(IObject)))
				CreateObjectField(fieldInfo, propertiesInfo, layout);
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

	void PropertiesWindow::CreateRegularField(IPropertyField* fieldSample, FieldInfo* fieldInfo,
											  FieldPropertiesInfo &propertiesInfo, UIVerticalLayout* layout)
	{
		const Type* fieldPropertyType = fieldSample->GetFieldType();

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
		label->text = MakeSmartFieldName(fieldInfo->Name());

		IPropertyField* fieldProperty = (IPropertyField*)fieldSample->GetType().CreateSample();
		fieldProperty->SpecializeType(&fieldInfo->GetType());

		propertiesInfo.properties.Add(fieldInfo, fieldProperty);

		horLayout->AddChild(label, false);
		horLayout->AddChild(fieldProperty->GetWidget(), false);

		layout->AddChild(horLayout, false);
	}

	void PropertiesWindow::CreateObjectField(FieldInfo* fieldInfo, FieldPropertiesInfo &propertiesInfo,
											 UIVerticalLayout* layout)
	{
		UIVerticalLayout* objLayout = mnew UIVerticalLayout();
		objLayout->spacing = 0.0f;
		objLayout->border = RectF(0, 0, 0, 0);
		objLayout->expandHeight = false;
		objLayout->expandWidth = true;
		objLayout->fitByChildren = true;
		objLayout->baseCorner = BaseCorner::RightTop;
		objLayout->layout = UIWidgetLayout::BothStretch();

		auto header = mnew UIWidget();
		header->layout = UIWidgetLayout::BothStretch();
		header->layout.minHeight = 20;

		auto expandBtn = o2UI.CreateWidget<UIButton>("expand");
		expandBtn->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-7, 0));
		header->AddChild(expandBtn, false);

		UILabel* label = o2UI.CreateWidget<UILabel>();
		label->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, 10, 0, 20, 0);
		label->horAlign = HorAlign::Left;
		label->text = MakeSmartFieldName(fieldInfo->Name());
		header->AddChild(label, false);

		auto spoiler = mnew UISpoiler();
		auto spoilerVerLayout = mnew UIVerticalLayout();
		spoilerVerLayout->spacing = 5.0f;
		spoilerVerLayout->borderLeft = 10;
		spoilerVerLayout->expandHeight = false;
		spoilerVerLayout->expandWidth = true;
		spoilerVerLayout->fitByChildren = true;
		spoilerVerLayout->baseCorner = BaseCorner::RightTop;
		spoilerVerLayout->layout = UIWidgetLayout::BothStretch();
		spoiler->AddChild(spoilerVerLayout);

		expandBtn->onClick = [=]() {
			if (spoiler->IsExpanded())
			{
				spoiler->Collapse();
				expandBtn->SetState("expanded", false);
			}
			else
			{
				spoiler->Expand();
				expandBtn->SetState("expanded", true);
			}
		};

		objLayout->AddChild(header, false);
		objLayout->AddChild(spoiler, false);

		BuildTypeViewer(spoilerVerLayout, &fieldInfo->GetType(), propertiesInfo);

		layout->AddChild(objLayout, true);
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
	PROTECTED_FUNCTION(IPropertyField*, CreatePropertyField, const Type*);
	PROTECTED_FUNCTION(IPropertyField*, GetAvailableField, const Type*);
	PROTECTED_FUNCTION(void, CreateRegularField, IPropertyField*, FieldInfo*, FieldPropertiesInfo&, UIVerticalLayout*);
	PROTECTED_FUNCTION(void, CreateObjectField, FieldInfo*, FieldPropertiesInfo&, UIVerticalLayout*);
}
END_META;
 