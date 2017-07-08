#include "ActorProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/ActorAsset.h"
#include "Assets/Assets.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"

namespace Editor
{
	ActorProperty::ActorProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("actor property");

		mBox = mPropertyWidget->GetChild("box");
		if (!mBox)
			mBox = mPropertyWidget;

		mBox->onDraw += [&]() { OnDrawn(); };
		mBox->SetFocusable(true);

		mNameText = mBox->GetLayerDrawable<Text>("caption");
		mNameText->text = "--";

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = Function<void()>(this, &ActorProperty::Revert);
	}

	ActorProperty::~ActorProperty()
	{
		delete mPropertyWidget;
	}

	void ActorProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Actor* value) { *((Property<Actor*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Actor*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Actor* value) { *((Actor**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Actor**)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void ActorProperty::Refresh()
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
	}

	void ActorProperty::Revert()
	{
		auto propertyObjects = o2EditorProperties.GetTargets();

		for (int i = 0; i < mValuesPointers.Count() && i < propertyObjects.Count(); i++)
			RevertoToPrototype(mValuesPointers[i].first, mValuesPointers[i].second, propertyObjects[i]);

		Refresh();
	}

	void ActorProperty::RevertoToPrototype(void* target, void* source, IObject* targetOwner)
	{
		if (!source || !targetOwner || targetOwner->GetType().IsBasedOn(TypeOf(Component)))
			return;

		Actor* sourceActor = mGetFunc(source);
		Actor* topSourceActor = sourceActor;
		while (topSourceActor->GetParent())
			topSourceActor = topSourceActor->GetParent();

		Actor* actorOwner = dynamic_cast<Actor*>(targetOwner);

		if (actorOwner)
		{
			Actor* topTargetActor = actorOwner;
			while (topTargetActor->GetPrototypeLink() != topSourceActor && topTargetActor->GetParent())
				topTargetActor = topTargetActor->GetParent();

			Actor* sameToProtoSourceActor = topTargetActor->FindLinkedActor(sourceActor);

			if (sameToProtoSourceActor)
			{
				mAssignFunc(target, sameToProtoSourceActor);
				return;
			}
		}

		if (sourceActor->IsOnScene())
			mAssignFunc(target, sourceActor);
	}

	UIWidget* ActorProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Actor* ActorProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ActorProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* ActorProperty::GetFieldType() const
	{
		return &TypeOf(Actor*);
	}

	void ActorProperty::SetValue(Actor* value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void ActorProperty::SetUnknownValue()
	{
		mCommonValue = nullptr;
		mValuesDifferent = true;

		mNameText->text = "--";
		mBox->layer["caption"]->transparency = 1.0f;

		CheckRevertableState();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	bool ActorProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	void ActorProperty::SetCommonValue(Actor* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		if (!mCommonValue)
		{
			mNameText->text = "Null:Actor";
			mBox->layer["caption"]->transparency = 0.5f;
		}
		else
		{
			mNameText->text = mCommonValue->GetName();
			mBox->layer["caption"]->transparency = 1.0f;
		}

		CheckRevertableState();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void ActorProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				Actor* value = mGetFunc(ptr.first);
				Actor* proto = mGetFunc(ptr.second);

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

	void ActorProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("hover", true);
	}

	void ActorProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("hover", false);
	}

	void ActorProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);

		if (mCommonValue)
		{
			if (mCommonValue->IsAsset())
				o2EditorAssets.ShowAssetIcon(o2Assets.GetAssetPath(mCommonValue->GetAssetId()));
			else if (mCommonValue->IsOnScene())
				o2EditorTree.HightlightActorsTreeNode(mCommonValue);
		}
	}

	void ActorProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox->IsFocused() && key == VK_DELETE || key == VK_BACK)
			SetValueByUser(nullptr);
	}

	void ActorProperty::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDroppedFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDroppedFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDragEnterFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDragEnterFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDragExitFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDragExitFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDroppedFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		SetValueByUser(actorsTree->GetSelectedActors()[0]);

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ActorProperty::OnDragEnterFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void ActorProperty::OnDragExitFromActorsTree(UIActorsTree* actorsTree)
	{
		if (actorsTree->GetSelectedActors().Count() > 1)
			return;

		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ActorProperty::OnDroppedFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto lastSelectedAsset = assetsIconsScroll->GetSelectedAssets().Last();
		if (lastSelectedAsset.assetType != &TypeOf(ActorAsset))
			return;

		SetValueByUser(o2Scene.GetAssetActorByID(lastSelectedAsset.id));

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ActorProperty::OnDragEnterFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto lastSelectedAsset = assetsIconsScroll->GetSelectedAssets().Last();
		if (lastSelectedAsset.assetType != &TypeOf(ActorAsset))
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void ActorProperty::OnDragExitFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ActorProperty::SetValueByUser(Actor* value)
	{
		StoreValues(mBeforeChangeValues);
		SetValue(value);
		CheckValueChangeCompleted();
	}

	void ActorProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void ActorProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::ActorProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::DragDropArea);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mBox);
	PROTECTED_FIELD(mNameText);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Actor*, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SetValue, Actor*);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, SetCommonValue, Actor*);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, RevertoToPrototype, void*, void*, IObject*);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDroppedFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDragEnterFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDragExitFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDroppedFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragEnterFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragExitFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, SetValueByUser, Actor*);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
