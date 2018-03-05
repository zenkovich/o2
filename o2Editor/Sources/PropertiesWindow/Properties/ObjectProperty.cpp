#include "stdafx.h"
#include "ObjectProperty.h"

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
	ObjectProperty::ObjectProperty()
	{
		mSpoiler = o2UI.CreateWidget<UISpoiler>("expand with caption");
		mSpoiler->onExpand = THIS_FUNC(OnExpand);
	}

	ObjectProperty::~ObjectProperty()
	{
		delete mSpoiler;
	}

	void ObjectProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mTargetObjects.Clear();
			return;
		}

		mTargetObjects = targets;
		Refresh();
	}

	void ObjectProperty::Refresh()
	{
		mFieldProperties.Set(mTargetObjects.Select<Pair<IObject*, IObject*>>([](const Pair<void*, void*>& x) {
			return Pair<IObject*, IObject*>((IObject*)(x.first), 
											x.second ? (IObject*)(x.second) : nullptr);
		}));
	}

	UIWidget* ObjectProperty::GetWidget() const
	{
		return mSpoiler;
	}

	const Type* ObjectProperty::GetFieldType() const
	{
		return &TypeOf(IObject);
	}

	void ObjectProperty::SpecializeType(const Type* type)
	{
		mObjectType = type;
	}

	void ObjectProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);
	}

	WString ObjectProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
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

	const FieldPropertiesInfo& ObjectProperty::GetPropertiesInfo() const
	{
		return mFieldProperties;
	}

	void ObjectProperty::OnExpand()
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

DECLARE_CLASS(Editor::ObjectProperty);
