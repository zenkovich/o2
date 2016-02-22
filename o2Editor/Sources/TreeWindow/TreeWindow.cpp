#include "TreeWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Core/Actions/CreateActors.h"
#include "Core/Actions/DeleteActors.h"
#include "Core/Actions/EnableAction.h"
#include "Core/Actions/LockAction.h"
#include "Core/Actions/ReparentActors.h"
#include "Core/EditorApplication.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "Scene/Scene.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "Utils/Clipboard.h"

DECLARE_SINGLETON(TreeWindow);

UITree* TreeWindow::GetActorsTree() const
{
	return mActorsTree;
}

void TreeWindow::ExpandActorsTreeNode(Actor* targetActor)
{
	if (auto node = mActorsTree->GetNode((UnknownType*)(void*)targetActor))
		return;

	Vector<Actor*> parentsStack;
	Actor* treeVisibleNodeActor = targetActor;
	while (!mActorsTree->GetNode((UnknownType*)(void*)treeVisibleNodeActor))
	{
		treeVisibleNodeActor = treeVisibleNodeActor->GetParent();

		if (!treeVisibleNodeActor)
			return;

		parentsStack.Add(treeVisibleNodeActor);
	}

	for (int i = parentsStack.Count() - 1; i >= 0; i--)
	{
		auto node = mActorsTree->GetNode((UnknownType*)(void*)parentsStack[i]);
		node->Expand();
	}
}

TreeWindow::TreeWindow()
{
	InitializeWindow();
}

TreeWindow::TreeWindow(const TreeWindow& other):
	IEditorWindow(other)
{
	InitializeWindow();
}

TreeWindow::~TreeWindow()
{
	o2Scene.onActorCreated -= Function<void(Actor*)>(this, &TreeWindow::OnActorCreated);
	o2Scene.onActorDeleting -= Function<void(Actor*)>(this, &TreeWindow::OnActorDestroyed);
}

void TreeWindow::InitializeWindow()
{
	mWindow->caption = "Tree";
	mWindow->name = "tree window";
	if (auto iconLayer = mWindow->GetLayer("icon"))
	{
		*((Sprite*)iconLayer->drawable) = Sprite("ui/UI_tree_icon.png");
		iconLayer->layout = Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1));
	}

	mWindow->SetViewLayout(Layout::BothStretch(0, -2, 0, 18));

	UIWidget* upPanel = mnew UIWidget();
	upPanel->name = "up panel";
	upPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
	upPanel->AddLayer("back", mnew Sprite("ui/UI_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

	UIButton* searchButton = o2UI.CreateWidget<UIButton>("search");
	searchButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
	searchButton->onClick += Function<void()>(this, &TreeWindow::OnSearchPressed);
	upPanel->AddChild(searchButton);

	mListTreeToggle = o2UI.CreateWidget<UIToggle>("list-tree");
	mListTreeToggle->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
	mListTreeToggle->onToggle += Function<void(bool)>(this, &TreeWindow::OnListTreeToggled);
	upPanel->AddChild(mListTreeToggle);

	mSearchEditBox = o2UI.CreateWidget<UIEditBox>("backless");
	mSearchEditBox->layout = UIWidgetLayout::BothStretch(19, 2, 21, -2);
	mSearchEditBox->onChanged += Function<void(const WString&)>(this, &TreeWindow::OnSearchEdited);
	upPanel->AddChild(mSearchEditBox);

	mWindow->AddChild(upPanel);

	mActorsTree = o2UI.CreateWidget<UITree>("actors");
	mActorsTree->SetViewLayout(Layout::BothStretch(0, 0, 0, 0));
	mActorsTree->layout = UIWidgetLayout::BothStretch(2, 0, 0, 18);
	mActorsTree->SetNodesPoolResizeCount(100);

	mActorsTree->getParentFunc = Function<UnknownType*(UnknownType*)>(this, &TreeWindow::GetActorsParent);
	mActorsTree->getChildsFunc = Function<Vector<UnknownType*>(UnknownType*)>(this, &TreeWindow::GetActorsChildren);
	mActorsTree->setupNodeFunc = Function<void(UITreeNode*, UnknownType*)>(this, &TreeWindow::SetupTreeNodeActor);
	mActorsTree->onDraggedObjects = Function<void(Vector<UnknownType*>, UnknownType*, UnknownType*)>(this, &TreeWindow::RearrangeActors);
	mActorsTree->onItemDblClick = Function<void(UITreeNode*)>(this, &TreeWindow::OnTreeNodeDblClick);
	mActorsTree->onItemRBClick = Function<void(UITreeNode*)>(this, &TreeWindow::OnTreeRBPressed);

	mWindow->AddChild(mActorsTree);

	mEnableActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
	mEnableActorsTogglesGroup->onReleased = Function<void(bool)>(this, &TreeWindow::EnableActorsGroupReleased);

	mLockActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);
	mLockActorsTogglesGroup->onReleased = Function<void(bool)>(this, &TreeWindow::LockActorsGroupReleased);

	mTreeContextMenu = o2UI.CreateWidget<UIContextMenu>("standard");
	mTreeContextMenu->AddItems({
		UIContextMenu::Item("Create new", [&]() { OnContextCreateNewPressed(); }, nullptr, ShortcutKeys('N', true, true)),

		UIContextMenu::Item("Create", {
			UIContextMenu::Item("Sprite", [&]() { OnContextCreateSprite(); }),
			UIContextMenu::Item("Button", [&]() { OnContextCreateButton(); })
		}),

		UIContextMenu::Item::Separator(),

		UIContextMenu::Item("Copy", [&]() { OnContextCopyPressed(); }, nullptr, ShortcutKeys('C', true)),
		UIContextMenu::Item("Cut", [&]() { OnContextCutPressed(); }, nullptr, ShortcutKeys('X', true)),
		UIContextMenu::Item("Paste", [&]() { OnContextPastePressed(); }, nullptr, ShortcutKeys('V', true)),
		UIContextMenu::Item("Delete", [&]() { OnContextDeletePressed(); }, nullptr, ShortcutKeys(VK_DELETE)),
		UIContextMenu::Item("Duplicate", [&]() { OnContextDuplicatePressed(); }, nullptr, ShortcutKeys('D', true)),

		UIContextMenu::Item::Separator(),

		UIContextMenu::Item("Collapse all", [&]() { OnContextCollapseAllPressed(); }),
		UIContextMenu::Item("Expand all", [&]() { OnContextExpandAllPressed(); }),

		UIContextMenu::Item::Separator(),

		UIContextMenu::Item("Enable/disable", [&]() { OnContextEnablePressed(); }, nullptr, ShortcutKeys('L', true)),
		UIContextMenu::Item("Lock/unlock", [&]() { OnContextLockPressed(); }, nullptr, ShortcutKeys('O', true)),

		UIContextMenu::Item::Separator(),

		UIContextMenu::Item("Save prefab", [&]() { OnContextCopyPressed(); }),
		UIContextMenu::Item("Reset to prefab", [&]() { OnContextCopyPressed(); }),
		UIContextMenu::Item("Break link to prefab", [&]() { OnContextCopyPressed(); }),
	});

	mWindow->AddChild(mTreeContextMenu);

	for (int i = 0; i < 10; i++)
	{
		Actor* actor = mnew Actor();
		actor->name = String::Format("Actor #%i", i + 1);
		actor->layer = o2Scene.AddLayer(String::Format("Layer #%i", i + 1));

		for (int j = 0; j < 2; j++)
		{
			Actor* childActor = mnew Actor();
			childActor->name = String::Format("Child actor #%i", j + 1);
			actor->AddChild(childActor);

			for (int k = 0; k < 20; k++)
			{
				Actor* childActor2 = mnew Actor({ mnew ImageComponent("ui/UI_Background.png") });
				childActor2->name = String::Format("Sub Child actor #%i", k + 1);
				childActor2->transform.position = Vec2F(Math::Random(-500.0f, 500.0f), Math::Random(-500.0f, 500.0f));
				childActor->AddChild(childActor2);
			}
		}
	}

	mActorsTree->RebuildTree();
}

void TreeWindow::PostInitializeWindow()
{
	o2Scene.onActorCreated += Function<void(Actor*)>(this, &TreeWindow::OnActorCreated);
	o2Scene.onActorDeleting += Function<void(Actor*)>(this, &TreeWindow::OnActorDestroyed);
	//o2Scene.onChanged += [&](auto x) { mActorsTree->RebuildTree(); };
}

void TreeWindow::OnSearchPressed()
{}

void TreeWindow::OnListTreeToggled(bool value)
{}

void TreeWindow::OnSearchEdited(const WString& searchStr)
{
	mInSearch = searchStr.Length() > 0;

	if (mInSearch)
	{
		mSearchActors.Clear();

		for (auto actor : o2Scene.GetRootActors())
			SearchActorsRecursive(actor, (String)searchStr);
	}

	mActorsTree->RebuildTree();
}

void TreeWindow::SearchActorsRecursive(Actor* actor, const String& searchStr)
{
	if (actor->GetName().CountOf(searchStr) > 0)
		mSearchActors.Add(actor);

	for (auto child : actor->GetChilds())
		SearchActorsRecursive(child, searchStr);
}

UnknownType* TreeWindow::GetActorsParent(UnknownType* obj)
{
	if (mInSearch)
		return nullptr;

	Actor* actor = (Actor*)(void*)obj;
	return (UnknownType*)(void*)actor->GetParent();
}

Vector<UnknownType*> TreeWindow::GetActorsChildren(UnknownType* parentObj)
{
	if (mInSearch)
	{
		if (parentObj)
			return Vector<UnknownType*>();
		else
			return mSearchActors.Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
	}

	if (parentObj)
	{
		Actor* parent = (Actor*)(void*)parentObj;
		return parent->GetChilds().Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
	}

	return o2Scene.GetRootActors().Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
}

void TreeWindow::SetupTreeNodeActor(UITreeNode* node, UnknownType* actorObj)
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

	actor->onEnableChanged = [=](bool enabled) { mActorsTree->UpdateTreeNode(actorObj); };
	actor->onLockChanged = [=](bool enabled) { mActorsTree->UpdateTreeNode(actorObj); };
}

void TreeWindow::RearrangeActors(Vector<UnknownType*> objects, UnknownType*parentObj, UnknownType* prevObj)
{
	Actor* parent = (Actor*)(void*)parentObj;
	Actor* prevActor = (Actor*)(void*)prevObj;
	Vector<Actor*> actors = objects.Select<Actor*>([](UnknownType* x) { return (Actor*)(void*)x; });

	auto action = mnew EditorReparentActorsAction(actors);

	o2Scene.ReparentActors(actors, parent, prevActor);

	action->ActorsReparented(parent, prevActor);
	o2EditorApplication.DoneAction(action);
}

void TreeWindow::OnTreeNodeDblClick(UITreeNode* node)
{
	Actor* actor = (Actor*)(void*)node->GetObject();

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

void TreeWindow::OnTreeRBPressed(UITreeNode* node)
{
	mTreeContextMenu->Show();
}

void TreeWindow::EnableActorsGroupPressed(bool value)
{}

void TreeWindow::EnableActorsGroupReleased(bool value)
{
	auto action = mnew EditorEnableAction(
		mEnableActorsTogglesGroup->GetToggled().Select<Actor*>([](UIToggle* x) {
		return (Actor*)(void*)(((UITreeNode*)x->GetParent())->GetObject());
	}), value);

	o2EditorApplication.DoneAction(action);
}

void TreeWindow::LockActorsGroupPressed(bool value)
{}

void TreeWindow::LockActorsGroupReleased(bool value)
{
	auto action = mnew EditorLockAction(
		mLockActorsTogglesGroup->GetToggled().Select<Actor*>([](UIToggle* x) {
		return (Actor*)(void*)(((UITreeNode*)x->GetParent())->GetObject());
	}), value);

	o2EditorApplication.DoneAction(action);
}

void TreeWindow::CreateActor(Actor* newActor)
{
	auto selectedObjects = mActorsTree->GetSelectedObjects();

	if (selectedObjects.Count() > 0)
	{
		auto obj = selectedObjects.Last();
		auto node = mActorsTree->GetNode(obj);

		Actor* parentActor = (Actor*)(void*)obj;
		parentActor->AddChild(newActor);

		node->Rebuild(true);

		auto parentChilds = parentActor->GetChilds();
		auto action = mnew EditorCreateActorsAction({ newActor }, parentActor,
													parentChilds.Count() > 1 ? parentChilds[parentChilds.Count() - 2] : nullptr);
		o2EditorApplication.DoneAction(action);
	}
	else
	{
		mActorsTree->RebuildTree();

		auto scereActors = o2Scene.GetRootActors();
		auto action = mnew EditorCreateActorsAction({ newActor }, nullptr,
													scereActors.Count() > 1 ? scereActors[scereActors.Count() - 2] : nullptr);
		o2EditorApplication.DoneAction(action);
	}
}

void TreeWindow::OnContextCreateNewPressed()
{
	if (!mActorsTree->IsSelected())
		return;

	Actor* newActor = mnew Actor();
	newActor->name = "Empty";
	CreateActor(newActor);
}

void TreeWindow::OnContextCreateSprite()
{
	if (!mActorsTree->IsSelected())
		return;

	Actor* newActor = mnew Actor({ mnew ImageComponent() });
	newActor->name = "Sprite";
	newActor->transform.size = Vec2F(10, 10);
	CreateActor(newActor);
}

void TreeWindow::OnContextCreateButton()
{
	if (!mActorsTree->IsSelected())
		return;
}

void TreeWindow::OnContextCopyPressed()
{
	if (!mActorsTree->IsSelected())
		return;

	Vector<Actor*> selectedNodes = mActorsTree->GetSelectedObjects().Select<Actor*>([](auto x) { return (Actor*)(void*)x; });

	DataNode data;
	data = selectedNodes;

	WString clipboardData = data.SaveAsWString();

	Clipboard::SetText(clipboardData);
}

void TreeWindow::OnContextCutPressed()
{
	if (!mActorsTree->IsSelected())
		return;

	OnContextCopyPressed();
	OnContextDeletePressed();
}

void TreeWindow::OnContextPastePressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedObjs = mActorsTree->GetSelectedObjects();

	Actor* parent = selectedObjs.Count() > 0 ? (Actor*)(void*)selectedObjs.Last() : nullptr;

	WString clipboardData = Clipboard::GetText();
	DataNode data;
	data.LoadFromData(clipboardData);

	Vector<Actor*> actors;
	actors = data;

	for (auto actor : actors)
		actor->GenNewId();

	auto parentChilds = parent ? parent->GetChilds() : o2Scene.GetRootActors();
	Actor* prevActor = parentChilds.Count() > 0 ? parentChilds.Last() : nullptr;

	for (auto actor : actors)
		actor->SetParent(parent);

	mActorsTree->RebuildTree();

	Vector<UnknownType*> newActorsObjs = actors.Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
	mActorsTree->SetSelectedObjects(newActorsObjs);

	auto action = mnew EditorCreateActorsAction(actors, parent, prevActor);
	o2EditorApplication.DoneAction(action);
}

void TreeWindow::OnContextDeletePressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedActors = o2EditorSceneScreen.GetTopSelectedActors();
	o2EditorSceneScreen.ClearSelectionWithoutAction();

	auto action = mnew EditorDeleteActorsAction(selectedActors);
	o2EditorApplication.DoneAction(action);

	for (auto actor : selectedActors)
		delete actor;

	mActorsTree->RebuildTree();
}

void TreeWindow::OnContextDuplicatePressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedActors = o2EditorSceneScreen.GetTopSelectedActors();
	Vector<UnknownType*> newActorsObjs;

	for (auto actor : selectedActors)
	{
		Actor* copy = actor->Clone();
		copy->SetParent(actor->GetParent());

		newActorsObjs.Add((UnknownType*)(void*)copy);
	}

	mActorsTree->RebuildTree();
	mActorsTree->SetSelectedObjects(newActorsObjs);
}

void TreeWindow::OnContextExpandAllPressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		auto node = mActorsTree->GetNode(obj);

		if (node)
			node->Expand();
	}
}

void TreeWindow::OnContextCollapseAllPressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		auto node = mActorsTree->GetNode(obj);

		if (node)
			node->Collapse();
	}
}

void TreeWindow::OnContextLockPressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedActors = mActorsTree->GetSelectedObjects().Select<Actor*>([](UnknownType* x) { return (Actor*)(void*)x; });

	bool value = selectedActors.Count() > 0 ? !selectedActors.Last()->IsLocked() : true;
	auto action = mnew EditorLockAction(selectedActors, value);
	o2EditorApplication.DoneAction(action);

	for (auto actor : selectedActors)
	{
		actor->SetLocked(value);

		auto node = mActorsTree->GetNode((UnknownType*)(void*)actor);
		if (node)
			node->Rebuild();
	}
}

void TreeWindow::OnContextEnablePressed()
{
	if (!mActorsTree->IsSelected())
		return;

	auto selectedActors = mActorsTree->GetSelectedObjects().Select<Actor*>([](UnknownType* x) { return (Actor*)(void*)x; });

	bool value = selectedActors.Count() > 0 ? !selectedActors.Last()->IsEnabled() : true;
	auto action = mnew EditorLockAction(selectedActors, value);
	o2EditorApplication.DoneAction(action);

	for (auto actor : selectedActors)
	{
		actor->SetEnabled(value);

		auto node = mActorsTree->GetNode((UnknownType*)(void*)actor);
		if (node)
			node->Rebuild();
	}
}

void TreeWindow::OnActorCreated(Actor* actor)
{
	mActorsTree->RebuildTree();
}

void TreeWindow::OnActorDestroyed(Actor* actor)
{
	mActorsTree->RebuildTree();
}

