#include "o2Editor/stdafx.h"
#include "ActorProperty.h"

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
	ActorProperty::ActorProperty()
	{
		mCommonValue = nullptr;
	}

	ActorProperty::ActorProperty(const ActorProperty& other) :
		TPropertyField<Ref<Actor>>(other)
	{
		InitializeControls();
	}

	ActorProperty& ActorProperty::operator=(const ActorProperty& other)
	{
		TPropertyField<Ref<Actor>>::operator=(other);
		InitializeControls();
		return *this;
	}

	void ActorProperty::InitializeControls()
	{
		mCommonValue = nullptr;

		mBox = GetChildWidget("container/layout/box");
		if (mBox)
		{
			mBox->SetFocusable(true);
			mBox->onDraw += MakeFunction<CursorAreaEventsListener, void>(this, &DragDropArea::OnDrawn);

			mNameText = mBox->GetLayerDrawable<Text>("caption");
			if (mNameText)
				mNameText->text = "--";
		}
	}

	void ActorProperty::Revert()
	{
		auto propertyObjects = o2EditorPropertiesWindow.GetTargets();

		for (int i = 0; i < mValuesProxies.Count() && i < propertyObjects.Count(); i++)
			RevertoToPrototype(mValuesProxies[i].first, mValuesProxies[i].second, propertyObjects[i]);

		Refresh();
	}

	bool ActorProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	void ActorProperty::RevertoToPrototype(IAbstractValueProxy* target, IAbstractValueProxy* source,
										   IObject* targetOwner)
	{
		if (!source || !targetOwner || DynamicCast<Component>(targetOwner))
			return;

		Ref<Actor> sourceActor = GetProxy(source);
		Ref<Actor> topSourceActor = sourceActor;
		while (topSourceActor->GetParent())
			topSourceActor = topSourceActor->GetParent();

		Actor* actorOwner = DynamicCast<Actor>(targetOwner);

		if (actorOwner)
		{
			Actor* topTargetActor = actorOwner;
			while (topTargetActor->GetPrototypeLink() != topSourceActor && topTargetActor->GetParent())
				topTargetActor = topTargetActor->GetParent();

			Actor* sameToProtoSourceActor = topTargetActor->FindLinkedActor(sourceActor.Get());

			if (sameToProtoSourceActor)
			{
				SetProxy(target, sameToProtoSourceActor);
				return;
			}
		}

		if (sourceActor->IsOnScene())
			SetProxy(target, sourceActor);
	}

	void ActorProperty::OnTypeSpecialized(const Type& type)
	{
		TPropertyField<Ref<Actor>>::OnTypeSpecialized(type);
		mActorType = type.InvokeStatic<const Type*>("GetActorTypeStatic");
	}

	bool ActorProperty::IsValueRevertable() const
	{
		bool revertable = false;

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				Ref<Actor> value = GetProxy(ptr.first);
				Ref<Actor> proto = GetProxy(ptr.second);

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

		return revertable;
	}

	void ActorProperty::UpdateValueView()
	{
		if (!mValuesDifferent)
		{
			if (!mCommonValue)
			{
				mNameText->text = "Null:" + mActorType->GetName();
				mBox->layer["caption"]->transparency = 0.5f;
			}
			else
			{
				mNameText->text = mCommonValue->GetName();
				mBox->layer["caption"]->transparency = 1.0f;
			}
		}
		else
		{
			mNameText->text = "--";
			mBox->layer["caption"]->transparency = 1.0f;
		}
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
				o2EditorAssets.ShowAssetIcon(o2Assets.GetAssetPath(mCommonValue->GetAssetID()));
			else if (mCommonValue->IsOnScene())
				o2EditorTree.HighlightObjectTreeNode(mCommonValue.Get());
		}
	}

	void ActorProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox && mBox->IsFocused() && (key == VK_DELETE || key == VK_BACK))
			SetValueByUser(nullptr);
	}

	void ActorProperty::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = DynamicCast<SceneHierarchyTree*>(group))
			OnDroppedFromActorsTree(actorsTree);
		else if (auto* assetsScroll = DynamicCast<AssetsIconsScrollArea*>(group))
			OnDroppedFromAssetsScroll(assetsScroll);
	}

	void ActorProperty::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		if (auto* actorsTree = DynamicCast<SceneHierarchyTree*>(group))
			OnDragEnterFromActorsTree(actorsTree);
		else if (auto* Replace raw pointers with `Ref<>` in the provided code, except for `void*` pointers. Use `const Ref<>&` for function arguments with pointer types and in overridden methods. Remove the default `nullptr` value for class members with type `Ref<>`. If a class variable is some kind of "parent", use `WeakRef<>`. Use `mmake<>` to create `Ref<>` objects, similar to `make_shared<>`. Replace `dynamic_cast<type*>` with `DynamicCast<type>` to cast `Ref<>` objects. 

Here's the updated code:

```
#include <Ref.h>
#include <WeakRef.h>

void ActorProperty::OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group)
{
    if (const auto* actorsTree = DynamicCast<SceneHierarchyTree>(group))
        OnDragEnterFromActorsTree(actorsTree);
    else if (const auto* assetsScroll = DynamicCast<AssetsIconsScrollArea>(group))
        OnDragEnterFromAssetsScroll(assetsScroll);
}

void ActorProperty::OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group)
{
    if (const auto* actorsTree = DynamicCast<SceneHierarchyTree>(group))
        OnDragExitFromActorsTree(actorsTree);
    else if (const auto* assetsScroll = DynamicCast<AssetsIconsScrollArea>(group))
        OnDragExitFromAssetsScroll(assetsScroll);
}

void ActorProperty::OnDropped(const Ref<ISelectableDragableObjectsGroup>& group)
{
    if (const auto* actorsTree = DynamicCast<SceneHierarchyTree>(group))
        OnDroppedFromActorsTree(actorsTree);
    else if (const auto* assetsIconsScroll = DynamicCast<AssetsIconsScrollArea>(group))
        OnDroppedFromAssetsScroll(assetsIconsScroll);
}

void ActorProperty::OnDroppedFromActorsTree(const Ref<SceneHierarchyTree>& actorsTree)
{
    if (actorsTree->GetSelectedObjects().Count() > 1)
        return;

    SetValueByUser(DynamicCast<Actor>(actorsTree->GetSelectedObjects()[0]));

    o2Application.SetCursor(CursorType::Arrow);
    mBox->Focus();
}

void ActorProperty::OnDragEnterFromActorsTree(const Ref<SceneHierarchyTree>& actorsTree)
{
    if (actorsTree->GetSelectedObjects().Count() > 1)
        return;

    const auto actor = DynamicCast<Actor>(actorsTree->GetSelectedObjects()[0]);
    if (!actor || !actor->GetType().IsBasedOn(*mActorType))
        return;

    o2Application.SetCursor(CursorType::Hand);
    mBox->SetState("focused", true);
}

void ActorProperty::OnDragExitFromActorsTree(const Ref<SceneHierarchyTree>& actorsTree)
{
    if (actorsTree->GetSelectedObjects().Count() > 1)
        return;

    o2Application.SetCursor(CursorType::Arrow);
    mBox->SetState("focused", false);
}

void ActorProperty::OnDroppedFromAssetsScroll(const Ref<AssetsIconsScrollArea>& assetsIconsScroll)
{
    if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
        return;

    const auto lastSelectedAsset = assetsIconsScroll->GetSelectedAssets().Last();
    if (lastSelectedAsset->meta->GetAssetType() != &TypeOf(ActorAsset))
        return;

    SetValueByUser(o2Scene.GetAssetActorByID(lastSelectedAsset->meta->ID()));

    o2Application.SetCursor(CursorType::Arrow);
    mBox->Focus();
}

void ActorProperty::OnDragEnterFromAssetsScroll(const Ref<AssetsIconsScrollArea>& assetsIconsScroll)
{
    if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
        return;

    const auto lastSelectedAsset = assetsIconsScroll->GetSelectedAssets().Last();
    if (lastSelectedAsset->meta->GetAssetType() != &TypeOf(ActorAsset))
        return;

    o2Application.SetCursor(CursorType::Hand);
    mBox->SetState("focused", true);
}

void ActorProperty::OnDragExitFromAssetsScroll(const Ref<AssetsIconsScrollArea>& assetsIconsScroll)
{
    if (assetsIconsScroll->GetSelectedAssets().Count() > 1)
        return;

    o2Application.SetCursor(CursorType::Arrow);
    mBox->SetState("focused", false);
}
}
DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::Ref<Actor>>);
// --- META ---

DECLARE_CLASS(Editor::ActorProperty, Editor__ActorProperty);
// --- END META ---
```

Note that the code assumes the existence and correct usage of `Ref.h`, `WeakRef.h`, and `mmake<>`. Please make sure these dependencies are available and properly include them in your project.