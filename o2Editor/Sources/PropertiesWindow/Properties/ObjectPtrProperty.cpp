#include "stdafx.h"
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
		mSpoiler = o2UI.CreateWidget<UISpoiler>("expand with caption");
		mSpoiler->onExpand = THIS_FUNC(OnExpand);
	}

	ObjectPtrProperty::~ObjectPtrProperty()
	{
		delete mSpoiler;
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
		return mSpoiler;
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

		o2EditorProperties.BuildObjectProperties(mSpoiler, mObjectType, mFieldProperties, "");

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

		mPropertiesInitialized = true;
	}

}

DECLARE_CLASS(Editor::ObjectPtrProperty);
