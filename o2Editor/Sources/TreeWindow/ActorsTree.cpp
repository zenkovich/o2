#include "ActorsTree.h"

#include "Assets/ActorAsset.h"
#include "Assets/Assets.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/UIAssetIcon.h"
#include "Core/Actions/EnableAction.h"
#include "Core/Actions/LockAction.h"
#include "Core/Actions/ReparentActors.h"
#include "Core/EditorApplication.h"
#include "Events/EventSystem.h"
#include "PropertiesWindow/Properties/ActorProperty.h"
#include "PropertiesWindow/Properties/ComponentProperty.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/Toggle.h"

namespace Editor
{
	UIActorsTree::UIActorsTree():
		UITree(), mAttackedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	UIActorsTree::UIActorsTree(const UIActorsTree& other):
		UITree(other), mAttackedToSceneEvents(false), mDragActorPropertyField(nullptr), mDragComponentPropertyField(nullptr)
	{
		Initialize();
	}

	UIActorsTree::~UIActorsTree()
	{
		if (Scene::IsSingletonInitialzed())
		{
			o2Scene.onActorCreated -= Function<void(Actor*)>(this, &UIActorsTree::OnActorCreated);
			o2Scene.onActorDestroying -= Function<void(Actor*)>(this, &UIActorsTree::OnActorDestroyed);

			if (mAttackedToSceneEvents)
			{
				auto updateActorTreeNode = Function<void(Actor*)>(this, &UIActorsTree::UpdateTreeNode);

				o2Scene.onActorCreated -= Function<void(Actor*)>(this, &UIActorsTree::OnActorCreated);
				o2Scene.onActorDestroying -= Function<void(Actor*)>(this, &UIActorsTree::OnActorDestroyed);

				o2Scene.onActorEnableChanged -= updateActorTreeNode;
				o2Scene.onActorLockChanged -= updateActorTreeNode;
				o2Scene.onActorNameChanged -= updateActorTreeNode;
				o2Scene.onActorChildsHierarchyChanged -= updateActorTreeNode;
			}
		}
	}

	UIActorsTree& UIActorsTree::operator=(const UIActorsTree& other)
	{
		UITree::operator=(other);
		return *this;
	}

	void UIActorsTree::AttachToSceneEvents()
	{
		auto updateActorTreeNode = Function<void(Actor*)>(this, &UIActorsTree::UpdateTreeNode);

		o2Scene.onActorCreated += Function<void(Actor*)>(this, &UIActorsTree::OnActorCreated);
		o2Scene.onActorDestroying += Function<void(Actor*)>(this, &UIActorsTree::OnActorDestroyed);

		o2Scene.onActorEnableChanged += updateActorTreeNode;
		o2Scene.onActorLockChanged += updateActorTreeNode;
		o2Scene.onActorNameChanged += updateActorTreeNode;
		o2Scene.onActorChildsHierarchyChanged += updateActorTreeNode;

		mAttackedToSceneEvents = true;
	}

	void UIActorsTree::Draw()
	{
		UITree::Draw();
	}

	void UIActorsTree::Update(float dt)
	{
		UITree::Update(dt);
	}

	void UIActorsTree::RebuildTree()
	{
		UITree::RebuildTree();
	}

	void UIActorsTree::UpdateTreeNode(Actor* object)
	{
		UITree::UpdateTreeNode((UnknownType*)(void*)object);
	}

	UITreeNode* UIActorsTree::GetNode(Actor* object)
	{
		return UITree::GetNode((UnknownType*)(void*)object);
	}

	void UIActorsTree::ExpandAll()
	{
		UITree::ExpandAll();
	}

	void UIActorsTree::CollapseAll()
	{
		UITree::CollapseAll();
	}

	void UIActorsTree::ManualBeginDraggingActors(const ActorsVec& actors)
	{
		mDraggingNodes = true;
		SetSelectedActors(actors);

		mDragOffset = Vec2F();
		mDragNode->Show(true);

		setupNodeFunc(mDragNode, mSelectedItems.Last().object);

		if (mSelectedItems.Count() > 1)
		{
			if (auto nameLayer = mDragNode->FindLayer<Text>())
				nameLayer->text = String::Format("%i items", mSelectedItems.Count());
		}

		for (auto sel : mSelectedItems)
		{
			if (sel.node)
				sel.node->SetState("dragging", true);
		}
	}

	void UIActorsTree::ManualUpdateDraggingActors(const Input::Cursor& cursor)
	{
		UpdateDragging(cursor);
	}

	void UIActorsTree::CompleteManualDraggingActors()
	{
		EndDragging();
	}

	void UIActorsTree::BreakDragging()
	{
		mDraggingNodes = false;
		mDragNode->Hide(true);

		for (auto node : mAllNodes)
			node->SetState("inserting", false);

		for (auto sel : mSelectedItems)
		{
			if (sel.node)
				sel.node->SetState("dragging", false);
		}

		DeselectAllActors();
	}

	UIActorsTree::ActorsVec UIActorsTree::GetSelectedActors() const
	{
		return UITree::GetSelectedObjects().Select<Actor*>([](auto x) { return (Actor*)(void*)x; });
	}

	void UIActorsTree::SetSelectedActors(const ActorsVec& actors)
	{
		UITree::SetSelectedObjects(actors.Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; }));
	}

	void UIActorsTree::SelectActor(Actor* object)
	{
		UITree::SelectObject((UnknownType*)(void*)object);
	}

	void UIActorsTree::SelectAndExpandActor(Actor* object)
	{
		UITree::SelectAndExpandObject((UnknownType*)(void*)object);
	}

	void UIActorsTree::DeselectActor(Actor* object)
	{
		UITree::DeselectObject((UnknownType*)(void*)object);
	}

	void UIActorsTree::DeselectAllActors()
	{
		UITree::DeselectAllObjects();
	}

	UITreeNode* UIActorsTree::GetTreeNodeUnderPoint(const Vec2F& point)
	{
		return UITree::GetTreeNodeUnderPoint(point);
	}

	UITreeNode* UIActorsTree::GetNodeSample() const
	{
		return UITree::GetNodeSample();
	}

	Sprite* UIActorsTree::GetHoverDrawable() const
	{
		return UITree::GetHoverDrawable();
	}

	Sprite* UIActorsTree::GetSelectionDrawable() const
	{
		return UITree::GetSelectionDrawable();
	}

	void UIActorsTree::SetSelectionDrawableLayout(const Layout& layout)
	{
		UITree::SetSelectionDrawableLayout(layout);
	}

	Layout UIActorsTree::GetSelectionDrawableLayout() const
	{
		return UITree::GetSelectionDrawableLayout();
	}

	bool UIActorsTree::IsScrollable() const
	{
		return true;
	}

	void UIActorsTree::SetNodesPoolResizeCount(int count)
	{
		UITree::SetNodesPoolResizeCount(count);
	}

	void UIActorsTree::SetSelectionSpritesPoolResizeCount(int count)
	{
		UITree::SetSelectionSpritesPoolResizeCount(count);
	}

	void UIActorsTree::SetSelectedColor(const Color4& color)
	{
		UITree::SetSelectedColor(color);
	}

	Color4 UIActorsTree::GetSelectedColor() const
	{
		return UITree::GetSelectedColor();
	}

	void UIActorsTree::SetUnselectedColor(const Color4& color)
	{
		UITree::SetUnselectedColor(color);
	}

	Color4 UIActorsTree::GetUnselectedColor() const
	{
		return UITree::GetUnselectedColor();
	}

	void UIActorsTree::SetHoverColor(const Color4& color)
	{
		UITree::SetHoverColor(color);
	}

	Color4 UIActorsTree::GetHoverColor() const
	{
		return UITree::GetHoverColor();
	}

	bool UIActorsTree::IsSelectable() const
	{
		return true;
	}

	void UIActorsTree::Initialize()
	{
		mEnableActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
		mEnableActorsTogglesGroup->onReleased = Function<void(bool)>(this, &UIActorsTree::EnableActorsGroupReleased);

		mLockActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
		mLockActorsTogglesGroup->onReleased = Function<void(bool)>(this, &UIActorsTree::LockActorsGroupReleased);

		SetNodesPoolResizeCount(100);
		getParentFunc = Function<UnknownType*(UnknownType*)>(this, &UIActorsTree::GetActorsParent);
		getChildsFunc = Function<Vector<UnknownType*>(UnknownType*)>(this, &UIActorsTree::GetActorsChildren);
		setupNodeFunc = Function<void(UITreeNode*, UnknownType*)>(this, &UIActorsTree::SetupTreeNodeActor);
		onDraggedObjects = Function<void(Vector<UnknownType*>, UnknownType*, UnknownType*)>(this, &UIActorsTree::RearrangeActors);
		onItemDblClick = Function<void(UITreeNode*, Actor*)>(this, &UIActorsTree::OnTreeNodeDblClick);

		UITree::onItemRBClick = [&](UITreeNode* x) { 
			Actor* actor = x ? (Actor*)(void*)x->GetObject() : nullptr;
			onItemRBClick(x, actor);
		};

		UITree::onItemsSelectionChanged = [&](Vector<UnknownType*> x) {
			onItemsSelectionChanged(x.Select<Actor*>([](auto x) { return (Actor*)(void*)x; })); };
	}

	UnknownType* UIActorsTree::GetActorsParent(UnknownType* obj)
	{
		Actor* actor = (Actor*)(void*)obj;
		return (UnknownType*)(void*)actor->GetParent();
	}

	Vector<UnknownType*> UIActorsTree::GetActorsChildren(UnknownType* parentObj)
	{
		if (parentObj)
		{
			Actor* parent = (Actor*)(void*)parentObj;
			return parent->GetChilds().Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
		}

		return o2Scene.GetRootActors().Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
	}

	void UIActorsTree::SetupTreeNodeActor(UITreeNode* node, UnknownType* actorObj)
	{
		Actor* actor = (Actor*)(void*)actorObj;

		node->name = actor->GetName();

		auto nameLayer = node->layer["name"];
		((Text*)nameLayer->drawable)->text = actor->GetName();

		auto enableToggle = (UIToggle*)node->GetChild("enableToggle");
		enableToggle->SetValue(actor->IsEnabled());
		enableToggle->onClick = [=]() { actor->SetEnabled(enableToggle->GetValue()); };
		enableToggle->SetToggleGroup(mEnableActorsTogglesGroup);

		auto lockToggle = (UIToggle*)node->GetChild("lockToggle");
		lockToggle->SetValue(actor->IsLocked());
		lockToggle->SetState("locked", actor->IsLockedInHierarchy());
		lockToggle->SetState("halfHide", actor->IsLockedInHierarchy() && !actor->IsLocked());
		lockToggle->onClick = [=]() { actor->SetLocked(lockToggle->GetValue()); };
		lockToggle->SetToggleGroup(mLockActorsTogglesGroup);

		auto lingBtn = (UIButton*)node->GetChild("linkBtn");

		float alpha = actor->IsEnabledInHierarchy() ? 1.0f : 0.5f;
		nameLayer->transparency = alpha;
		enableToggle->SetState("halfHide", !actor->IsEnabledInHierarchy());
		lingBtn->SetState("halfHide", !actor->IsEnabledInHierarchy());
	}

	void UIActorsTree::RearrangeActors(Vector<UnknownType*> objects, UnknownType*parentObj, UnknownType* prevObj)
	{
		Actor* parent = (Actor*)(void*)parentObj;
		Actor* prevActor = (Actor*)(void*)prevObj;
		Vector<Actor*> actors = objects.Select<Actor*>([](UnknownType* x) { return (Actor*)(void*)x; });

		auto action = mnew ReparentActorsAction(actors);

		o2Scene.ReparentActors(actors, parent, prevActor);

		action->ActorsReparented(parent, prevActor);
		o2EditorApplication.DoneAction(action);
	}

	void UIActorsTree::OnTreeNodeDblClick(UITreeNode* node, Actor* actor)
	{
		node->SetState("edit", true);

		auto editBox = (UIEditBox*)node->GetChild("nameEditBox");
		editBox->text = (String)actor->name;
		editBox->SelectAll();
		editBox->UIWidget::Select();
		editBox->ResetSroll();

		editBox->onChangeCompleted = [=](const WString& text) {
			actor->SetName(text);
			node->SetState("edit", false);
			node->Rebuild(false);
		};
	}

	void UIActorsTree::EnableActorsGroupPressed(bool value)
	{}

	void UIActorsTree::EnableActorsGroupReleased(bool value)
	{
		auto action = mnew EnableAction(
			mEnableActorsTogglesGroup->GetToggled().Select<Actor*>([](UIToggle* x) {
			return (Actor*)(void*)(((UITreeNode*)x->GetParent())->GetObject());
		}), value);

		o2EditorApplication.DoneAction(action);
	}

	void UIActorsTree::LockActorsGroupPressed(bool value)
	{}

	void UIActorsTree::LockActorsGroupReleased(bool value)
	{
		auto action = mnew LockAction(
			mLockActorsTogglesGroup->GetToggled().Select<Actor*>([](UIToggle* x) {
			return (Actor*)(void*)(((UITreeNode*)x->GetParent())->GetObject());
		}), value);

		o2EditorApplication.DoneAction(action);
	}

	void UIActorsTree::OnActorCreated(Actor* actor)
	{
		UITree::RebuildTree(false);
	}

	void UIActorsTree::OnActorDestroyed(Actor* actor)
	{
		UITree::RebuildTree(false);
	}

	void UIActorsTree::UpdateDragging(const Input::Cursor& cursor)
	{
		UITree::UpdateDragging(cursor);

		auto listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);
		ActorProperty* actorProperty = dynamic_cast<ActorProperty*>(listenerUnderCursor);
		ComponentProperty* componentProperty = dynamic_cast<ComponentProperty*>(listenerUnderCursor);

		if (actorProperty && !mDragActorPropertyField)
		{
			if (mSelectedItems.Count() == 1)
			{
				mDragActorPropertyField = actorProperty;
				mDragActorPropertyField->GetWidget()->SetState("selected", true);
				o2Application.SetCursor(CursorType::Hand);
			}
		}

		if (componentProperty && !mDragComponentPropertyField)
		{
			if (mSelectedItems.Count() == 1)
			{
				Actor* actor = (Actor*)(void*)mSelectedItems[0].object;
				if (actor->GetComponent(componentProperty->GetSpecializedType()))
				{
					mDragComponentPropertyField = componentProperty;
					mDragComponentPropertyField->GetWidget()->SetState("selected", true);
					o2Application.SetCursor(CursorType::Hand);
				}
			}
		}

		if (!actorProperty && mDragActorPropertyField)
		{
			mDragActorPropertyField->GetWidget()->SetState("selected", false);
			mDragActorPropertyField = nullptr;
			o2Application.SetCursor(CursorType::Arrow);
		}

		if (!componentProperty && mDragComponentPropertyField)
		{
			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
			mDragComponentPropertyField = nullptr;
			o2Application.SetCursor(CursorType::Arrow);
		}
	}

	void UIActorsTree::EndDragging()
	{
		auto listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);
		UIAssetsIconsScrollArea* assetsScrollArea = dynamic_cast<UIAssetsIconsScrollArea*>(listenerUnderCursor);
		if (assetsScrollArea)
		{
			String targetPath = assetsScrollArea->GetViewingPath();

			auto iconUnderCursor = assetsScrollArea->GetIconUnderPoint(o2Input.GetCursorPos());
			if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == TypeOf(FolderAsset).ID())
				targetPath = iconUnderCursor->GetAssetInfo().mPath;

			for (auto& sel : mSelectedItems)
			{
				ActorAsset newAsset;
				newAsset.actor = *(Actor*)(void*)sel.object;
				String path = targetPath.IsEmpty() ? newAsset.actor.name + ".prefab" : targetPath + "/" + newAsset.actor.name + ".prefab";
				newAsset.Save(o2Assets.MakeUniqueAssetName(path));
			}

			BreakDragging();
		}
		else if (mDragActorPropertyField)
		{
			mDragActorPropertyField->SetValue((Actor*)(void*)(mSelectedItems[0].object));
			mDragActorPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);

			mDragActorPropertyField = nullptr;

			mDraggingNodes = false;
			mDragNode->Hide(true);

			for (auto node : mAllNodes)
				node->SetState("inserting", false);

			for (auto sel : mSelectedItems)
			{
				if (sel.node)
					sel.node->SetState("dragging", false);
			}

			for (auto sel : mSelectedItems)
				FreeSelectionSprite(sel.selectionSprite);

			mSelectedItems.Clear();

			for (auto obj : mBeforeDragSelected)
			{
				SelectedNode selectionNode;
				selectionNode.object = obj.object;
				selectionNode.selectionSprite = CreateSelectionSprite();
				selectionNode.node = UITree::GetNode(obj.object);

				mSelectedItems.Add(selectionNode);
			}

			UpdateLayout();
		}
		else if (mDragComponentPropertyField)
		{
			Actor* actor = (Actor*)(void*)mSelectedItems[0].object;
			auto component = actor->GetComponent(mDragComponentPropertyField->GetSpecializedType());

			mDragComponentPropertyField->SetValue(component);
			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
			o2Application.SetCursor(CursorType::Arrow);

			mDragComponentPropertyField = nullptr;

			mDraggingNodes = false;
			mDragNode->Hide(true);

			for (auto node : mAllNodes)
				node->SetState("inserting", false);

			for (auto sel : mSelectedItems)
			{
				if (sel.node)
					sel.node->SetState("dragging", false);
			}

			for (auto sel : mSelectedItems)
				FreeSelectionSprite(sel.selectionSprite);

			mSelectedItems.Clear();

			for (auto obj : mBeforeDragSelected)
			{
				SelectedNode selectionNode;
				selectionNode.object = obj.object;
				selectionNode.selectionSprite = CreateSelectionSprite();
				selectionNode.node = UITree::GetNode(obj.object);

				mSelectedItems.Add(selectionNode);
			}

			UpdateLayout();
		}
		else UITree::EndDragging();
	}
}