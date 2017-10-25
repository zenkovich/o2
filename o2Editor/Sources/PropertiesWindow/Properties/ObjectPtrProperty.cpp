#include "ObjectPtrProperty.h"

#include "PropertiesWindow/PropertiesWindow.h"
#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

using namespace o2;

namespace Editor
{
	ObjectPtrProperty::ObjectPtrProperty()
	{
		mLayout = mnew UIVerticalLayout();
		mLayout->spacing = 0.0f;
		mLayout->border = RectF(0, 0, 0, 0);
		mLayout->expandHeight = false;
		mLayout->expandWidth = true;
		mLayout->fitByChildren = true;
		mLayout->baseCorner = BaseCorner::RightTop;
		*mLayout->layout = UIWidgetLayout::BothStretch();

		auto header = mnew UIWidget();
		*header->layout = UIWidgetLayout::BothStretch();
		header->layout->minHeight = 20;

		mExpandButton = o2UI.CreateWidget<UIButton>("expand");
		*mExpandButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-7, 0));
		mExpandButton->onClick = [&]() { SetExpanded(!IsExpanded()); };
		header->AddChild(mExpandButton, false);

		mNameLabel = o2UI.CreateWidget<UILabel>();
		*mNameLabel->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, 10, 0, 20, 0);
		mNameLabel->horAlign = HorAlign::Left;
		header->AddChild(mNameLabel, false);

		mSpoiler = mnew UISpoiler();
		mPropertiesLayout = mnew UIVerticalLayout();
		mPropertiesLayout->spacing = 5.0f;
		mPropertiesLayout->borderLeft = 10;
		mPropertiesLayout->expandHeight = false;
		mPropertiesLayout->expandWidth = true;
		mPropertiesLayout->fitByChildren = true;
		mPropertiesLayout->baseCorner = BaseCorner::RightTop;
		*mPropertiesLayout->layout = UIWidgetLayout::BothStretch();
		mSpoiler->AddChild(mPropertiesLayout);

		mLayout->AddChild(header, false);
		mLayout->AddChild(mSpoiler, false);
	}

	ObjectPtrProperty::~ObjectPtrProperty()
	{
		delete mLayout;
	}

	void ObjectPtrProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mTargetObjects.Clear();
			return;
		}

		mTargetObjects = targets;

		Refresh();
	}

	void ObjectPtrProperty::Refresh()
	{
		for (auto targetObj : mTargetObjects)
		{
			if (*(IObject**)targetObj.first == nullptr)
				*((IObject**)(targetObj.first)) = (IObject*)mObjectType->CreateSample();
		}

		mFieldProperties.Set(mTargetObjects.Select<Pair<IObject*, IObject*>>([](const Pair<void*, void*>& x) { 
			return Pair<IObject*, IObject*>(*(IObject**)(x.first), 
											x.second ? *(IObject**)(x.second) : nullptr);
		}));
	}

	UIWidget* ObjectPtrProperty::GetWidget() const
	{
		return mLayout;
	}

	const Type* ObjectPtrProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void ObjectPtrProperty::SpecializeType(const Type* type)
	{
		if (type->GetUsage() != Type::Usage::Pointer)
			return;

		mObjectType = ((PointerType*)type)->GetUnpointedType();

		o2EditorProperties.BuildObjectProperties(mPropertiesLayout, mObjectType, mFieldProperties, "");

		auto onChangedFunc = [&]() { onChanged(); };

		auto onChangeCompletedFunc =
			[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		for (auto prop : mFieldProperties.properties)
		{
			prop.Value()->onChanged = onChangedFunc;
			prop.Value()->onChangeCompleted = onChangeCompletedFunc;
		}
	}

	void ObjectPtrProperty::Expand()
	{
		SetExpanded(true);
	}

	void ObjectPtrProperty::Collapse()
	{
		SetExpanded(false);
	}

	void ObjectPtrProperty::SetExpanded(bool expanded)
	{
		if (!expanded)
		{
			mSpoiler->Collapse();
			mExpandButton->SetState("expanded", false);
		}
		else
		{
			mSpoiler->Expand();
			mExpandButton->SetState("expanded", true);
		}
	}

	bool ObjectPtrProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	const FieldPropertiesInfo& ObjectPtrProperty::GetPropertiesInfo() const
	{
		return mFieldProperties;
	}

}

CLASS_META(Editor::ObjectPtrProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mObjectType);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mLayout);
	PROTECTED_FIELD(mNameLabel);
	PROTECTED_FIELD(mExpandButton);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mPropertiesLayout);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(const FieldPropertiesInfo&, GetPropertiesInfo);
}
END_META;
