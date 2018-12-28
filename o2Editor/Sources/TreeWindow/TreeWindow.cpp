#include "stdafx.h"
#include "TreeWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Core/Actions/Create.h"
#include "Core/Actions/Delete.h"
#include "Core/Actions/Enable.h"
#include "Core/Actions/Lock.h"
#include "Core/Actions/Reparent.h"
#include "Core/EditorApplication.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Actor.h"
#include "Scene/Components/EditorTestComponent.h"
#include "Scene/Components/ImageComponent.h"
#include "Scene/Components/ParticlesEmitterComponent.h"
#include "Scene/Scene.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/SceneTree.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Editor/SceneEditableObject.h"
#include "Utils/System/Clipboard.h"

DECLARE_SINGLETON(Editor::TreeWindow);

namespace Editor
{
	TreeWindow::TreeWindow()
	{
		InitializeWindow();
	}

	TreeWindow::TreeWindow(const TreeWindow& other) :
		IEditorWindow(other)
	{
		InitializeWindow();
	}

	TreeWindow::~TreeWindow()
	{}

	UISceneTree* TreeWindow::GetSceneTree() const
	{
		return mSceneTree;
	}

	void TreeWindow::HightlightObjectTreeNode(SceneEditableObject* targetObject)
	{
		mSceneTree->ScrollToAndHightlight(targetObject);
	}

	void TreeWindow::SetWidgetsLayersVisible(bool visible)
	{
		UIWidget::isEditorLayersVisible = visible;
		mSceneTree->UpdateNodesView();
	}

	bool TreeWindow::IsWidgetsLayersVisible() const
	{
		return UIWidget::isEditorLayersVisible;
	}

	void TreeWindow::SetWidgetsInternalChildrenVisible(bool visible)
	{
		UIWidget::isEditorInternalChildrenVisible = visible;
		mSceneTree->UpdateNodesView();
	}

	bool TreeWindow::IsWidgetsInternalChildrenVisible() const
	{
		return UIWidget::isEditorInternalChildrenVisible;
	}

	void TreeWindow::InitializeWindow()
	{
		mWindow->caption = "Tree";
		mWindow->name = "tree window";
		mWindow->SetIcon(mnew Sprite("ui/UI_tree_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1)));
		mWindow->SetViewLayout(Layout::BothStretch(0, -2, 0, 18));

		InitializeTopPanel();
		InitializeSceneTree();
		InitializeContextMenu();
		InitializeTestScene();

		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::InitializeTestScene()
	{
		for (int i = 0; i < 10; i++)
			o2Scene.AddTag(String::Format("Tag_#%i", i + 1));

		// test actors
		for (int i = 0; i < 2; i++)
		{
			Actor* actor = mnew Actor(ActorCreateMode::InScene);
			actor->name = String::Format("Actor #%i", i + 1);
			actor->layer = o2Scene.AddLayer(String::Format("Layer #%i", i + 1));

			for (int j = 0; j < 1; j++)
			{
				Actor* childActor = mnew Actor(ActorCreateMode::InScene);
				childActor->name = String::Format("%i Child actor #%i", i + 1, j + 1);
				actor->AddChild(childActor);

				for (int k = 0; k < 1; k++)
				{
					Actor* childActor2 = mnew Actor({ mnew ImageComponent("ui/UI_Background.png"),
													  mnew EditorTestComponent(),
													  mnew ParticlesEmitterComponent() }, ActorCreateMode::InScene);
					childActor2->name = String::Format("%i %i Sub Child actor #%i", i + 1, j + 1, k + 1);
					//childActor2->transform->position = Vec2F(Math::Random(-500.0f, 500.0f), Math::Random(-500.0f, 500.0f));
					childActor2->transform->position = Vec2F((float)(k*100), (float)((i*2 + j)*100));
					childActor->AddChild(childActor2);
				}
			}
		}
	}

	void TreeWindow::InitializeSceneTree()
	{
		mSceneTree = o2UI.CreateWidget<UISceneTree>("standard");
		*mSceneTree->layout = UIWidgetLayout::BothStretch(2, 0, 0, 18);

		mSceneTree->onNodeRightButtonClicked = THIS_FUNC(OnTreeRBPressed);

		mWindow->AddChild(mSceneTree);
	}

	void TreeWindow::InitializeContextMenu()
	{
		// Context menu
		mTreeContextMenu = o2UI.CreateWidget<UIContextMenu>("standard");
		mTreeContextMenu->AddItem("Create empty actor", [&]() { OnContextCreateNewPressed(); }, ImageAssetRef(),
								  ShortcutKeys('N', true, true));

		mTreeContextMenu->AddItem("Create UI/Widget", [&]() { CreateObject<UIWidget>("Widget"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Empty layer", [&]() { CreateObject<UIWidgetLayer>("Layer"); });
		mTreeContextMenu->AddItem("Create UI/Sprite layer", [&]()
		{
			UIWidgetLayer* newLayer = mnew UIWidgetLayer();
			newLayer->drawable = mnew Sprite();
			newLayer->name = "sprite";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create UI/Text layer", [&]()
		{
			UIWidgetLayer* newLayer = mnew UIWidgetLayer();
			newLayer->drawable = mnew Text();
			newLayer->name = "sprite";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Horizontal layout", [&]() { CreateObject<UIHorizontalLayout>("Horizontal layout"); });
		mTreeContextMenu->AddItem("Create UI/Vertical layout", [&]() { CreateObject<UIVerticalLayout>("Vertical layout"); });
		mTreeContextMenu->AddItem("Create UI/Grid layout", [&]() { CreateObject<UIGridLayout>("Grid layout"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Scroll area", [&]() { CreateObject<UIScrollArea>("Scroll area"); });
		mTreeContextMenu->AddItem("Create UI/Button", [&]() { CreateObject<UIButton>("Button"); });
		mTreeContextMenu->AddItem("Create UI/Label", [&]() { CreateObject<UILabel>("Label"); });
		mTreeContextMenu->AddItem("Create UI/Image", [&]() { CreateObject<UIImage>("Image"); });
		mTreeContextMenu->AddItem("Create UI/Edit box", [&]() { CreateObject<UIEditBox>("Edit box"); });
		mTreeContextMenu->AddItem("Create UI/Toggle", [&]() { CreateObject<UIToggle>("Toggle"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Horizontal progress", [&]() { CreateObject<UIHorizontalProgress>("Progress"); });
		mTreeContextMenu->AddItem("Create UI/Vertical progress", [&]() { CreateObject<UIVerticalProgress>("Progress"); });
		mTreeContextMenu->AddItem("Create UI/Horizontal scroll bar", [&]() { CreateObject<UIHorizontalScrollBar>("Scroll bar"); });
		mTreeContextMenu->AddItem("Create UI/Vertical scroll bar", [&]() { CreateObject<UIVerticalScrollBar>("Scroll bar"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Context menu", [&]() { CreateObject<UIContextMenu>("Context menu"); });
		mTreeContextMenu->AddItem("Create UI/Menu panel", [&]() { CreateObject<UIMenuPanel>("Menu panel"); });

		mTreeContextMenu->AddItem("Create UI/---");
		mTreeContextMenu->AddItem("Create UI/Custom drop down", [&]() { CreateObject<UICustomDropDown>("Dropdown"); });
		mTreeContextMenu->AddItem("Create UI/Custom list", [&]() { CreateObject<UICustomList>("List"); });
		mTreeContextMenu->AddItem("Create UI/Drop down", [&]() { CreateObject<UIDropDown>("Dropdown"); });
		mTreeContextMenu->AddItem("Create UI/List", [&]() { CreateObject<UIList>("List"); });
		mTreeContextMenu->AddItem("Create UI/Long list", [&]() { CreateObject<UILongList>("List"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Spoiler", [&]() { CreateObject<UISpoiler>("Spoiler"); });
		mTreeContextMenu->AddItem("Create UI/Tree", [&]() { CreateObject<UITree>("Tree"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Window", [&]() { CreateObject<UIWindow>("Window"); });

		InitializeOtherCreateMenu();

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mTreeContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, ImageAssetRef(), ShortcutKeys('X', true));
		mTreeContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mTreeContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));
		mTreeContextMenu->AddItem("Duplicate", [&]() { OnContextDuplicatePressed(); }, ImageAssetRef(), ShortcutKeys('D', true));

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Collapse all", [&]() { OnContextCollapseAllPressed(); });
		mTreeContextMenu->AddItem("Expand all", [&]() { OnContextExpandAllPressed(); });

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Enable\\disable", [&]() { OnContextEnablePressed(); }, ImageAssetRef(), ShortcutKeys('L', true));
		mTreeContextMenu->AddItem("Lock\\unlock", [&]() { OnContextLockPressed(); }, ImageAssetRef(), ShortcutKeys('O', true));

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Save prototype", [&]() { OnContextCopyPressed(); });
		mTreeContextMenu->AddItem("Reset to prototype", [&]() { OnContextCopyPressed(); });
		mTreeContextMenu->AddItem("Break link to prototype", [&]() { OnContextCopyPressed(); });

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddToggleItem("View widgets layers", true, THIS_FUNC(OnViewLayersToggled));
		mTreeContextMenu->AddToggleItem("View widgets internal children", true, THIS_FUNC(OnViewInternalChildrenToggled));

		mTreeContextMenu->AddItem("---");

		InitUIStyleCreateMenu();

		mWindow->AddChild(mTreeContextMenu);

		mSceneTree->onFocused = [&]() { mTreeContextMenu->SetItemsMaxPriority(); };
		mSceneTree->onUnfocused = [&]() { mTreeContextMenu->SetItemsMinPriority(); };
	}

	void TreeWindow::InitializeOtherCreateMenu()
	{
		auto subTypes = TypeOf(Actor).GetDerivedTypes();

		for (auto subType : subTypes)
		{
			String path = subType->GetName().ReplacedAll("o2::", "").ReplacedAll("::", "/");
			mTreeContextMenu->AddItem("Create other/" + path, [=]() {
				auto objectType = dynamic_cast<const ObjectType*>(subType);
				Actor* newActor = dynamic_cast<Actor*>(objectType->DynamicCastToIObject(subType->CreateSample()));
				newActor->name = path;
				OnCreateObject(newActor);
			});
		}
	}

	void TreeWindow::InitUIStyleCreateMenu()
	{
		auto styleWidgets = o2UI.GetWidgetStyles();

		for (auto styleWidget : styleWidgets)
		{
			auto path = styleWidget->GetType().GetName() + "/" + styleWidget->GetName();
			path.ReplaceAll("o2::", "");
			path.ReplaceAll("::", "/");

			mTreeContextMenu->AddItem(String("Create UI style/") + path, [=]()
			{
				UIWidget* newWidget = styleWidget->CloneAs<UIWidget>();
				newWidget->SetEnableForcible(true);
				OnCreateObject(newWidget);
			});
		}
	}

	void TreeWindow::InitUILayersCreateMenu()
	{
		mTreeContextMenu->AddItem("Create/UI layer/Empty", [&]()
		{
			UIWidgetLayer* newLayer = mnew UIWidgetLayer();
			newLayer->name = "empty";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create/UI layer/Sprite", [&]()
		{
			UIWidgetLayer* newLayer = mnew UIWidgetLayer();
			newLayer->drawable = mnew Sprite();
			newLayer->name = "sprite";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create/UI layer/Text", [&]()
		{
			UIWidgetLayer* newLayer = mnew UIWidgetLayer();
			newLayer->drawable = mnew Text();
			newLayer->name = "text";
			OnCreateObject(newLayer);
		});
	}

	void TreeWindow::PostInitializeWindow()
	{
		mSceneTree->AttachToSceneEvents();
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
			mSearchObjects.Clear();

			for (auto actor : o2Scene.GetRootActors())
				SearchObjectsRecursive(actor, (String)searchStr);
		}

		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::SearchObjectsRecursive(SceneEditableObject* object, const String& searchStr)
	{
		if (object->GetName().CountOf(searchStr) > 0)
			mSearchObjects.Add(object);

		for (auto child : object->GetEditablesChildren())
			SearchObjectsRecursive(child, searchStr);
	}

	void TreeWindow::OnTreeRBPressed(UITreeNode* node)
	{
		bool canCreateUILayers = false;		
		if (node) 
		{
			SceneEditableObject* object = node->GetObject();
			canCreateUILayers = dynamic_cast<UIWidgetLayer*>(object) != nullptr ||
				dynamic_cast<UIWidget::LayersEditable*>(object) != nullptr ||
				dynamic_cast<UIWidget*>(object) != nullptr;
		}

		mTreeContextMenu->FindItemWidget("Create UI/Empty layer")->SetEnabled(canCreateUILayers);
		mTreeContextMenu->FindItemWidget("Create UI/Sprite layer")->SetEnabled(canCreateUILayers);
		mTreeContextMenu->FindItemWidget("Create UI/Text layer")->SetEnabled(canCreateUILayers);

		mTreeContextMenu->Show();
	}

	void TreeWindow::OnCreateObject(SceneEditableObject* newObject)
	{
		auto selectedObjects = mSceneTree->GetSelectedObjects();

		if (newObject->IsSupportsLayout())
		{
			if (!selectedObjects.IsEmpty())
				newObject->SetLayout(Layout::BothStretch());
			else
				newObject->SetLayout(Layout::Based(BaseCorner::Center, Vec2F(100, 100)));
		}
		else
			newObject->SetTransform(Basis(Vec2F(), Vec2F(100, 0), Vec2F(0, 100)));

		if (selectedObjects.Count() > 0)
		{
			auto obj = selectedObjects.Last();
			auto node = mSceneTree->GetNode(obj);

			SceneEditableObject* parentObject = obj;
			parentObject->AddEditableChild(newObject);

			auto parentChilds = parentObject->GetEditablesChildren();
			auto action = mnew CreateAction({ newObject }, parentObject,
											parentChilds.Count() > 1 ? parentChilds[parentChilds.Count() - 2] : nullptr);

			o2EditorApplication.DoneAction(action);
		}
		else
		{
			auto sceneObjects = o2Scene.GetRootEditableObjects();
			auto action = mnew CreateAction({ newObject }, nullptr,
											sceneObjects.Count() > 1 ? sceneObjects[sceneObjects.Count() - 2] : nullptr);

			o2EditorApplication.DoneAction(action);
		}

		newObject->UpdateSelfTransform();

		mSceneTree->SelectAndHightlightObject(newObject);
		mSceneTree->ScrollTo(newObject);
	}

	void TreeWindow::OnContextCreateNewPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		Actor* newActor = mnew Actor();
		newActor->name = "Empty";
		OnCreateObject(newActor);
	}

	void TreeWindow::OnContextCreateSprite()
	{
		if (!mSceneTree->IsFocused())
			return;

		Actor* newActor = mnew Actor({ mnew ImageComponent() });
		newActor->name = "Sprite";
		newActor->transform->size = Vec2F(10, 10);
		OnCreateObject(newActor);
	}

	void TreeWindow::OnContextCreateButton()
	{
		if (!mSceneTree->IsFocused())
			return;
	}

	void TreeWindow::OnContextCopyPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		Vector<SceneEditableObject*> selectedObjects =
			mSceneTree->GetSelectedObjects().Select<SceneEditableObject*>(
				[](auto x) { return dynamic_cast<SceneEditableObject*>(x); });

		DataNode data;
		data.SetValueRaw(selectedObjects);

		WString clipboardData = data.SaveAsWString();

		Clipboard::SetText(clipboardData);
	}

	void TreeWindow::OnContextCutPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		OnContextCopyPressed();
		OnContextDeletePressed();
	}

	void TreeWindow::OnContextPastePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		SceneEditableObject* parent = selectedObjects.Count() > 0 ? selectedObjects.Last() : nullptr;
		auto parentChilds = parent ? parent->GetEditablesChildren() : o2Scene.GetRootEditableObjects();
		SceneEditableObject* prevObject = parentChilds.Count() > 0 ? parentChilds.Last() : nullptr;

		WString clipboardData = Clipboard::GetText();
		DataNode data;
		data.LoadFromData(clipboardData);

		Vector<SceneEditableObject*> objects;
		data.GetValueRaw(objects);

		for (auto object : objects)
			object->GenerateNewID();

		for (auto object : objects)
			object->SetEditableParent(parent);

		mSceneTree->UpdateNodesView();

		auto action = mnew CreateAction(objects, parent, prevObject);
		o2EditorApplication.DoneAction(action);

		mSceneTree->SetSelectedObjects(objects);
	}

	void TreeWindow::OnContextDeletePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		o2EditorPropertiesWindow.SetTarget(nullptr);

		auto selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();
		o2EditorSceneScreen.ClearSelectionWithoutAction();

		auto action = mnew DeleteAction(selectedObjects);
		o2EditorApplication.DoneAction(action);

		for (auto actor : selectedObjects)
			delete actor;

		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::OnContextDuplicatePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();

		for (auto object : selectedObjects)
		{
			SceneEditableObject* copy = object->CloneAs<SceneEditableObject>();
			copy->SetEditableParent(object->GetEditableParent());
		}

		mSceneTree->UpdateNodesView();
		mSceneTree->SetSelectedObjects(selectedObjects);
	}

	void TreeWindow::OnContextExpandAllPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		for (auto object : selectedObjects)
		{
			auto node = mSceneTree->GetNode(object);

			if (node)
				node->Expand();
		}
	}

	void TreeWindow::OnContextCollapseAllPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObject = mSceneTree->GetSelectedObjects();

		for (auto object : selectedObject)
		{
			auto node = mSceneTree->GetNode(object);

			if (node)
				node->Collapse();
		}
	}

	void TreeWindow::OnContextLockPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		bool value = selectedObjects.Count() > 0 ? !selectedObjects.Last()->IsLocked() : true;
		auto action = mnew LockAction(selectedObjects, value);
		o2EditorApplication.DoneAction(action);

		for (auto object : selectedObjects)
		{
			object->SetLocked(value);

			// 			auto node = mActorsTree->GetNode(actor);
			// 			if (node)
			// 				node->UpdateView();
		}
	}

	void TreeWindow::OnContextEnablePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		bool value = selectedObjects.Count() > 0 ? !selectedObjects.Last()->IsEnabled() : true;
		auto action = mnew LockAction(selectedObjects, value);
		o2EditorApplication.DoneAction(action);

		for (auto object : selectedObjects)
		{
			object->SetEnabled(value);

			// 			auto node = mActorsTree->GetNode(actor);
			// 			if (node)
			// 				node->UpdateView();
		}
	}

	void TreeWindow::OnViewLayersToggled(bool view)
	{
		SetWidgetsLayersVisible(view);
	}

	void TreeWindow::OnViewInternalChildrenToggled(bool view)
	{
		SetWidgetsInternalChildrenVisible(view);
	}

	void TreeWindow::OnActorCreated(SceneEditableObject* actor)
	{
		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::OnActorDestroyed(SceneEditableObject* actor)
	{
		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::InitializeTopPanel()
	{
		UIWidget* upPanel = mnew UIWidget();
		upPanel->name = "up panel";
		*upPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		UIButton* searchButton = o2UI.CreateWidget<UIButton>("search");
		*searchButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		searchButton->onClick += THIS_FUNC(OnSearchPressed);
		upPanel->AddChild(searchButton);

		mListTreeToggle = o2UI.CreateWidget<UIToggle>("list-tree");
		*mListTreeToggle->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
		mListTreeToggle->onToggle += THIS_FUNC(OnListTreeToggled);
		upPanel->AddChild(mListTreeToggle);

		mSearchEditBox = o2UI.CreateWidget<UIEditBox>("backless");
		*mSearchEditBox->layout = UIWidgetLayout::BothStretch(19, 2, 21, -2);
		mSearchEditBox->onChanged += THIS_FUNC(OnSearchEdited);
		upPanel->AddChild(mSearchEditBox);

		mWindow->AddChild(upPanel);
	}

}

DECLARE_CLASS(Editor::TreeWindow);
