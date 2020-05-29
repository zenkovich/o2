#include "o2Editor/stdafx.h"
#include "VectorProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

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

		mSpoiler->borderTop = 5;

		mHeaderContainer = mnew HorizontalLayout();
		*mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, 100, 0, 20, 0);
		mHeaderContainer->baseCorner = BaseCorner::Right;
		mHeaderContainer->expandHeight = false;
		mHeaderContainer->SetInternalParent(mSpoiler, false);

		// Count property
		mCountProperty = mSpoiler->FindChildByType<IntegerProperty>(false);
		if (!mCountProperty)
			mCountProperty = o2UI.CreateWidget<IntegerProperty>();

		if (mCountProperty)
		{
			mHeaderContainer->AddChild(mCountProperty);

			mCountProperty->layout->maxWidth = 100;
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
			*mAddButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(2, 0));
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

		for (auto& pair : mTargetObjects) 
		{
			pair.first.Refresh();
			pair.second.Refresh();
		}

		auto lastCount = mCountOfElements;
		auto lastDifferent = mCountDifferents;

		mCountOfElements = mVectorType->GetObjectVectorSize(mTargetObjects[0].first.data);
		mCountDifferents = false;

		for (auto target : mTargetObjects)
		{
			int targetCount = mVectorType->GetObjectVectorSize(target.first.data);
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
						mVectorType->GetObjectVectorElementProxy(x.first.data, i),
						x.second.data ? mVectorType->GetObjectVectorElementProxy(x.second.data, i) : nullptr);
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
					[&](const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
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
						mVectorType->GetObjectVectorElementProxy(x.first.data, i),
						x.second.data ? mVectorType->GetObjectVectorElementProxy(x.second.data, i) : nullptr);
				});

				IPropertyField* propertyDef = mValueProperties[i];
				propertyDef->SetValueAndPrototypeProxy(itemTargetValues);
			}
		}

		mIsRefreshing = false;
	}

	const Type* VectorProperty::GetValueType() const
	{
		return mVectorType;
	}

	void VectorProperty::SpecializeType(const Type* type)
	{
		mVectorType = nullptr;

		if (type->GetUsage() == Type::Usage::Vector)
			mVectorType = dynamic_cast<const VectorType*>(type);
		else if (type->GetUsage() == Type::Usage::Property) 
		{
			auto propertyType = dynamic_cast<const PropertyType*>(type);

			if (propertyType->GetValueType()->GetUsage() == Type::Usage::Vector)
				mVectorType = dynamic_cast<const VectorType*>(propertyType->GetValueType());
		}
	}

	void VectorProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);

		if (fieldInfo)
		{
			if (fieldInfo->HasAttribute<ExpandedByDefaultAttribute>())
				mSpoiler->Expand();
		}
	}

	const Type* VectorProperty::GetSpecializedType() const
	{
		return mVectorType;
	}

	void VectorProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);

		Text* spoilerCaptionLayer = mSpoiler->GetLayerDrawable<Text>("caption");
		if (spoilerCaptionLayer)
		{
			Vec2F captionSize = Text::GetTextSize(text, spoilerCaptionLayer->GetFont().Get(), spoilerCaptionLayer->GetHeight());
			*mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, captionSize.x + 20.0f, 0, 17, 0);
		}
	}

	WString VectorProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
	}

	Button* VectorProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			mRemoveBtn->layout->maxWidth = 20;
			mRemoveBtn->layout->minHeight = 20;
			mHeaderContainer->AddChild(mRemoveBtn, 0);
		}

		return mRemoveBtn;
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

		IPropertyField* res = o2EditorProperties.CreateFieldProperty(mVectorType->GetElementType(), "Element", 
																	 onChangeCompleted, onChanged);
		res->SetParentContext(mParentContext);

		res->AddLayer("drag", mnew Sprite("ui/UI4_drag_handle.png"), Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-18, 0)));

		if (res)
			res->SetFieldInfo(mFieldInfo);

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

	void VectorProperty::OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
										   const Vector<DataDocument>& after)
	{
		for (auto& pair : mTargetObjects)
			pair.first.SetValue();

		onChangeCompleted(path, before, after);
	}

	void VectorProperty::Resize(int newCount)
	{
		newCount = Math::Max(0, newCount);

		Vector<DataDocument> prevValues, newValues;
		auto elementFieldInfo = mVectorType->GetElementFieldInfo();

		for (auto& obj : mTargetObjects)
		{
			prevValues.Add(DataDocument());
			prevValues.Last()["Size"].Set(mVectorType->GetObjectVectorSize(obj.first.data));
			DataValue& elementsData = prevValues.Last()["Elements"];

			int lastCount = mVectorType->GetObjectVectorSize(obj.first.data);
			for (int i = newCount; i < lastCount; i++)
			{
				elementFieldInfo->Serialize(mVectorType->GetObjectVectorElementPtr(obj.first.data, i),
											elementsData.AddMember("Element" + (String)i));
			}

			newValues.Add(DataDocument());
			newValues.Last()["Size"].Set(newCount);

			mVectorType->SetObjectVectorSize(obj.first.data, newCount);
		}

		Refresh();

		if (prevValues != newValues)
			onChangeCompleted(mValuesPath + "/count", prevValues, newValues);

		onChanged(this);
		o2EditorSceneScreen.OnSceneChanged();
	}

	void VectorProperty::Remove(int idx)
	{
		Vector<DataDocument> prevValues, newValues;
		auto elementFieldInfo = mVectorType->GetElementFieldInfo();

		for (auto& obj : mTargetObjects)
		{
			prevValues.Add(DataDocument());
			mVectorType->Serialize(obj.first.data, prevValues.Last());

			mVectorType->RemoveObjectVectorElement(obj.first.data, idx);

			newValues.Add(DataDocument());
			mVectorType->Serialize(obj.first.data, newValues.Last());
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
