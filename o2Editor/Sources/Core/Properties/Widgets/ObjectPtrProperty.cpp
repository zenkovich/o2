#include "stdafx.h"
#include "ObjectPtrProperty.h"

#include "Core/Properties/Properties.h"
#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/UIManager.h"
#include "UI/ContextMenu.h"

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
			mSpoiler->onExpand = THIS_FUNC(Refresh);

		auto typeContainer = mnew UIWidget();
		typeContainer->layout->minHeight = 20;
		mSpoiler->AddChild(typeContainer);

		mTypeCaption = o2UI.CreateLabel("nullptr");
		*mTypeCaption->layout = UIWidgetLayout(0, 1, 0.7f, 0, 0, 0, 0, 0);
		typeContainer->AddChild(mTypeCaption);

		mCreateDeleteButton = o2UI.CreateButton("Create");
		*mCreateDeleteButton->layout = UIWidgetLayout(0.7f, 1, 1, 0, 0, 0, 0, 0);
		mCreateDeleteButton->onClick = THIS_FUNC(OnCreateOrDeletePressed);
		typeContainer->AddChild(mCreateDeleteButton);

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
		if (!mSpoiler->IsExpanded())
			return;

		const Type* objectPtrType = nullptr;
		if (!mTargetObjects.IsEmpty())
		{
			auto object = GetProxy<IObject*>(mTargetObjects[0].first);
			if (object)
				objectPtrType = object->GetType().GetPointerType();
		}

		if (objectPtrType != mObjectPtrType)
		{
			if (!mTargetObjects.IsEmpty())
			{
				auto object = GetProxy<IObject*>(mTargetObjects[0].first);

				if (object)
				{
					mTypeCaption->text = object->GetType().GetName();
					mCreateDeleteButton->caption = "Delete";
				}
				else
				{
					mTypeCaption->text = "nullptr";
					mCreateDeleteButton->caption = "Create";
				}
			}

			mObjectPtrType = objectPtrType;

			o2EditorProperties.FreeProperties(mFieldProperties);

			if (mObjectPtrType)
			{
				mObjectType = ((PointerType*)mObjectPtrType)->GetUnpointedType();

				auto onChangeCompletedFunc =
					[&](const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
				{
					onChangeCompleted(mValuesPath + "/" + path, before, after);
				};

				o2EditorProperties.BuildObjectProperties(mSpoiler, mObjectType, mFieldProperties, "", onChangeCompleted, onChanged);
			}

			mPropertiesInitialized = true;
		}

		if (mObjectPtrType)
		{
			mFieldProperties.Set(mTargetObjects.Select<Pair<IObject*, IObject*>>(
				[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
			{
				return Pair<IObject*, IObject*>(GetProxy<IObject*>(x.first),
												x.second ? GetProxy<IObject*>(x.second) : nullptr);
			}));
		}
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
		mContextInitialized = false;
	}

	const Type* ObjectPtrProperty::GetSpecializedType() const
	{
		return mObjectPtrType;
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

	void ObjectPtrProperty::OnCreateOrDeletePressed()
	{
		bool hasObject = !mTargetObjects.IsEmpty() && GetProxy<IObject*>(mTargetObjects[0].first) != nullptr;
		if (hasObject)
		{
			for (auto targetObj : mTargetObjects)
			{
				IObject* object = GetProxy<IObject*>(targetObj.first);

				if (object != nullptr)
				{
					delete object;
					SetProxy<IObject*>(targetObj.first, nullptr);
				}
			}

			Refresh();
			mSpoiler->SetLayoutDirty();
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
					return UIContextMenu::Item(type->GetName(), [=]() { CreateObject(type); });
				}));

				mContextInitialized = true;
			}

			if (mImmediateCreateObject)
				CreateObject(mObjectType);
			else
				mCreateMenu->Show();
		}
	}

	void ObjectPtrProperty::CreateObject(const Type* type)
	{
		for (auto targetObj : mTargetObjects)
		{
			if (GetProxy<IObject*>(targetObj.first) == nullptr)
				SetProxy<IObject*>(targetObj.first, (IObject*)type->CreateSample());
		}

		Refresh();
		mSpoiler->SetLayoutDirty();
	}
}

DECLARE_CLASS(Editor::ObjectPtrProperty);
