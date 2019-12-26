#include "stdafx.h"
#include "ObjectPtrProperty.h"

#include "Core/EditorScope.h"
#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Utils/Editor/Attributes/DefaultTypeAttribute.h"
#include "Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "Utils/Editor/Attributes/NoHeaderAttribute.h"

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

	void ObjectPtrProperty::OnFreeProperty()
	{
		if (mObjectViewer)
			o2EditorProperties.FreeObjectViewer(mObjectViewer);

		mObjectViewer = nullptr;
	}

	void ObjectPtrProperty::CopyData(const Actor& otherActor)
	{
		IPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void ObjectPtrProperty::InitializeControls()
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

		mHeaderContainer = mnew HorizontalLayout();
		*mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, 100, 0, 17, 0);
		mHeaderContainer->baseCorner = BaseCorner::Right;
		mHeaderContainer->expandHeight = false;
		mHeaderContainer->SetInternalParent(mSpoiler, false);

		mTypeCaption = o2UI.CreateLabel("nullptr");
		*mTypeCaption->layout = WidgetLayout(0, 1, 1, 0, 0, 0, 75, 1);
		mTypeCaption->horOverflow = Label::HorOverflow::Dots;
		mTypeCaption->horAlign = HorAlign::Left;
		mTypeCaption->verAlign = VerAlign::Bottom;
		mTypeCaption->height = 8;
		mTypeCaption->transparency = 0.6f;
		mHeaderContainer->AddInternalWidget(mTypeCaption);

		mCreateDeleteButton = o2UI.CreateButton("Create");
		mCreateDeleteButton->layout->maxWidth = 75;
		mCreateDeleteButton->layout->minHeight = 15;
		mCreateDeleteButton->layout->height = 15;
		mCreateDeleteButton->onClick = THIS_FUNC(OnCreateOrDeletePressed);
		mHeaderContainer->AddChild(mCreateDeleteButton);

		mCreateMenu = o2UI.CreateWidget<ContextMenu>();
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
		PushEditorScopeOnStack scope;

		if (!mTargetObjects.IsEmpty())
		{
			auto object = GetProxy(mTargetObjects[0].first);

			if (!object && mFieldInfo)
			{
				if (auto defaultTypeAttr = mFieldInfo->GetAttribute<DefaultTypeAttribute>())
				{
					CreateObject(dynamic_cast<const ObjectType*>(defaultTypeAttr->defaultType));
					return;
				}
			}

			if (object)
			{				
				mTypeCaption->text = object->GetType().GetName();
				mCreateDeleteButton->caption = "Delete";
				mCreateDeleteButton->enabledForcibly = !mDontDeleteEnabled;
			}
			else
			{
				mTypeCaption->text = "nullptr";
				mCreateDeleteButton->caption = "Create";
				mCreateDeleteButton->enabledForcibly = true;
			}
		}

		if (!mSpoiler->IsExpanded())
			return;

		CheckViewer();

		if (mObjectViewer)
		{
			mObjectViewer->Refresh(mTargetObjects.Select<Pair<IObject*, IObject*>>(
				[&](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
			{
				return Pair<IObject*, IObject*>(GetProxy(x.first),
												x.second ? GetProxy(x.second) : nullptr);
			}));
		}
	}

	void ObjectPtrProperty::CheckViewer()
	{
		mCurrentObjectType = nullptr;
		if (!mTargetObjects.IsEmpty())
		{
			auto object = GetProxy(mTargetObjects[0].first);
			if (object)
				mCurrentObjectType = dynamic_cast<const ObjectType*>(&object->GetType());
		}

		const Type* prevObjectType = mObjectViewer ? mObjectViewer->GetViewingObjectType() : nullptr;
		if (mCurrentObjectType != prevObjectType)
		{
			if (mObjectViewer)
				o2EditorProperties.FreeObjectViewer(mObjectViewer);

			if (mCurrentObjectType)
			{
				mObjectViewer = o2EditorProperties.CreateObjectViewer(mCurrentObjectType, mValuesPath, onChangeCompleted, onChanged);
				mSpoiler->AddChild(mObjectViewer->GetLayout());
			}
		}
	}

	const Type* ObjectPtrProperty::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* ObjectPtrProperty::GetValueTypeStatic()
	{
		return &TypeOf(IObject*);
	}

	void ObjectPtrProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);

		if (fieldInfo)
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

			mDontDeleteEnabled = fieldInfo->HasAttribute<DontDeleteAttribute>();
		}
	}

	void ObjectPtrProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);

		Text* spoilerCaptionLayer = mSpoiler->GetLayerDrawable<Text>("caption");
		if (spoilerCaptionLayer)
		{
			Vec2F captionSize = Text::GetTextSize(text, spoilerCaptionLayer->GetFont().Get(), spoilerCaptionLayer->GetHeight());
			*mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, captionSize.x + 20.0f, 0, 17, 0);
		}
	}

	WString ObjectPtrProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
	}

	Button* ObjectPtrProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			mRemoveBtn->layout->maxWidth = 20;
			mHeaderContainer->AddChild(mRemoveBtn, 0);
		}

		return mRemoveBtn;
	}

	void ObjectPtrProperty::SetBasicType(const ObjectType* type)
	{
		mBasicObjectType = type;
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
		PushEditorScopeOnStack scope;

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

				auto availableTypes = mBasicObjectType->GetDerivedTypes();
				availableTypes.Insert(mBasicObjectType, 0);

				mImmediateCreateObject = availableTypes.Count() == 1;

				mCreateMenu->AddItems(availableTypes.Select<ContextMenu::Item>([&](const Type* type)
				{
					return ContextMenu::Item(type->GetName(), [=]() { CreateObject(dynamic_cast<const ObjectType*>(type)); });
				}));

				mContextInitialized = true;
			}

			if (mImmediateCreateObject)
				CreateObject(mBasicObjectType);
			else
				mCreateMenu->Show();
		}
	}

	void ObjectPtrProperty::CreateObject(const ObjectType* type)
	{
		PushEditorScopeOnStack scope;

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
