#include "TreeWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Core/Actions/CreateActors.h"
#include "Core/Actions/DeleteActors.h"
#include "Core/Actions/EnableAction.h"
#include "Core/Actions/LockAction.h"
#include "Core/Actions/ReparentActors.h"
#include "Core/EditorApplication.h"
#include "Scene/Actor.h"
#include "Scene/Components/EditorTestComponent.h"
#include "Scene/Components/ImageComponent.h"
#include "Scene/Scene.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "Utils/Clipboard.h"

DECLARE_SINGLETON(Editor::TreeWindow);

namespace Editor
{
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

	UIActorsTree* TreeWindow::GetActorsTree() const
	{
		return mActorsTree;
	}

	void TreeWindow::ExpandActorsTreeNode(Actor* targetActor)
	{
		if (!mActorsTree->GetNode(targetActor))
		{

			Vector<Actor*> parentsStack;
			Actor* treeVisibleNodeActor = targetActor;
			while (!mActorsTree->GetNode(treeVisibleNodeActor))
			{
				treeVisibleNodeActor = treeVisibleNodeActor->GetParent();

				if (!treeVisibleNodeActor)
					return;

				parentsStack.Add(treeVisibleNodeActor);
			}

			for (int i = parentsStack.Count() - 1; i >= 0; i--)
			{
				auto node = mActorsTree->GetNode(parentsStack[i]);
				node->Expand();
			}
		}

		mActorsTree->ScrollTo(targetActor);
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

		// up panel
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

		// actors tree
		mActorsTree = o2UI.CreateWidget<UIActorsTree>("standard");
		mActorsTree->layout = UIWidgetLayout::BothStretch(2, 0, 0, 18);

		mActorsTree->onItemRBClick = Function<void(UITreeNode*, Actor*)>(this, &TreeWindow::OnTreeRBPressed);

		mWindow->AddChild(mActorsTree);

		// Context menu
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
			o2Scene.AddTag(String::Format("Tag_#%i", i + 1));

		// test actors
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
					Actor* childActor2 = mnew Actor({ mnew ImageComponent("ui/UI_Background.png"), mnew EditorTestComponent() });
					childActor2->name = String::Format("Sub Child actor #%i", k + 1);
					childActor2->transform.position = Vec2F(Math::Random(-500.0f, 500.0f), Math::Random(-500.0f, 500.0f));
					childActor->AddChild(childActor2);
				}
			}
		}

		mActorsTree->UpdateView();
	}

	void TreeWindow::PostInitializeWindow()
	{
		mActorsTree->AttachToSceneEvents();
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

		mActorsTree->UpdateView();
	}

	void TreeWindow::SearchActorsRecursive(Actor* actor, const String& searchStr)
	{
		if (actor->GetName().CountOf(searchStr) > 0)
			mSearchActors.Add(actor);

		for (auto child : actor->GetChilds())
			SearchActorsRecursive(child, searchStr);
	}

	void TreeWindow::OnTreeRBPressed(UITreeNode* node, Actor* actor)
	{
		mTreeContextMenu->Show();
	}

	void TreeWindow::CreateActor(Actor* newActor)
	{
		auto selectedActors = mActorsTree->GetSelectedActors();

		if (selectedActors.Count() > 0)
		{
			auto obj = selectedActors.Last();
			auto node = mActorsTree->GetNode(obj);

			Actor* parentActor = obj;
			parentActor->AddChild(newActor);

			node->UpdateView(true);

			auto parentChilds = parentActor->GetChilds();
			auto action = mnew CreateActorsAction({ newActor }, parentActor,
														parentChilds.Count() > 1 ? parentChilds[parentChilds.Count() - 2] : nullptr);
			o2EditorApplication.DoneAction(action);
		}
		else
		{
			mActorsTree->UpdateView();

			auto scereActors = o2Scene.GetRootActors();
			auto action = mnew CreateActorsAction({ newActor }, nullptr,
														scereActors.Count() > 1 ? scereActors[scereActors.Count() - 2] : nullptr);
			o2EditorApplication.DoneAction(action);
		}
	}

	void TreeWindow::OnContextCreateNewPressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		Actor* newActor = mnew Actor();
		newActor->name = "Empty";
		CreateActor(newActor);
	}

	void TreeWindow::OnContextCreateSprite()
	{
		if (!mActorsTree->IsFocused())
			return;

		Actor* newActor = mnew Actor({ mnew ImageComponent() });
		newActor->name = "Sprite";
		newActor->transform.size = Vec2F(10, 10);
		CreateActor(newActor);
	}

	void TreeWindow::OnContextCreateButton()
	{
		if (!mActorsTree->IsFocused())
			return;
	}

	void TreeWindow::OnContextCopyPressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		Vector<Actor*> selectedNodes = mActorsTree->GetSelectedObjects().Select<Actor*>([](auto x) { return (Actor*)(void*)x; });

		DataNode data;
		data = selectedNodes;

		WString clipboardData = data.SaveAsWString();

		Clipboard::SetText(clipboardData);
	}

	void TreeWindow::OnContextCutPressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		OnContextCopyPressed();
		OnContextDeletePressed();
	}

	void TreeWindow::OnContextPastePressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = mActorsTree->GetSelectedActors();

		Actor* parent = selectedActors.Count() > 0 ? selectedActors.Last() : nullptr;

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

		mActorsTree->UpdateView();
		mActorsTree->SetSelectedActors(actors);

		auto action = mnew CreateActorsAction(actors, parent, prevActor);
		o2EditorApplication.DoneAction(action);
	}

	void TreeWindow::OnContextDeletePressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = o2EditorSceneScreen.GetTopSelectedActors();
		o2EditorSceneScreen.ClearSelectionWithoutAction();

		auto action = mnew DeleteActorsAction(selectedActors);
		o2EditorApplication.DoneAction(action);

		for (auto actor : selectedActors)
			delete actor;

		mActorsTree->UpdateView();
	}

	void TreeWindow::OnContextDuplicatePressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = o2EditorSceneScreen.GetTopSelectedActors();

		for (auto actor : selectedActors)
		{
			Actor* copy = actor->Clone();
			copy->SetParent(actor->GetParent());
		}

		mActorsTree->UpdateView();
		mActorsTree->SetSelectedActors(selectedActors);
	}

	void TreeWindow::OnContextExpandAllPressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = mActorsTree->GetSelectedActors();

		for (auto actor : selectedActors)
		{
			auto node = mActorsTree->GetNode(actor);

			if (node)
				node->Expand();
		}
	}

	void TreeWindow::OnContextCollapseAllPressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = mActorsTree->GetSelectedActors();

		for (auto actor : selectedActors)
		{
			auto node = mActorsTree->GetNode(actor);

			if (node)
				node->Collapse();
		}
	}

	void TreeWindow::OnContextLockPressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = mActorsTree->GetSelectedActors();

		bool value = selectedActors.Count() > 0 ? !selectedActors.Last()->IsLocked() : true;
		auto action = mnew LockAction(selectedActors, value);
		o2EditorApplication.DoneAction(action);

		for (auto actor : selectedActors)
		{
			actor->SetLocked(value);

			auto node = mActorsTree->GetNode(actor);
			if (node)
				node->UpdateView();
		}
	}

	void TreeWindow::OnContextEnablePressed()
	{
		if (!mActorsTree->IsFocused())
			return;

		auto selectedActors = mActorsTree->GetSelectedActors();

		bool value = selectedActors.Count() > 0 ? !selectedActors.Last()->IsEnabled() : true;
		auto action = mnew LockAction(selectedActors, value);
		o2EditorApplication.DoneAction(action);

		for (auto actor : selectedActors)
		{
			actor->SetEnabled(value);

			auto node = mActorsTree->GetNode(actor);
			if (node)
				node->UpdateView();
		}
	}

	void TreeWindow::OnActorCreated(Actor* actor)
	{
		mActorsTree->UpdateView();
	}

	void TreeWindow::OnActorDestroyed(Actor* actor)
	{
		mActorsTree->UpdateView();
	}

}