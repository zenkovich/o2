#include "stdafx.h"
#include "VectorProperty.h"

#include "PropertiesWindow/Properties/IntegerProperty.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	VectorProperty::VectorProperty()
	{
		mSpoiler = o2UI.CreateWidget<UISpoiler>("expand with caption");
		mSpoiler->onExpand = THIS_FUNC(OnExpand);

		auto countPropertyProto = o2EditorProperties.GetFieldPropertyPrototype(&TypeOf(int));
		auto countPropertyWidget = o2EditorProperties.CreateRegularField(&countPropertyProto->GetType(), "Count");

		mCountProperty = (IntegerProperty*)countPropertyWidget.first;
		mSpoiler->AddChild(countPropertyWidget.second);

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
		delete mSpoiler;
	}

	void VectorProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mTargetObjects = targets;
		Refresh();
	}

	void VectorProperty::Refresh()
	{
		if (mTargetObjects.IsEmpty())
			return;

		if (!IsExpanded())
			return;

		mIsRefreshing = true;

		auto lastCount = mCountOfElements;
		auto lastDifferent = mCountDifferents;

		mCountOfElements = mType->GetObjectVectorSize(GetProxyValuePointer(mTargetObjects[0].first));
		mCountDifferents = false;

		for (auto target : mTargetObjects)
		{
			int targetCount = mType->GetObjectVectorSize(GetProxyValuePointer(target.first));
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
					mSpoiler->RemoveChild(prop.widget, false);
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
				auto itemTargetValues = mTargetObjects.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
					[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
				{
					return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
						mType->GetObjectVectorElementProxy(GetProxyValuePointer(x.first), i),
						x.second ? mType->GetObjectVectorElementProxy(GetProxyValuePointer(x.second), i) : nullptr);
				});

				PropertyDef propertyDef;
				if (i < mValueProperties.Count())
					propertyDef = mValueProperties[i];
				else
				{
					propertyDef = GetFreeValueProperty();
					mValueProperties.Add(propertyDef);
				}

				mSpoiler->AddChild(propertyDef.widget, false);
				propertyDef.propertyField->SetCaption((String)"Element " + (String)i);
				propertyDef.propertyField->SetValueAndPrototypeProxy(itemTargetValues);
				propertyDef.propertyField->SetValuePath((String)i);
				propertyDef.propertyField->onChangeCompleted =
					[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
				{
					onChangeCompleted(mValuesPath + "/" + path, before, after);
				};
			}

			for (; i < mValueProperties.Count(); i++)
			{
				mSpoiler->RemoveChild(mValueProperties[i].widget, false);
				FreeValueProperty(mValueProperties[i]);
			}

			mSpoiler->SetLayoutDirty();

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}
		else
		{
			for (int i = 0; i < mCountOfElements; i++)
			{
				auto itemTargetValues = mTargetObjects.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
					[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
				{
					return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
						mType->GetObjectVectorElementProxy(GetProxyValuePointer(x.first), i),
						x.second ? mType->GetObjectVectorElementProxy(GetProxyValuePointer(x.second), i) : nullptr);
				});

				PropertyDef propertyDef = mValueProperties[i];
				propertyDef.propertyField->SetValueAndPrototypeProxy(itemTargetValues);
			}
		}

		mIsRefreshing = false;
	}

	UIWidget* VectorProperty::GetWidget() const
	{
		return mSpoiler;
	}

	const Type* VectorProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void VectorProperty::SpecializeType(const Type* type)
	{
		mType = (VectorType*)type;
	}

	void VectorProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);
	}

	WString VectorProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
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
		mSpoiler->SetExpanded(expanded);
	}

	bool VectorProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	void* VectorProperty::GetProxyValuePointer(IAbstractValueProxy* proxy) const
	{
		IPointerValueProxy* pointerProxy = dynamic_cast<IPointerValueProxy*>(proxy);
		if (pointerProxy)
			return pointerProxy->GetValueVoidPointer();

		return nullptr;
	}

	VectorProperty::PropertyDef VectorProperty::GetFreeValueProperty()
	{
		if (!mValuePropertiesPool.IsEmpty())
			return mValuePropertiesPool.PopBack();

		PropertyDef res;
		auto fieldPropertySamplePair = o2EditorProperties.CreateFieldProperty(mType->GetElementType(), "Element");

		if (!fieldPropertySamplePair.first)
			return res;

		res.propertyField = fieldPropertySamplePair.first;
		res.propertyField->onChanged = [&]() { onChanged(); };
		res.widget = fieldPropertySamplePair.second;

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

	void VectorProperty::OnExpand()
	{
		Refresh();
	}

}

DECLARE_CLASS(Editor::VectorProperty);
