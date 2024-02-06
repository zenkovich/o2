#include "o2Editor/stdafx.h"
#include "ComponentProperty.h"

#include "o2/Application/Application.h"
#include "o2/Assets/Assets.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"


namespace Editor
{
	ComponentProperty::ComponentProperty()
	{
		InitializeControls();
	}

	ComponentProperty::ComponentProperty(const ComponentProperty& other) :
		TPropertyField<Ref<Component>>(other)
	{
		InitializeControls();
	}

	ComponentProperty& ComponentProperty::operator=(const ComponentProperty& other)
	{
		TPropertyField<Ref<Component>>::operator=(other);
		InitializeControls();
		return *this;
	}

	void ComponentProperty::InitializeControls()
	{
		mCommonValue = nullptr;
		mComponentType = &TypeOf(Component);

		mBox = GetChildWidget("container/layout/box");
		if (mBox)
		{
			mBox->onDraw += [&]() { DragDropArea::OnDrawn(); };
			mBox->SetFocusable(true);

			mNameText = mBox->GetLayerDrawable<Text>("caption");
			if (mNameText)
				mNameText->text = "--";
		}
	}

	void ComponentProperty::Revert()
	{
		auto propertyObjects = o2EditorPropertiesWindow.GetTargets();

		for (int i = 0; i < mValuesProxies.Count() && i < propertyObjects.Count(); i++)
			RevertoToPrototype(mValuesProxies[i].first, mValuesProxies[i].second, propertyObjects[i]);

		Refresh();
	}

	bool ComponentProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	void ComponentProperty::OnTypeSpecialized(const Type& type)
	{
		TPropertyField<Ref<Component>>::OnTypeSpecialized(type);
		mComponentType = type.InvokeStatic<const Type*>("GetComponentTypeStatic");
	}

	bool ComponentProperty::IsValueRevertable() const
	{
		for (auto& ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				Ref<Component> value = GetProxy(ptr.first);
				Ref<Component> proto = GetProxy(ptr.second);

				if (value && value->GetPrototypeLink())
				{
					if (proto != value->GetPrototypeLink())
						return true;
				}
				else
				{
					if (value != proto)
						return true;
				}
			}
		}

		return false;
	}

	void ComponentProperty::UpdateValueView()
	{
		if (mValuesDifferent)
		{
			mNameText->text = "--";
			mBox->layer["caption"]->transparency = 1.0f;
		}
		else
		{
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
		}
	}

	void ComponentProperty::RevertoToPrototype(IAbstractValueProxy* target, IAbstractValueProxy* source, IObject* targetOwner)
	{
		if (!source || !targetOwner || targetOwner->GetType().IsBasedOn(TypeOf(Component)))
			return;

		Ref<Component> sourceComponent = GetProxy(source);
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
				Component* sameToProtoSourceComponent = sameToProtoSourceActor->GetComponents().FindOrDefault(
					[&](Component* x) { return sourceComponent == x->GetPrototypeLink(); });

				if (sameToProtoSourceComponent)
				{
					SetProxy(target, sameToProtoSourceComponent);
					return;
				}
			}
		}

		if (sourceComponent->GetOwnerActor()->IsOnScene())
			SetProxy(target, sourceComponent);
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
					o2EditorTree.HighlightObjectTreeNode(ownerActor);
			}
		}
	}

	void ComponentProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox && mBox->IsFocused() && (key == VK_DELETE || key == VK_BACK))
			SetValueByUser(nullptr);
	}

	void ComponentProperty::OnDropped(const Ref<ISelectableDragableObjectsGroup>& group)
	{
		if (auto* actorsTree = dynamic_cast<SceneHierarchyTree*>(group))
			OnDroppedFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group))
			OnDroppedFromAssetsScroll(assetsScroll);
	}

	void ComponentProperty::OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group)
	{
		if (auto* actorsTree = dynamic_cast<SceneHierarchyTree*>(group))
			OnDragEnterFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group))
			OnDragEnterFromAssetsScroll(assetsScroll);
	}

	void ComponentProperty::OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group)
	{
		if (auto* actorsTree = dynamic_cast<SceneHierarchyTree*>(group))
			OnDragExitFromActorsTree(actorsTree);
		else if (auto* assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group))
			OnDragExitFromAssetsScroll(assetsScroll);
	}

	void ComponentProperty::OnDroppedFromActorsTree(SceneHierarchyTree* actorsTree)
	{
		if (actorsTree->GetSelectedObjects().Count() > 1)
			return;

		if (Actor* actor = dynamic_cast<Actor*>(actorsTree->GetSelectedObjects()[0]))
			SetValueByUser(actor->GetComponent(mComponentType));
		else
			return;

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ComponentProperty::OnDragEnterFromActorsTree(SceneHierarchyTree* actorsTree)
	{
		if (actorsTree->GetSelectedObjects().Count() != 1)
			return;

		Actor* actor = dynamic_cast<Actor*>(actorsTree->GetSelectedObjects()[0]);
		if (!actor)
			return;

		if (!actor->GetComponent(mComponentType))
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void ComponentProperty::OnDragExitFromActorsTree(SceneHierarchyTree* actorsTree)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void ComponentProperty::OnDroppedFromAssetsScroll(AssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto actor = o2Scene.GetAssetActorByID(assetsIconsScroll->GetSelectedAssets().Last()->meta->ID());
		SetValueByUser(actor->GetComponent(mComponentType));

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}

	void ComponentProperty::OnDragEnterFromAssetsScroll(AssetsIconsScrollArea* assetsIconsScroll)
	{
		if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
			return;

		auto actor = o2Scene.GetAssetActorByID(assetsIconsScroll->GetSelectedAssets().Last()->meta->ID());
		if (actor)
		{
			auto component = actor->GetComponent(mComponentType);

			if (!component)
				return;

			o2Application.SetCursor(CursorType::Hand);
			mBox->SetState("focused", true);
		}
	}

	void ComponentProperty::OnDragExitFromAssetsScroll(AssetsIconsScrollArea* assetsIconsScroll)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}
}
DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::Ref<Component>>);
// --- META ---

DECLARE_CLASS(Editor::ComponentProperty, Editor__ComponentProperty);
// --- END META ---
