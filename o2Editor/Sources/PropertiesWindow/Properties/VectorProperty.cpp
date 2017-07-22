#include "VectorProperty.h"

#include "PropertiesWindow/Properties/IntegerProperty.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"

namespace Editor
{
	VectorProperty::VectorProperty()
	{
		mLayout = mnew UIVerticalLayout();
		mLayout->spacing = 0.0f;
		mLayout->border = RectF(0, 0, 0, 0);
		mLayout->expandHeight = false;
		mLayout->expandWidth = true;
		mLayout->fitByChildren = true;
		mLayout->baseCorner = BaseCorner::RightTop;
		mLayout->layout = UIWidgetLayout::BothStretch();

		auto header = mnew UIWidget();
		header->layout = UIWidgetLayout::BothStretch();
		header->layout.minHeight = 20;

		mExpandButton = o2UI.CreateWidget<UIButton>("expand");
		mExpandButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-7, 0));
		mExpandButton->onClick = [&]() { SetExpanded(!IsExpanded()); };
		header->AddChild(mExpandButton, false);

		mNameLabel = o2UI.CreateWidget<UILabel>();
		mNameLabel->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, 10, 0, 20, 0);
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
		mPropertiesLayout->layout = UIWidgetLayout::BothStretch();
		mSpoiler->AddChild(mPropertiesLayout);

		mLayout->AddChild(header, false);
		mLayout->AddChild(mSpoiler, false);

		auto countPropertyProto = o2EditorProperties.GetFieldPropertyPrototype(&TypeOf(int));
		auto countPropertyWidget = o2EditorProperties.CreateRegularField(&countPropertyProto->GetType(), "Count");

		mCountProperty = (IntegerProperty*)countPropertyWidget.first;
		mPropertiesLayout->AddChild(countPropertyWidget.second);

		mCountProperty->onChanged = THIS_FUNC(OnCountChanged);
	}

	VectorProperty::~VectorProperty()
	{
		for (auto x : mValuePropertiesPool)
		{
			delete x.propertyField;
			delete x.widget;
		}

		delete mCountProperty;
		delete mLayout;
	}

	void VectorProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mTargetObjects.Clear();
			return;
		}

		mTargetObjects = targets;
		Refresh();
	}

	void VectorProperty::Refresh()
	{
		if (mTargetObjects.IsEmpty())
			return;

		mIsRefreshing = true;

		auto lastCount = mCountOfElements;
		auto lastDifferent = mCountDifferents;

		mCountOfElements = mType->GetObjectVectorSize(mTargetObjects[0].first);
		mCountDifferents = false;

		for (auto target : mTargetObjects)
		{
			int targetCount = mType->GetObjectVectorSize(target.first);
			if (targetCount != mCountOfElements)
			{
				mCountDifferents = true;
				break;
			}
		}

		if (mCountDifferents)
		{
			if (!lastDifferent)
			{
				mCountProperty->SetUnknownValue();

				for (auto prop : mValueProperties)
				{
					mPropertiesLayout->RemoveChild(prop.widget, false);
					FreeValueProperty(prop);
				}

				mValueProperties.Clear();

				onChanged();
				o2EditorSceneScreen.OnSceneChanged();
			}
		}
		else if (lastDifferent || lastCount != mCountOfElements)
		{
			mCountProperty->SetValue(mCountOfElements);

			int i = 0;
			for (; i < mCountOfElements; i++)
			{
				Vector<Pair<void*, void*>> itemTargetValues = 
					mTargetObjects.Select<Pair<void*,void*>>([&](const Pair<void*, void*>& x) 
				{
					return Pair<void*, void*>(mType->GetObjectVectorElementPtr(x.first, i),
											  x.second ? mType->GetObjectVectorElementPtr(x.second, i) : nullptr);
				});

				PropertyDef propertyDef;
				if (i < mValueProperties.Count())
					propertyDef = mValueProperties[i];
				else
				{
					propertyDef = GetFreeValueProperty();
					mValueProperties.Add(propertyDef);
				}

				mPropertiesLayout->AddChild(propertyDef.widget, false);
				propertyDef.nameLabel->text = (String)"Element " + (String)i;
				propertyDef.propertyField->SetValueAndPrototypePtr(itemTargetValues, false);
				propertyDef.propertyField->SetValuePath((String)i);
				propertyDef.propertyField->onChangeCompleted = 
					[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
				{
					onChangeCompleted(mValuesPath + "/" + path, before, after);
				};
			}

			for (; i < mValueProperties.Count(); i++)
			{
				mPropertiesLayout->RemoveChild(mValueProperties[i].widget, false);
				FreeValueProperty(mValueProperties[i]);
			}

			mPropertiesLayout->UpdateLayout();

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}
		else
		{
			for (int i = 0; i < mCountOfElements; i++)
			{
				Vector<Pair<void*, void*>> itemTargetValues =
					mTargetObjects.Select<Pair<void*, void*>>([&](const Pair<void*, void*>& x)
				{
					return Pair<void*, void*>(mType->GetObjectVectorElementPtr(x.first, i),
											  x.second ? mType->GetObjectVectorElementPtr(x.second, i) : nullptr);
				});

				PropertyDef propertyDef = mValueProperties[i];
				propertyDef.propertyField->SetValueAndPrototypePtr(itemTargetValues, false);
			}
		}

		mIsRefreshing = false;
	}

	UIWidget* VectorProperty::GetWidget() const
	{
		return mLayout;
	}

	const Type* VectorProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void VectorProperty::SpecializeType(const Type* type)
	{
		mType = (VectorType*)type;
		mNameLabel->text = type->GetName();
	}

	void VectorProperty::Expand()
	{
		SetExpanded(true);
	}

	void VectorProperty::Collapse()
	{
		SetExpanded(false);
	}

	void VectorProperty::SetExpanded(bool expanded)
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

	bool VectorProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	VectorProperty::PropertyDef VectorProperty::GetFreeValueProperty()
	{
		if (!mValuePropertiesPool.IsEmpty())
			return mValuePropertiesPool.PopBack();

		PropertyDef res;
		auto fieldPropertySamplePair = o2EditorProperties.CreateFieldProperty(mType->GetElementType());

		res.propertyField = fieldPropertySamplePair.first;
		res.propertyField->onChanged = [&]() { onChanged(); };
		res.widget = fieldPropertySamplePair.second;
		res.nameLabel = res.widget->FindChild<UILabel>();

		res.propertyField->SpecializeType(mType->GetElementType());

		return res;
	}

	void VectorProperty::FreeValueProperty(PropertyDef def)
	{
		mValuePropertiesPool.Add(def);
	}

	void VectorProperty::OnCountChanged()
	{
		if (mIsRefreshing)
			return;

		Vector<DataNode> prevValues, newValues;

		int newCount = mCountProperty->GetCommonValue();
		auto countFI = mType->GetElementFieldInfo();
		for (auto target : mTargetObjects)
		{
			prevValues.Add(DataNode());
			prevValues.Last()["Size"].SetValue(mType->GetObjectVectorSize(target.first));
			DataNode& elementsData = prevValues.Last()["Elements"];

			int lastCount = mType->GetObjectVectorSize(target.first);
			for (int i = newCount; i < lastCount; i++)
				countFI->Serialize(mType->GetObjectVectorElementPtr(target.first, i), *elementsData.AddNode("Element" + (String)i));

			newValues.Add(DataNode());
			newValues.Last()["Size"].SetValue(newCount);

			mType->SetObjectVectorSize(target.first, newCount);
		}

		Refresh();

		if (prevValues != newValues)
			onChangeCompleted(mValuesPath + "/count", prevValues, newValues);

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}
}

CLASS_META(Editor::VectorProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mType);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mLayout);
	PROTECTED_FIELD(mNameLabel);
	PROTECTED_FIELD(mExpandButton);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mPropertiesLayout);
	PROTECTED_FIELD(mValueProperties);
	PROTECTED_FIELD(mValuePropertiesPool);
	PROTECTED_FIELD(mCountProperty);
	PROTECTED_FIELD(mCountDifferents);
	PROTECTED_FIELD(mCountOfElements);
	PROTECTED_FIELD(mIsRefreshing);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PROTECTED_FUNCTION(PropertyDef, GetFreeValueProperty);
	PROTECTED_FUNCTION(void, FreeValueProperty, PropertyDef);
	PROTECTED_FUNCTION(void, OnCountChanged);
}
END_META;
