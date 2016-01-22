#include "TreeWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "Scene/Scene.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "Utils/Clipboard.h"

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
{}

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
	mLockActorsTogglesGroup = mnew UIToggleGroup(UIToggleGroup::Type::VerOneClick);

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

		UIContextMenu::Item("Enable/disable", [&]() { OnContextLockPressed(); }, nullptr, ShortcutKeys('L', true)),
		UIContextMenu::Item("Lock/unlock", [&]() { OnContextEnablePressed(); }, nullptr, ShortcutKeys('O', true)),

		UIContextMenu::Item::Separator(),

		UIContextMenu::Item("Save prefab", [&]() { OnContextCopyPressed(); }),
		UIContextMenu::Item("Reset to prefab", [&]() { OnContextCopyPressed(); }),
		UIContextMenu::Item("Break link to prefab", [&]() { OnContextCopyPressed(); }),
	});

	mWindow->AddChild(mTreeContextMenu);

	for (int i = 0; i < 30; i++)
	{
		Actor* actor = mnew Actor();
		actor->name = String::Format("Actor #%i", i + 1);

		for (int j = 0; j < 30; j++)
		{
			Actor* childActor = mnew Actor();
			childActor->name = String::Format("Child actor #%i", j + 1);
			actor->AddChild(childActor);

			for (int k = 0; k < 5; k++)
			{
				Actor* childActor2 = mnew Actor();
				childActor2->name = String::Format("Sub Child actor #%i", k + 1);
				childActor->AddChild(childActor2);
			}
		}
	}

	mActorsTree->RebuildTree();
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

		for (auto actor : o2Scene.GetAllActors())
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

	return o2Scene.GetAllActors().Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
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

int GetActorIdx(Actor* actor)
{
	if (actor->GetParent())
	{
		return actor->GetParent()->GetChilds().Find(actor) + GetActorIdx(actor->GetParent());
	}

	return o2Scene.GetAllActors().Find(actor);
}

void TreeWindow::RearrangeActors(Vector<UnknownType*> objects, UnknownType*parentObj, UnknownType* prevObj)
{
	Actor* parent = (Actor*)(void*)parentObj;

	struct ActorDef
	{
		Actor* actor;
		int    idx;
		Basis  transform;

		bool operator==(const ActorDef& other) const { return actor == other.actor; }
	};
	Vector<ActorDef> actorsDefs;

	for (auto obj : objects)
	{
		ActorDef def;
		def.actor = (Actor*)(void*)obj;
		def.idx = GetActorIdx(def.actor);
		def.transform = def.actor->transform.GetWorldBasis();
		actorsDefs.Add(def);
	}

	for (auto def : actorsDefs)
		def.actor->ExcludeFromScene();

	actorsDefs.Sort([](auto& a, auto& b) { return a.idx < b.idx; });

	if (parent)
	{
		int insertIdx = parent->GetChilds().Find((Actor*)(void*)prevObj);
		if (insertIdx < 0)
			insertIdx = parent->GetChilds().Count();

		for (auto def : actorsDefs)
		{
			parent->AddChild(def.actor, insertIdx++);
			def.actor->transform.SetWorldBasis(def.transform);
		}
	}
	else
	{
		int insertIdx = o2Scene.GetAllActors().Find((Actor*)(void*)prevObj);
		if (insertIdx < 0)
			insertIdx = o2Scene.GetAllActors().Count();

		for (auto def : actorsDefs)
		{
			def.actor->SetPositionIndexInParent(insertIdx++);
			def.actor->transform.SetWorldBasis(def.transform);
		}
	}
}

void TreeWindow::OnTreeNodeDblClick(UITreeNode* node)
{
	Actor* actor = (Actor*)(void*)node->GetObject();

	node->SetState("edit", true);

	auto editBox = (UIEditBox*)node->GetChild("nameEditBox");
	editBox->text = (String)actor->name;
	editBox->SelectAll();
	editBox->UIWidget::Select();

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

void TreeWindow::OnContextCreateNewPressed()
{
	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		auto node = mActorsTree->GetNode(obj);

		Actor* newActor = mnew Actor();
		newActor->name = "Empty";
		Actor* parentActor = (Actor*)(void*)obj;
		parentActor->AddChild(newActor);

		node->Rebuild(true);
	}
}

void TreeWindow::OnContextCreateSprite()
{
	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		auto node = mActorsTree->GetNode(obj);

		Actor* newActor = mnew Actor({ mnew ImageComponent() });
		newActor->name = "Sprite";
		Actor* parentActor = (Actor*)(void*)obj;
		parentActor->AddChild(newActor);

		node->Rebuild(true);
	}
}

void TreeWindow::OnContextCreateButton()
{}

void TreeWindow::OnContextCopyPressed()
{
	Vector<Actor*> selectedNodes = mActorsTree->GetSelectedObjects().Select<Actor*>([](auto x) { return (Actor*)(void*)x; });

	DataNode data;
	data = selectedNodes;

	WString clipboardData = data.SaveAsWString();

	Clipboard::SetText(clipboardData);
}

void TreeWindow::OnContextCutPressed()
{
	OnContextCopyPressed();
	OnContextDeletePressed();
}

void TreeWindow::OnContextPastePressed()
{
	auto selectedObjs = mActorsTree->GetSelectedObjects();

	Actor* parent = selectedObjs.Count() > 0 ? (Actor*)(void*)selectedObjs.Last() : nullptr;

	WString clipboardData = Clipboard::GetText();
	DataNode data;
	data.LoadFromData(clipboardData);

	Vector<Actor*> actors;
	actors = data;

	for (auto actor : actors)
		actor->SetParent(parent);

	mActorsTree->RebuildTree();

	Vector<UnknownType*> newActorsObjs = actors.Select<UnknownType*>([](auto x) { return (UnknownType*)(void*)x; });
	mActorsTree->SetSelectedObjects(newActorsObjs);
}

void TreeWindow::OnContextDeletePressed()
{
	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		Actor* actor = (Actor*)(void*)obj;
		delete actor;
	}

	mActorsTree->RebuildTree();
}

void TreeWindow::OnContextDuplicatePressed()
{
	auto selectedObjects = mActorsTree->GetSelectedObjects();
	Vector<UnknownType*> newActorsObjs;

	for (auto obj : selectedObjects)
	{
		Actor* actor = (Actor*)(void*)obj;

		Actor* copy = actor->Clone();
		copy->SetParent(actor->GetParent());

		newActorsObjs.Add((UnknownType*)(void*)copy);
	}

	mActorsTree->RebuildTree();
	mActorsTree->SetSelectedObjects(newActorsObjs);
}

void TreeWindow::OnContextExpandAllPressed()
{
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
	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		Actor* actor = (Actor*)(void*)obj;
		actor->SetLocked(!actor->IsLocked());

		auto node = mActorsTree->GetNode(obj);
		if (node)
			node->Rebuild();
	}
}

void TreeWindow::OnContextEnablePressed()
{
	auto selectedObjects = mActorsTree->GetSelectedObjects();

	for (auto obj : selectedObjects)
	{
		Actor* actor = (Actor*)(void*)obj;
		actor->SetEnabled(!actor->IsEnabled());

		auto node = mActorsTree->GetNode(obj);
		if (node)
			node->Rebuild();
	}
}

