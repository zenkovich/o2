#include "stdafx.h"
#include "ObjectProperty.h"

#include "Core/Properties/Properties.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "Core/Properties/IObjectPropertiesViewer.h"

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
		CopyData(other);
		return *this;
	}

	void ObjectProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void ObjectProperty::InitializeControls()
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

	void ObjectProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mTargetObjects = targets;
		Refresh();
	}

	void ObjectProperty::Refresh()
	{
		if (mSpoiler->IsExpanded())
		{
			if (mObjectPropertiesViewer)
				mObjectPropertiesViewer->Refresh(mTargetObjects);
		}
	}

	const Type* ObjectProperty::GetFieldType() const
	{
		return &TypeOf(IObject);
	}

	void ObjectProperty::SpecializeType(const Type* type)
	{
		mObjectType = type;
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

		auto onChangeCompletedFunc =
			[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
		{
			onChangeCompleted(mValuesPath + "/" + path, before, after);
		};

		if (!mObjectPropertiesViewer)
		{
			mObjectPropertiesViewer = o2EditorProperties.CreateObjectViewer(mObjectType);
			mObjectPropertiesViewer->InitializeControls(mValuesPath, onChangeCompletedFunc, onChanged);
			mSpoiler->AddChild(mObjectPropertiesViewer->GetViewWidget());
		}

		mObjectPropertiesViewer->Refresh(mTargetObjects);

		mPropertiesInitialized = true;
	}

	IObject* ObjectProperty::GetProxyPtr(IAbstractValueProxy* proxy) const
	{
		IIObjectPointerValueProxy* objProxy = dynamic_cast<IIObjectPointerValueProxy*>(proxy);

		if (objProxy)
			return objProxy->GetObjectPtr();

		return nullptr;
	}

}

DECLARE_CLASS(Editor::ObjectProperty);
