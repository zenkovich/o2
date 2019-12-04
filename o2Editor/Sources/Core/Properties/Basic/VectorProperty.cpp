#include "stdafx.h"
#include "VectorProperty.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Basic/IntegerProperty.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	VectorProperty::VectorProperty()
	{
		InitializeControls();
	}

	VectorProperty::VectorProperty(const VectorProperty& other):
		IPropertyField(other)
	{
		InitializeControls();
	}

	VectorProperty& VectorProperty::operator=(const VectorProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void VectorProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();		
	}

	void VectorProperty::InitializeControls()
	{
		// Spoiler
		mSpoiler = FindChildByType<Spoiler>(false);

		if (!mSpoiler)
		{
			mSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");
			AddChild(mSpoiler);
		}

		if (mSpoiler)
			mSpoiler->onExpand = THIS_FUNC(OnExpand);

		// Count property
		mCountProperty = mSpoiler->FindChildByType<IntegerProperty>(false);
		if (!mCountProperty)
		{
			mCountProperty = dynamic_cast<IntegerProperty*>
				(o2EditorProperties.CreateFieldProperty(&TypeOf(int), "Count"));
		}

		if (mCountProperty)
		{
			if (mSpoiler)
				mSpoiler->AddChild(mCountProperty);

			mCountProperty->SetValue(0);
			mCountProperty->onChanged = THIS_FUNC(OnCountChanged);
		}

		// Add button
		mAddButtonContainer = mSpoiler->FindChildByTypeAndName<Widget>("add new");
		if (!mAddButtonContainer)
		{
			mAddButtonContainer = mnew Widget();
			mAddButtonContainer->name = "add new";
			mAddButtonContainer->layout->minHeight = 20;
			mSpoiler->AddChild(mAddButtonContainer);
		}

		mAddButton = mAddButtonContainer->FindChildByTypeAndName<Button>("add button");
		if (!mAddButton)
		{
			mAddButton = o2UI.CreateWidget<Button>("add small");
			mAddButton->name = "add button";
			*mAddButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0));
			mAddButton->onClick = THIS_FUNC(OnAddPressed);
			mAddButton->isPointInside = [=](const Vec2F& point) {
				return mAddButton->layout->IsPointInside(point) || mAddButtonContainer->IsUnderPoint(point);
			};

			mAddButtonContainer->AddChild(mAddButton);
		}

		// Other
		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	VectorProperty::~VectorProperty()
	{
		for (auto x : mValuePropertiesPool)
			delete x;
	}

	void VectorProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		for (auto& pair : mTargetObjects) {
			if (pair.first.isCreated)
				delete pair.first.data;

			if (pair.second.isCreated)
				delete pair.second.data;
		}

		mTargetObjects.Clear();

		for (auto& pair : targets)
			mTargetObjects.Add({ GetObjectFromProxy(pair.first), GetObjectFromProxy(pair.second) });

		Refresh();
	}

	void VectorProperty::Refresh()
	{
		PushEditorScopeOnStack scope;

		if (mTargetObjects.IsEmpty())
			return;

		if (!IsExpanded())
			return;
		
		mIsRefreshing = true;

		for (auto& pair : mTargetObjects) {
			pair.first.Refresh();
			pair.second.Refresh();
		}

		auto lastCount = mCountOfElements;
		auto lastDifferent = mCountDifferents;

		mCountOfElements = mType->GetObjectVectorSize(mTargetObjects[0].first.data);
		mCountDifferents = false;

		for (auto target : mTargetObjects)
		{
			int targetCount = mType->GetObjectVectorSize(target.first.data);
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
					mSpoiler->RemoveChild(prop, false);
					FreeValueProperty(prop);
				}

				mValueProperties.Clear();

				mAddButtonContainer->Hide(true);

				onChanged(this);
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
					[&](const Pair<TargetObjectData, TargetObjectData>& x)
				{
					return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
						mType->GetObjectVectorElementProxy(x.first.data, i),
						x.second.data ? mType->GetObjectVectorElementProxy(x.second.data, i) : nullptr);
				});

				IPropertyField* propertyDef;
				if (i < mValueProperties.Count())
					propertyDef = mValueProperties[i];
				else
				{
					propertyDef = GetFreeValueProperty();
					mValueProperties.Add(propertyDef);
				}

				mSpoiler->AddChild(propertyDef);
				propertyDef->SetCaption((String)"# " + (String)i);
				propertyDef->SetValueAndPrototypeProxy(itemTargetValues);
				propertyDef->SetValuePath((String)i);
				propertyDef->GetRemoveButton()->onClick = [=]() { Remove(i); };

				propertyDef->onChangeCompleted =
					[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
				{
					OnPropertyChanged(mValuesPath + "/" + path, before, after);
				};
			}

			for (; i < mValueProperties.Count(); i++)
			{
				mSpoiler->RemoveChild(mValueProperties[i], false);
				FreeValueProperty(mValueProperties[i]);
			}

			mValueProperties.Resize(mCountOfElements);

			mAddButtonContainer->Show(true);
			mAddButtonContainer->SetIndexInSiblings(mSpoiler->GetChildren().Count());

			mSpoiler->SetLayoutDirty();

			onChanged(this);
			o2EditorSceneScreen.OnSceneChanged();
		}
		else
		{
			for (int i = 0; i < mCountOfElements; i++)
			{
				auto itemTargetValues = mTargetObjects.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
					[&](const Pair<TargetObjectData, TargetObjectData>& x)
				{
					return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
						mType->GetObjectVectorElementProxy(x.first.data, i),
						x.second.data ? mType->GetObjectVectorElementProxy(x.second.data, i) : nullptr);
				});

				IPropertyField* propertyDef = mValueProperties[i];
				propertyDef->SetValueAndPrototypeProxy(itemTargetValues);
			}
		}

		mIsRefreshing = false;
	}

	const Type* VectorProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void VectorProperty::SpecializeType(const Type* type)
	{
		mType = nullptr;

		if (type->GetUsage() == Type::Usage::Vector)
			mType = dynamic_cast<const VectorType*>(type);
		else if (type->GetUsage() == Type::Usage::Property) 
		{
			auto propertyType = dynamic_cast<const PropertyType*>(type);

			if (propertyType->GetValueType()->GetUsage() == Type::Usage::Vector)
				mType = dynamic_cast<const VectorType*>(propertyType->GetValueType());
		}
	}

	const Type* VectorProperty::GetSpecializedType() const
	{
		return mType;
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
		IPointerValueProxy* variableProxy = dynamic_cast<IPointerValueProxy*>(proxy);
		if (variableProxy)
			return variableProxy->GetValueVoidPointer();

		return nullptr;
	}

	VectorProperty::IPropertyField* VectorProperty::GetFreeValueProperty()
	{
		if (!mValuePropertiesPool.IsEmpty())
			return mValuePropertiesPool.PopBack();

		IPropertyField* res = o2EditorProperties.CreateFieldProperty(mType->GetElementType(), "Element", onChangeCompleted, onChanged);
		res->AddLayer("drag", mnew Sprite("ui/UI4_drag_handle.png"), Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-16, 0)));

		if (res)
			res->SpecializeType(mType->GetElementType());

		return res;
	}

	void VectorProperty::FreeValueProperty(IPropertyField* def)
	{
		mValuePropertiesPool.Add(def);
	}

	void VectorProperty::OnCountChanged(IPropertyField* def)
	{
		if (mIsRefreshing)
			return;

		Resize(mCountProperty->GetCommonValue());
	}

	void VectorProperty::OnAddPressed()
	{
		Resize(mCountOfElements + 1);
	}

	void VectorProperty::OnExpand()
	{
		Refresh();
	}

	VectorProperty::TargetObjectData VectorProperty::GetObjectFromProxy(IAbstractValueProxy* proxy)
	{
		TargetObjectData res;

		if (!proxy)
			return res;

		res.proxy = proxy;

		if (auto pointerProxy = dynamic_cast<IPointerValueProxy*>(proxy)) {
			res.data = pointerProxy->GetValueVoidPointer();
			res.isCreated = false;
		}
		else {
			void* sample = proxy->GetType().CreateSample();
			proxy->GetValuePtr(sample);

			res.data = sample;
			res.isCreated = true;
		}

		return res;
	}

	void VectorProperty::OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		for (auto& pair : mTargetObjects)
			pair.first.SetValue();

		onChangeCompleted(path, before, after);
	}

	void VectorProperty::Resize(int newCount)
	{
		Vector<DataNode> prevValues, newValues;
		auto elementFieldInfo = mType->GetElementFieldInfo();

		for (auto& obj : mTargetObjects)
		{
			prevValues.Add(DataNode());
			prevValues.Last()["Size"].SetValue(mType->GetObjectVectorSize(obj.first.data));
			DataNode& elementsData = prevValues.Last()["Elements"];

			int lastCount = mType->GetObjectVectorSize(obj.first.data);
			for (int i = newCount; i < lastCount; i++)
			{
				elementFieldInfo->Serialize(mType->GetObjectVectorElementPtr(obj.first.data, i),
											*elementsData.AddNode("Element" + (String)i));
			}

			newValues.Add(DataNode());
			newValues.Last()["Size"].SetValue(newCount);

			mType->SetObjectVectorSize(obj.first.data, newCount);
		}

		Refresh();

		if (prevValues != newValues)
			onChangeCompleted(mValuesPath + "/count", prevValues, newValues);

		onChanged(this);
		o2EditorSceneScreen.OnSceneChanged();
	}

	void VectorProperty::Remove(int idx)
	{
		Vector<DataNode> prevValues, newValues;
		auto elementFieldInfo = mType->GetElementFieldInfo();

		for (auto& obj : mTargetObjects)
		{
			prevValues.Add(DataNode());
			mType->Serialize(obj.first.data, prevValues.Last());

			mType->RemoveObjectVectorElement(obj.first.data, idx);

			newValues.Add(DataNode());
			mType->Serialize(obj.first.data, newValues.Last());
		}

		Refresh();

		if (prevValues != newValues)
			onChangeCompleted(mValuesPath, prevValues, newValues);

		onChanged(this);
		o2EditorSceneScreen.OnSceneChanged();
	}

	void VectorProperty::TargetObjectData::Refresh()
	{
		if (!isCreated)
			return;

		proxy->GetValuePtr(data);
	}

	void VectorProperty::TargetObjectData::SetValue()
	{
		if (!isCreated)
			return;

		proxy->SetValuePtr(data);
	}

}

DECLARE_CLASS(Editor::VectorProperty);
