#include "stdafx.h"
#include "ObjectProperty.h"

#include "Core/EditorScope.h"
#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/Spoiler.h"

using namespace o2;

namespace Editor
{
	ObjectProperty::ObjectProperty()
	{
		InitializeControls();
	}

	ObjectProperty::ObjectProperty(const ObjectProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	ObjectProperty& ObjectProperty::operator=(const ObjectProperty& other)
	{
		IPropertyField::operator=(other);
		return *this;
	}

	void ObjectProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void ObjectProperty::InitializeControls()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = FindChildByType<Spoiler>(false);
		if (!mSpoiler)
		{
			mSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");
			AddChild(mSpoiler);
		}

		if (mSpoiler)
			mSpoiler->onExpand = THIS_FUNC(OnExpand);

		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	void ObjectProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		for (auto& pair : mTargetObjects)
		{
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

	void ObjectProperty::Refresh()
	{
		PushEditorScopeOnStack scope;

		if (mSpoiler->IsExpanded())
		{
			if (mObjectPropertiesViewer)
			{
				for (auto& pair : mTargetObjects)
				{
					pair.first.Refresh();
					pair.second.Refresh();
				}

				mObjectPropertiesViewer->Refresh(mTargetObjects.Select<Pair<IObject*, IObject*>>(
					[&](const Pair<TargetObjectData, TargetObjectData>& x)
				{
					return Pair<IObject*, IObject*>(x.first.data, x.second.data);
				}));
			}
		}
	}

	const Type* ObjectProperty::GetFieldType() const
	{
		return &TypeOf(IObject);
	}

	void ObjectProperty::SpecializeType(const Type* type)
	{
		if (type->GetUsage() == Type::Usage::Property)
			mObjectType = dynamic_cast<const PropertyType*>(type)->GetValueType();
		else
			mObjectType = type;
	}

	void ObjectProperty::SpecializeFieldInfo(const FieldInfo* fieldInfo)
	{
		SpecializeType(fieldInfo->GetType());

		if (fieldInfo->GetAttribute<ExpandedByDefaultAttribute>())
			mSpoiler->Expand();
	}

	const Type* ObjectProperty::GetSpecializedType() const
	{
		return mObjectType;
	}

	void ObjectProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);
	}

	WString ObjectProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
	}

	Button* ObjectProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			*mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, 0));
			AddInternalWidget(mRemoveBtn);
		}

		return mRemoveBtn;
	}

	void ObjectProperty::Expand()
	{
		SetExpanded(true);
	}

	void ObjectProperty::Collapse()
	{
		SetExpanded(false);
	}

	void ObjectProperty::SetExpanded(bool expanded)
	{
		mSpoiler->SetExpanded(expanded);
	}

	bool ObjectProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	void ObjectProperty::OnExpand()
	{
		PushEditorScopeOnStack scope;

		if (mPropertiesInitialized)
			return;

		if (!mObjectPropertiesViewer)
		{
			mObjectPropertiesViewer = o2EditorProperties.CreateObjectViewer(mObjectType, mValuesPath, THIS_FUNC(OnPropertyChanged), onChanged);
			mSpoiler->AddChild(mObjectPropertiesViewer->GetLayout());
		}

		Refresh();

		mPropertiesInitialized = true;
	}

	ObjectProperty::TargetObjectData ObjectProperty::GetObjectFromProxy(IAbstractValueProxy* proxy)
	{
		TargetObjectData res;

		if (!proxy)
			return res;

		res.proxy = proxy;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());
		if (auto pointerProxy = dynamic_cast<IPointerValueProxy*>(proxy))
		{
			res.data = objectType.DynamicCastToIObject(pointerProxy->GetValueVoidPointer());
			res.isCreated = false;
		}
		else
		{
			void* sample = objectType.CreateSample();
			proxy->GetValuePtr(sample);

			res.data = objectType.DynamicCastToIObject(sample);
			res.isCreated = true;
		}

		return res;
	}

	void ObjectProperty::OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		for (auto& pair : mTargetObjects)
			pair.first.SetValue();

		onChangeCompleted(path, before, after);
	}

	void ObjectProperty::TargetObjectData::Refresh()
	{
		if (!isCreated)
			return;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());
		proxy->GetValuePtr(objectType.DynamicCastFromIObject(data));
	}

	void ObjectProperty::TargetObjectData::SetValue()
	{
		if (!isCreated)
			return;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(proxy->GetType());
		proxy->SetValuePtr(objectType.DynamicCastFromIObject(data));
	}

}

DECLARE_CLASS(Editor::ObjectProperty);
