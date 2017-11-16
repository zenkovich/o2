#include "ComponentProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/Assets.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Scene/Actor.h"
#include "Scene/Component.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	ComponentProperty::ComponentProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("component property");

		mBox = mPropertyWidget->GetChildWidget("box");
		if (!mBox)
			mBox = mPropertyWidget;

		mBox->onDraw += [&]() { OnDrawn(); };
		mBox->SetFocusable(true);

		mNameText = mBox->GetLayerDrawable<Text>("caption");
		mNameText->text = "--";

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);

		mComponentType = &TypeOf(Component);
	}

	ComponentProperty::~ComponentProperty()
	{
		delete mPropertyWidget;
	}

	void ComponentProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Component* value) { *((Property<Component*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Component*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Component* value) { *((Component**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Component**)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void ComponentProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue();
		}
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	void ComponentProperty::Revert()
	{
		auto propertyObjects = o2EditorProperties.GetTargets();

		for (int i = 0; i < mValuesPointers.Count() && i < propertyObjects.Count(); i++)
			RevertoToPrototype(mValuesPointers[i].first, mValuesPointers[i].second, propertyObjects[i]);

		Refresh();
	}

	UIWidget* ComponentProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Component* ComponentProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ComponentProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* ComponentProperty::GetFieldType() const
	{
		return &TypeOf(Component*);
	}

	void ComponentProperty::SpecializeType(const Type* type)
	{
		if (type->GetUsage() == Type::Usage::Pointer)
			mComponentType = ((PointerType*)type)->GetUnpointedType();
		else
			mComponentType = type;
	}

	const Type* ComponentProperty::GetSpecializedType() const
	{
		return mComponentType;
	}

	void ComponentProperty::SetValue(Component* value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void ComponentProperty::SetUnknownValue()
	{
		mCommonValue = nullptr;
		mValuesDifferent = true;
		mNameText->text = "--";
		mBox->layer["caption"]->transparency = 1.0f;

		OnChanged();
	}

	bool ComponentProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	void ComponentProperty::RevertoToPrototype(void* target, void* source, IObject* targetOwner)
	{
		if (!source || !targetOwner || targetOwner->GetType().IsBasedOn(TypeOf(Component)))
			return;

		Component* sourceComponent = mGetFunc(source);
		Actor* topSourceActor = sourceComponent->GetOwnerActor();
		while (topSourceActor->GetParent())
			topSourceActor = topSourceActor->GetParent();

		Actor* actorOwner = dynamic_cast<Actor*>(targetOwner);

		if (actorOwner)
		{
			Actor* topTargetActor = actorOwner;
			while (topTargetActor->GetPrototypeLink() != topSourceActor && topTargetActor->GetParent())
				topTargetActor = topTargetActor->GetParent();

			Actor* sameToProtoSourceActor = topTargetActor->FindLinkedActor(sourceComponent->GetOwnerActor());

			if (sameToProtoSourceActor)
			{
				Component* sameToProtoSourceComponent = sameToProtoSourceActor->GetComponents().FindMatch(
					[&](Component* x) { return x->GetPrototypeLink() == sourceComponent; });

				if (sameToProtoSourceComponent)
				{
					mAssignFunc(target, sameToProtoSourceComponent);
					return;
				}
			}
		}

		if (sourceComponent->GetOwnerActor()->IsOnScene())
			mAssignFunc(target, sourceComponent);
	}

	void ComponentProperty::SetCommonValue(Component* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		if (!mCommonValue)
		{
			mNameText->text = "Null:" + mComponentType->GetName();
			mBox->layer["caption"]->transparency = 0.5f;
		}
		else
		{
			mNameText->text = mCommonValue->GetOwnerActor()->GetName();
			mBox->layer["caption"]->transparency = 1.0f;
		}

		OnChanged();
	}

	void ComponentProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				Component* value = mGetFunc(ptr.first);
				Component* proto = mGetFunc(ptr.second);

				if (value && value->GetPrototypeLink())
				{
					if (value->GetPrototypeLink() != proto)
					{
						revertable = true;
						break;
					}
				}
				else
				{
					if (value != proto)
					{
						revertable = true;
						break;
					}
				}
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void ComponentProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("hover", true);
	}

	void ComponentProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("hover", false);
	}

	void ComponentProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);

		if (mCommonValue)
		{
			auto ownerActor = mCommonValue->GetOwnerActor();

			if (ownerActor)
			{
				if (ownerActor->IsAsset())
					o2EditorAssets.ShowAssetIcon(o2Assets.GetAssetPath(ownerActor->GetAssetID()));
				else if (ownerActor->IsOnScene())
					o2EditorTree.HightlightActorsTreeNode(ownerActor);
			}
		}
	}

	void ComponentProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox->IsFocused() && key == VK_DELETE || key == VK_BACK)
			SetValueByUser(nullptr);
	}

	void ComponentProperty::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDroppedFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDroppedFromAssetsScroll(assetsScroll);
	}

	void ComponentProperty::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDragEnterFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDragEnterFromAssetsScroll(assetsScroll);
	}

	void ComponentProperty::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDragExitFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDragExitFromAssetsScroll(assetsScroll);
	}

	void ComponentProperty::OnDroppedFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		SetValueByUser(actorsTree->GetSelectedActors()[0]->GetComponent(mComponentType));

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ComponentProperty::OnDragEnterFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1 &&
			actorsTree->GetSelectedActors()[0]->GetComponent(mComponentType))
		{
			return;
		}

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void ComponentProperty::OnDragExitFromActorsTree(UIActorsTree* actorsTree)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ComponentProperty::OnDroppedFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto actor = o2Scene.GetAssetActorByID(assetsIconsScroll->GetSelectedAssets().Last().id);
		SetValueByUser(actor->GetComponent(mComponentType));

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ComponentProperty::OnDragEnterFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto actor = o2Scene.GetAssetActorByID(assetsIconsScroll->GetSelectedAssets().Last().id);
		if (actor)
		{
			auto component = actor->GetComponent(mComponentType);

			if (!component)
				return;

			o2Application.SetCursor(CursorType::Hand);
			mBox->SetState("focused", true);
		}
	}

	void ComponentProperty::OnDragExitFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ComponentProperty::SetValueByUser(Component* value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void ComponentProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void ComponentProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::ComponentProperty);
