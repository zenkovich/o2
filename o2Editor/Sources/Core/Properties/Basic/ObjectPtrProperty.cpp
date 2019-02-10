#include "stdafx.h"
#include "ObjectPtrProperty.h"

#include "Core/EditorScope.h"
#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/UIManager.h"

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
		IPropertyField::operator=(other);
		return *this;
	}

	void ObjectPtrProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void ObjectPtrProperty::InitializeControls()
	{
		PushScopeEnterOnStack scope;

		mSpoiler = FindChildByType<Spoiler>(false);
		if (!mSpoiler)
		{
			mSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");
			AddChild(mSpoiler);
		}

		if (mSpoiler)
			mSpoiler->onExpand = THIS_FUNC(Refresh);

		mTypeContainer = mnew Widget();
		*mTypeContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, 100, 0, 17, 0);
		mTypeContainer->SetInternalParent(mSpoiler, false);

		mTypeCaption = o2UI.CreateLabel("nullptr");
		*mTypeCaption->layout = WidgetLayout(0, 1, 1, 0, 0, 0, 75, 1);
		mTypeCaption->horOverflow = Label::HorOverflow::Dots;
		mTypeCaption->horAlign = HorAlign::Left;
		mTypeCaption->verAlign = VerAlign::Bottom;
		mTypeCaption->height = 8;
		mTypeCaption->transparency = 0.6f;
		mTypeContainer->AddChild(mTypeCaption);

		mCreateDeleteButton = o2UI.CreateButton("Create");
		*mCreateDeleteButton->layout = WidgetLayout(1, 1, 1, 0, -75, 0, 0, -3);
		mCreateDeleteButton->onClick = THIS_FUNC(OnCreateOrDeletePressed);
		mTypeContainer->AddChild(mCreateDeleteButton);

		mCreateMenu = o2UI.CreateWidget<UIContextMenu>();
		mCreateDeleteButton->AddChild(mCreateMenu);

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
		PushScopeEnterOnStack scope;

		if (!mTargetObjects.IsEmpty())
		{
			auto object = GetProxy(mTargetObjects[0].first);

			if (object)
			{
				auto tp = &object->GetType();

				if (tp == nullptr)
					tp = &object->GetType();

				mTypeCaption->text = object->GetType().GetName();
				mCreateDeleteButton->caption = "Delete";
			}
			else
			{
				mTypeCaption->text = "nullptr";
				mCreateDeleteButton->caption = "Create";
			}
		}

		if (!mSpoiler->IsExpanded())
			return;

		const Type* objectPtrType = nullptr;
		if (!mTargetObjects.IsEmpty())
		{
			auto object = GetProxy(mTargetObjects[0].first);
			if (object)
				objectPtrType = object->GetType().GetPointerType();
		}

		if (objectPtrType != mObjectPtrType)
		{
			mObjectPtrType = objectPtrType;

			if (mObjectPropertiesViewer)
				o2EditorProperties.FreeObjectViewer(mObjectPropertiesViewer, mObjectPtrType);

			if (mObjectPtrType)
			{
				mObjectType = dynamic_cast<const ObjectType*>(dynamic_cast<const PointerType*>(mObjectPtrType)->GetUnpointedType());
				mObjectPropertiesViewer = o2EditorProperties.CreateObjectViewer(mObjectType, mValuesPath, onChangeCompleted, onChanged);
				mSpoiler->AddChild(mObjectPropertiesViewer->GetViewWidget());
			}

			mPropertiesInitialized = true;
		}

		if (mObjectPtrType && mObjectPropertiesViewer)
		{
			mObjectPropertiesViewer->Refresh(mTargetObjects.Select<Pair<IObject*, IObject*>>(
				[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
			{
				return Pair<IObject*, IObject*>(GetProxy(x.first),
												x.second ? GetProxy(x.second) : nullptr);
			}));
		}
	}

	const Type* ObjectPtrProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void ObjectPtrProperty::SpecializeType(const Type* type)
	{
		if (type->GetUsage() == Type::Usage::Property)
			type = dynamic_cast<const PropertyType*>(type)->GetValueType();

		if (type->GetUsage() != Type::Usage::Pointer)
			return;

		mObjectType = dynamic_cast<const ObjectType*>(dynamic_cast<const PointerType*>(type)->GetUnpointedType());
		mContextInitialized = false;
	}

	void ObjectPtrProperty::SpecializeFieldInfo(const FieldInfo* fieldInfo)
	{
		SpecializeType(fieldInfo->GetType());

		if (fieldInfo->GetAttribute<ExpandedByDefaultAttribute>())
			mSpoiler->Expand();
	}

	const Type* ObjectPtrProperty::GetSpecializedType() const
	{
		return mObjectPtrType;
	}

	void ObjectPtrProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);

		Text* spoilerCaptionLayer = mSpoiler->GetLayerDrawable<Text>("caption");
		if (spoilerCaptionLayer)
		{
			Vec2F captionSize = Text::GetTextSize(text, spoilerCaptionLayer->GetFont().Get(), spoilerCaptionLayer->GetHeight());
			*mTypeContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, captionSize.x + 20.0f, 0, 17, 0);
		}
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

	void ObjectPtrProperty::OnCreateOrDeletePressed()
	{
		PushScopeEnterOnStack scope;

		bool hasObject = !mTargetObjects.IsEmpty() && GetProxy(mTargetObjects[0].first) != nullptr;
		if (hasObject)
		{
			for (auto targetObj : mTargetObjects)
			{
				IObject* object = GetProxy(targetObj.first);

				if (object != nullptr)
				{
					delete object;
					SetProxy(targetObj.first, nullptr);
				}
			}

			Refresh();
			mSpoiler->SetLayoutDirty();
			mSpoiler->Collapse();
		}
		else
		{
			if (!mContextInitialized)
			{
				mCreateMenu->RemoveAllItems();

				auto availableTypes = mObjectType->GetDerivedTypes();
				availableTypes.Insert(mObjectType, 0);

				mImmediateCreateObject = availableTypes.Count() == 1;

				mCreateMenu->AddItems(availableTypes.Select<UIContextMenu::Item>([&](const Type* type)
				{
					return UIContextMenu::Item(type->GetName(), [=]() { CreateObject(dynamic_cast<const ObjectType*>(type)); });
				}));

				mContextInitialized = true;
			}

			if (mImmediateCreateObject)
				CreateObject(mObjectType);
			else
				mCreateMenu->Show();
		}
	}

	void ObjectPtrProperty::CreateObject(const ObjectType* type)
	{
		PushScopeEnterOnStack scope;

		StoreValues(mBeforeChangeValues);
		for (auto targetObj : mTargetObjects)
		{
			if (GetProxy(targetObj.first) == nullptr)
				SetProxy(targetObj.first, type->DynamicCastToIObject(type->CreateSample()));
		}

		CheckValueChangeCompleted();

		Refresh();
		mSpoiler->SetLayoutDirty();
	}

	void ObjectPtrProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto targetObj : mTargetObjects)
		{
			data.Add(DataNode());
			data.Last() = GetProxy(targetObj.first);
		}
	}

	IObject* ObjectPtrProperty::GetProxy(IAbstractValueProxy* proxy) const
	{
		const Type& proxyType = proxy->GetType();
		if (proxyType.GetUsage() != Type::Usage::Pointer)
			return nullptr;

		const Type& unptrType = *dynamic_cast<const PointerType&>(proxyType).GetUnpointedType();
		if (!unptrType.IsBasedOn(TypeOf(IObject)))
			return nullptr;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(unptrType);

		void* valuePtr;
		proxy->GetValuePtr(&valuePtr);

		return objectType.DynamicCastToIObject(valuePtr);
	}

	void ObjectPtrProperty::SetProxy(IAbstractValueProxy* proxy, IObject* object)
	{
		const Type& proxyType = proxy->GetType();
		if (proxyType.GetUsage() != Type::Usage::Pointer)
			return;

		const Type& unptrType = *dynamic_cast<const PointerType&>(proxyType).GetUnpointedType();
		if (!unptrType.IsBasedOn(TypeOf(IObject)))
			return;

		const ObjectType& objectType = dynamic_cast<const ObjectType&>(unptrType);

		void* valuePtr = objectType.DynamicCastFromIObject(object);
		proxy->SetValuePtr(&valuePtr);
	}

}

DECLARE_CLASS(Editor::ObjectPtrProperty);
