#include "stdafx.h"
#include "ObjectPtrProperty.h"

#include "Core/Properties/Properties.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"

using namespace o2;

namespace Editor
{
	ObjectPtrProperty::ObjectPtrProperty()
	{
		InitializeControls();
	}

	ObjectPtrProperty::ObjectPtrProperty(const ObjectPtrProperty& other) :
		IPropertyField(other)
	{
		InitializeControls();
	}

	ObjectPtrProperty& ObjectPtrProperty::operator=(const ObjectPtrProperty& other)
	{
		CopyData(other);
		return *this;
	}

	void ObjectPtrProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void ObjectPtrProperty::InitializeControls()
	{
		mSpoiler = FindChildByType<UISpoiler>(false);
		if (!mSpoiler)
		{
			mSpoiler = o2UI.CreateWidget<UISpoiler>("expand with caption");
			AddChild(mSpoiler);
		}

		if (mSpoiler)
			mSpoiler->onExpand = THIS_FUNC(OnExpand);

		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	void ObjectPtrProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mTargetObjects = targets;
		Refresh();
	}

	void ObjectPtrProperty::Refresh()
	{
		for (auto targetObj : mTargetObjects)
		{
			if (GetProxy<IObject*>(targetObj.first) == nullptr)
				SetProxy<IObject*>(targetObj.first, (IObject*)mObjectType->CreateSample());
		}

		mFieldProperties.Set(mTargetObjects.Select<Pair<IObject*, IObject*>>(
			[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
		{
			return Pair<IObject*, IObject*>(GetProxy<IObject*>(x.first),
											x.second ? GetProxy<IObject*>(x.second) : nullptr);
		}));
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
	}

	void ObjectPtrProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);
	}

	WString ObjectPtrProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
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
		mSpoiler->SetExpanded(expanded);
	}

	bool ObjectPtrProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	const FieldPropertiesInfo& ObjectPtrProperty::GetPropertiesInfo() const
	{
		return mFieldProperties;
	}

	void ObjectPtrProperty::OnExpand()
	{
		if (mPropertiesInitialized)
			return;

		auto onChangeCompletedFunc =
			[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		o2EditorProperties.BuildObjectProperties(mSpoiler, mObjectType, mFieldProperties, "", onChangeCompleted, onChanged);

		mPropertiesInitialized = true;
	}

}

DECLARE_CLASS(Editor::ObjectPtrProperty);
