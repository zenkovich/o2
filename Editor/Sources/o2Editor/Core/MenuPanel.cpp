#include "o2Editor/stdafx.h"
#include "MenuPanel.h"

#include "o2/Application/Application.h"
#include "o2/Assets/Assets.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Math/Curve.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Dialogs/CurveEditorDlg.h"
#include "o2Editor/Core/Dialogs/System/OpenSaveDialog.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Core/Properties/Basic/ObjectPtrProperty.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/UIStyle/EditorUIStyle.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/GameWindow/GameWindow.h"
#include "o2Editor/LogWindow/LogWindow.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneWindow.h"
#include "o2Editor/TreeWindow/SceneTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

DECLARE_SINGLETON(Editor::MenuPanel);

namespace Editor
{
	MenuPanel::MenuPanel()
	{
		mMenuPanel = o2UI.CreateWidget<o2::MenuPanel>();
		*mMenuPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		EditorUIRoot.AddWidget(mMenuPanel);

		// FILE
		mMenuPanel->AddItem("File/New scene", [&]() { OnNewScenePressed(); }, ImageAssetRef(), ShortcutKeys('N', true));
		mMenuPanel->AddItem("File/Open scene", [&]() { OnOpenScenePressed(); }, ImageAssetRef(), ShortcutKeys('O', true));
		mMenuPanel->AddItem("File/Save scene", [&]() { OnSaveScenePressed(); }, ImageAssetRef(), ShortcutKeys('S', true));
		mMenuPanel->AddItem("File/Save scene as ...", [&]() { OnSaveSceneAsPressed(); }, ImageAssetRef(), ShortcutKeys('S', true, false, true));

		mMenuPanel->AddItem("File/---");

		mMenuPanel->AddItem("File/New project", [&]() { OnNewProjectPressed(); });
		mMenuPanel->AddItem("File/Open project", [&]() { OnOpenProjectPressed(); });
		mMenuPanel->AddItem("File/Save project", [&]() { OnSaveProjectPressed(); }, ImageAssetRef(), ShortcutKeys('S', true, true));

		mMenuPanel->AddItem("File/---");

		mMenuPanel->AddItem("File/Exit", [&]() { OnExitPressed(); });

		// EDIT
		mMenuPanel->AddItem("Edit/Undo", [&]() { OnUndoPressed(); }, ImageAssetRef(), ShortcutKeys('Z', true));
		mMenuPanel->AddItem("Edit/Redo", [&]() { OnRedoPressed(); }, ImageAssetRef(), ShortcutKeys('Z', true, true));

		mMenuPanel->AddItem("Edit/---");

		mMenuPanel->AddItem("Edit/Copy", [&]() {}, ImageAssetRef(), ShortcutKeys('C', true));
		mMenuPanel->AddItem("Edit/Cut", [&]() {}, ImageAssetRef(), ShortcutKeys('X', true));
		mMenuPanel->AddItem("Edit/Paste", [&]() {}, ImageAssetRef(), ShortcutKeys('V', true));
		mMenuPanel->AddItem("Edit/Delete", [&]() {}, ImageAssetRef(), ShortcutKeys(VK_DELETE));

		mMenuPanel->AddItem("Edit/---");

		mMenuPanel->AddItem("Edit/Project/Physics", [&]() { o2EditorPropertiesWindow.SetTarget(&o2Config.physics); });

		// VIEW
		mMenuPanel->AddItem("View/Show Tree", [&]() { OnShowTreePressed(); });
		mMenuPanel->AddItem("View/Show Scene", [&]() { OnShowScenePressed(); });
		mMenuPanel->AddItem("View/Show Assets", [&]() { OnShowAssetsPressed(); });
		mMenuPanel->AddItem("View/Show Properties", [&]() { OnShowPropertiesPressed(); });
		mMenuPanel->AddItem("View/Show Animation", [&]() { OnShowAnimationPressed(); });
		mMenuPanel->AddItem("View/Show Log", [&]() { OnShowLogPressed(); });
		mMenuPanel->AddItem("View/Show Game", [&]() { OnShowGamePressed(); });
		mMenuPanel->AddItem("View/---");
		mMenuPanel->AddItem("View/Reset layout", [&]() { OnResetLayoutPressed(); });

		// BUILD
		mMenuPanel->AddItem("Build/Run", [&]() { OnRunPressed(); }, ImageAssetRef(), ShortcutKeys(VK_F5));
		mMenuPanel->AddItem("Build/Build & run", [&]() { OnBuildAndRunPressed(); }, ImageAssetRef(), ShortcutKeys('R', true));
		mMenuPanel->AddItem("Build/Build", [&]() { OnBuildPressed(); }, ImageAssetRef(), ShortcutKeys('R', true, true));

		// HELP
		mMenuPanel->AddItem("Help/About", [&]() { OnAboutPressed(); });
		mMenuPanel->AddItem("Help/Documentation", [&]() { OnDocumentationPressed(); });

		// DEBUG
		mMenuPanel->AddItem("Debug/Curve editor test", [&]() { OnCurveEditorTestPressed(); });
		mMenuPanel->AddItem("Debug/Save layout as default", [&]() { OnSaveDefaultLayoutPressed(); });
		mMenuPanel->AddItem("Debug/Update assets", [&]() { o2Assets.RebuildAssets(); });
		mMenuPanel->AddItem("Debug/Add property", [&]() { o2UI.CreateWidget<ObjectPtrProperty>("with caption")->GetRemoveButton(); });

		mMenuPanel->AddToggleItem("Debug/View editor UI tree", false, [&](bool x) { o2EditorTree.GetSceneTree()->SetEditorWatching(x); });
		
		mMenuPanel->AddItem("Debug/RebuildEditorUIManager", [&]() {
			EditorUIStyleBuilder builder;
			builder.RebuildEditorUIManager(false);
		});

		mMenuPanel->AddItem("Debug/Dump memory", [&]() { o2Memory.DumpInfo(); });
	}

	MenuPanel::~MenuPanel()
	{}

	Widget* MenuPanel::AddItem(const o2::MenuPanel::Item& item)
	{
		return mMenuPanel->AddItem(item);
	}

	void MenuPanel::AddItem(const WString& path, const Function<void()>& clickFunc /*= Function<void()>()*/,
								 const ImageAssetRef& icon /*= ImageAssetRef()*/, 
								 const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		mMenuPanel->AddItem(path, clickFunc, icon, shortcut);
	}

	void MenuPanel::InsertItem(const o2::MenuPanel::Item& item, int position)
	{
		mMenuPanel->InsertItem(item, position);
	}

	void MenuPanel::AddItems(Vector<o2::MenuPanel::Item> items)
	{
		mMenuPanel->AddItems(items);
	}

	void MenuPanel::InsertItems(Vector<o2::MenuPanel::Item> items, int position)
	{
		mMenuPanel->InsertItems(items, position);
	}

	o2::MenuPanel::Item MenuPanel::GetItem(int position)
	{
		return mMenuPanel->GetItem(position);
	}

	Vector<o2::MenuPanel::Item> MenuPanel::GetItems() const
	{
		return mMenuPanel->GetItems();
	}

	void MenuPanel::RemoveItem(int position)
	{
		return mMenuPanel->RemoveItem(position);
	}

	void MenuPanel::RemoveItem(const WString& path)
	{
		return mMenuPanel->RemoveItem(path);
	}

	void MenuPanel::OnNewScenePressed()
	{
		if (o2EditorApplication.IsSceneChanged())
			o2Debug.Log("Check saving scene");

		o2EditorApplication.MakeNewScene();
	}

	void MenuPanel::OnOpenScenePressed()
	{
		if (o2EditorApplication.IsSceneChanged())
		{
			auto wnd = EditorUIRoot.AddWidget(o2UI.CreateWindow("Save scene?"));
			*wnd->layout = WidgetLayout::Based(BaseCorner::Center, Vec2F(300, 200));

			auto verLayout = o2UI.CreateVerLayout();
			wnd->AddChild(verLayout);
			*verLayout->layout = WidgetLayout::BothStretch();
			verLayout->baseCorner = BaseCorner::Top;

			auto text = o2UI.CreateLabel("Current scene was modified and not saved. Do you want to save it?");
			text->horOverflow = Label::HorOverflow::Wrap;
			verLayout->AddChild(text);

			auto horLayout = o2UI.CreateHorLayout();
			verLayout->AddChild(horLayout);

			*horLayout->layout = WidgetLayout::BothStretch();
			horLayout->border = BorderF(20, 20, 20, 20);
			horLayout->spacing = 20;
			horLayout->AddChild(o2UI.CreateButton("Save"));
			horLayout->AddChild(o2UI.CreateButton("Don't save"));
			horLayout->AddChild(o2UI.CreateButton("Cancel"));

			o2Debug.Log("Check saving scene");
			return;
		}

		String fileName = GetOpenFileNameDialog("Load scene", { { "o2 Scene", "*.scn" }, { "All", "*.*" } });

		if (fileName.IsEmpty())
			return;


		o2EditorApplication.LoadScene(fileName);
	}

	void MenuPanel::OnSaveScenePressed()
	{
		if (o2EditorApplication.GetLoadedSceneName().IsEmpty())
			OnSaveSceneAsPressed();
		else
			o2EditorApplication.SaveScene(o2EditorApplication.GetLoadedSceneName());
	}

	void MenuPanel::OnSaveSceneAsPressed()
	{
		String fileName = GetSaveFileNameDialog("Save scene", { { "o2 Scene", "*.scn" },{ "All", "*.*" } });

		if (fileName.IsEmpty())
			return;

		if (!fileName.EndsWith(".scn"))
			fileName += ".scn";

		o2EditorApplication.SaveScene(fileName);
	}

	void MenuPanel::OnNewProjectPressed()
	{

	}

	void MenuPanel::OnOpenProjectPressed()
	{

	}

	void MenuPanel::OnSaveProjectPressed()
	{

	}

	void MenuPanel::OnExitPressed()
	{
		o2Application.Shutdown();
	}

	void MenuPanel::OnUndoPressed()
	{
		o2EditorApplication.UndoAction();
	}

	void MenuPanel::OnRedoPressed()
	{
		o2EditorApplication.RedoAction();
	}

	void MenuPanel::OnShowTreePressed()
	{
		auto window = o2EditorWindows.GetWindow<TreeWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnShowScenePressed()
	{
		auto window = o2EditorWindows.GetWindow<SceneWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnShowAssetsPressed()
	{
		auto window = o2EditorWindows.GetWindow<AssetsWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnShowPropertiesPressed()
	{
		auto window = o2EditorWindows.GetWindow<PropertiesWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnShowAnimationPressed()
	{
		auto window = o2EditorWindows.GetWindow<AnimationWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnShowLogPressed()
	{
		auto window = o2EditorWindows.GetWindow<LogWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnShowGamePressed()
	{
		auto window = o2EditorWindows.GetWindow<GameWindow>();
		if (window)
			window->Show();
	}

	void MenuPanel::OnResetLayoutPressed()
	{
		o2EditorWindows.SetDefaultWindowsLayout();
	}

	void MenuPanel::OnRunPressed()
	{

	}

	void MenuPanel::OnBuildAndRunPressed()
	{

	}

	void MenuPanel::OnBuildPressed()
	{

	}

	void MenuPanel::OnAboutPressed()
	{

	}

	void MenuPanel::OnDocumentationPressed()
	{

	}

	void MenuPanel::OnSaveDefaultLayoutPressed()
	{
		o2EditorConfig.mGlobalConfig.mDefaultLayout = o2EditorWindows.GetWindowsLayout();
		o2Debug.Log("Default windows layout saved!");
	}

	void MenuPanel::OnCurveEditorTestPressed()
	{
		CurveEditorDlg::Show(Function<void()>());

		int testCurves = 50;
		int testKeys = 50;
		for (int i = 0; i < testCurves; i++)
		{
			Curve* curve = mnew Curve();

			for (int j = 0; j < testKeys; j++)
			{
				curve->AppendKey(Math::Random(0.1f, 2.0f), Math::Random(-1.0f, 1.0f), 1.0f);
			}

			CurveEditorDlg::AddEditingCurve("test" + (String)i, curve);
		}
	}
}
