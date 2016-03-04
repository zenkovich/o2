#include "PropertiesWindow.h"

#include "PropertiesWindow/IObjectPropertiesViewer.h"
#include "PropertiesWindow/Properties/BooleanProperty.h"
#include "PropertiesWindow/Properties/FloatProperty.h"
#include "PropertiesWindow/Properties/IntegerProperty.h"
#include "PropertiesWindow/Properties/StringProperty.h"
#include "PropertiesWindow/Properties/WStringProperty.h"
#include "UI/HorizontalLayout.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

DECLARE_SINGLETON(PropertiesWindow);

PropertiesWindow::PropertiesWindow():
	mCurrentViewer(nullptr)
{
	InitializeWindow();
	InitializeViewers();
}

PropertiesWindow::~PropertiesWindow()
{
	for (auto viewer : mViewers)
		delete viewer;
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
	mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));
}

void PropertiesWindow::InitializeViewers()
{
	auto viewersTypes = IObjectPropertiesViewer::type.DerivedTypes();

	for (auto type : viewersTypes)
		mViewers.Add((IObjectPropertiesViewer*)type->CreateSample());
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

void PropertiesWindow::BuildTypeViewer(UIVerticalLayout* layout, const Type* type, const Vector<IObject*>& objects,
									   Vector<IEditorPropertyField*>& usedPropertyFields)
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

		childHorLayout->RemoveAllChilds(false);

		mHorLayoutsPool.Add(childHorLayout);
	}

	layout->RemoveAllChilds(false);
	for (auto widget : unknownTypeChilds)
		delete widget;

	for (auto field : usedPropertyFields)
		mFieldPropertiesPool[field->GetFieldType()].Add(field);

	usedPropertyFields.Clear();

	Vector<const Type*> availableTypes = { &TypeOf(int), &TypeOf(float), &TypeOf(bool), &TypeOf(String), &TypeOf(WString) };

	// and build
	for (auto fieldInfo : type->Fields())
	{
		const Type* fieldType = &fieldInfo->GetType();

		if (!availableTypes.Contains(fieldType))
			continue;

		if (fieldInfo->GetProtectionSection() != ProtectSection::Public)
			continue;

		if (mHorLayoutsPool.IsEmpty())
		{
			for (int i = 0; i < mPropertyFieldsPoolStep; i++)
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

				mHorLayoutsPool.Add(horLayout);
			}
		}

		UIHorizontalLayout* horLayout = mHorLayoutsPool.PopBack();

		//add label
		if (mLabelsPool.IsEmpty())
		{
			for (int i = 0; i < mPropertyFieldsPoolStep; i++)
				mLabelsPool.Add(o2UI.CreateWidget<UILabel>());
		}

		UILabel* label = mLabelsPool.PopBack();
		label->text = MakeSmartFieldName(fieldInfo->Name());
		label->horAlign = HorAlign::Left;
		label->layout.minWidth = 100;

		// add property
		if (!mFieldPropertiesPool.ContainsKey(fieldType))
			mFieldPropertiesPool.Add(fieldType, Vector<IEditorPropertyField*>());

		if (mFieldPropertiesPool[fieldType].IsEmpty())
		{
			for (int i = 0; i < mPropertyFieldsPoolStep; i++)
				mFieldPropertiesPool[fieldType].Add(CreatePropertyField(fieldType));
		}

		IEditorPropertyField* fieldProperty = mFieldPropertiesPool[fieldType].PopBack();
		Vector<void*> fieldPointers = objects.Select<void*>([&](IObject* x) { return fieldInfo->GetValuePtr<char>(x); });
		fieldProperty->Setup(fieldPointers, fieldInfo->IsProperty());

		usedPropertyFields.Add(fieldProperty);

		// add to layout
		horLayout->AddChild(label);
		horLayout->AddChild(fieldProperty->GetWidget());

		layout->AddChild(horLayout);
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

IEditorPropertyField* PropertiesWindow::CreatePropertyField(const Type* type)
{
	if (type == &TypeOf(int))
		return mnew EditorIntegerProperty();
	else if (type == &TypeOf(float))
		return mnew EditorFloatProperty();
	else if (type == &TypeOf(String))
		return mnew EditorStringProperty();
	else if (type == &TypeOf(WString))
		return mnew EditorWStringProperty();
	else if (type == &TypeOf(bool))
		return mnew EditorBooleanProperty();

	return nullptr;
}