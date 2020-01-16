#include "o2Editor/stdafx.h"
#include "ObjectProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/NoHeaderAttribute.h"
#include "o2Editor/Core/EditorScope.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"

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

	void ObjectProperty::OnFreeProperty()
	{
		if (mObjectViewer)
			o2EditorProperties.FreeObjectViewer(mObjectViewer);

		mObjectViewer = nullptr;
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
			mSpoiler->onExpand = THIS_FUNC(Refresh);

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

		if (!mSpoiler->IsExpanded())
			return;

		for (auto& pair : mTargetObjects)
		{
			pair.first.Refresh();
			pair.second.Refresh();
		}

		CheckViewer();

		if (mObjectViewer)
		{
			mObjectViewer->Refresh(mTargetObjects.Select<Pair<IObject*, IObject*>>(
				[&](const Pair<TargetObjectData, TargetObjectData>& x)
			{
				return Pair<IObject*, IObject*>(x.first.data, x.second.data);
			}));
		}
	}

	void ObjectProperty::CheckViewer()
	{
		PushEditorScopeOnStack scope;

		const Type* objectType = nullptr;
		if (!mTargetObjects.IsEmpty())
		{
			auto object = mTargetObjects[0].first.data;
			if (object)
				objectType = &object->GetType();
		}

		const Type* prevObjectType = mObjectViewer ? mObjectViewer->GetViewingObjectType() : nullptr;
		if (objectType != prevObjectType)
		{
			if (mObjectViewer)
				o2EditorProperties.FreeObjectViewer(mObjectViewer);

			if (objectType)
			{
				mObjectViewer = o2EditorProperties.CreateObjectViewer(objectType, mValuesPath, onChangeCompleted, onChanged);
				mSpoiler->AddChild(mObjectViewer->GetLayout());
			}
		}
	}

	const Type* ObjectProperty::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* ObjectProperty::GetValueTypeStatic()
	{
		return &TypeOf(IObject);
	}

	void ObjectProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		if (fieldInfo->HasAttribute<ExpandedByDefaultAttribute>())
			mSpoiler->Expand();

		if (fieldInfo->HasAttribute<NoHeaderAttribute>())
		{
			mSpoiler->SetHeadHeight(0);
			mSpoiler->GetLayerDrawable<Text>("caption")->enabled = false;
			mSpoiler->GetInternalWidget("expand")->enabledForcibly = false;
			mSpoiler->borderLeft = 0;
			mSpoiler->borderTop = 0;
			mSpoiler->Expand();
		}
		else
		{
			mSpoiler->SetHeadHeight(18);
			mSpoiler->GetLayerDrawable<Text>("caption")->enabled = true;
			mSpoiler->GetInternalWidget("expand")->enabledForcibly = true;
			mSpoiler->borderLeft = 10;
			mSpoiler->borderTop = 5;
		}
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
