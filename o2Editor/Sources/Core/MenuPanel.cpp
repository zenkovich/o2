#include "stdafx.h"
#include "MenuPanel.h"

#include "Application/Application.h"
#include "Assets/Assets.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Core/EditorApplication.h"
#include "Core/EditorConfig.h"
#include "Core/UIRoot.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Dialogs/CurveEditorDlg.h"
#include "LogWindow/LogWindow.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Scene.h"
#include "SceneWindow/SceneWindow.h"
#include "TreeWindow/SceneTree.h"
#include "TreeWindow/TreeWindow.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/HorizontalLayout.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/MenuPanel.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "UIStyle/EditorUIStyle.h"
#include "Utils/Math/Curve.h"

DECLARE_SINGLETON(Editor::MenuPanel);

namespace Editor
{
	MenuPanel::MenuPanel()
	{
		mMenuPanel = o2UI.CreateWidget<UIMenuPanel>();
		*mMenuPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
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

		mMenuPanel->AddItem("Edit/Transform/Move", [&]() {});
		mMenuPanel->AddItem("Edit/Transform/Rotate", [&]() {});
		mMenuPanel->AddItem("Edit/Transform/Scale", [&]() {});

		mMenuPanel->AddItem("Edit/---");

		mMenuPanel->AddToggleItem("Edit/Options/View editor UI tree", false,
								  [&](bool x) { o2EditorTree.GetSceneTree()->SetEditorWatching(x); });

		// VIEW
		mMenuPanel->AddItem("View/Show Tree", [&]() { OnShowTreePressed(); });
		mMenuPanel->AddItem("View/Show Scene", [&]() { OnShowScenePressed(); });
		mMenuPanel->AddItem("View/Show Assets", [&]() { OnShowAssetsPressed(); });
		mMenuPanel->AddItem("View/Show Properties", [&]() { OnShowPropertiesPressed(); });
		mMenuPanel->AddItem("View/Show Animation", [&]() { OnShowAnimationPressed(); });
		mMenuPanel->AddItem("View/Show Log", [&]() { OnShowLogPressed(); });
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
		mMenuPanel->AddItem("Debug/RebuildEditorUIManager", [&]() {
			EditorUIStyleBuilder builder;
			builder.RebuildEditorUIManager();
		});
		mMenuPanel->AddItem("Debug/Dump memory", [&]() { o2Memory.DumpInfo(); });
	}

	MenuPanel::~MenuPanel()
	{}

	UIWidget* MenuPanel::AddItem(const UIMenuPanel::Item& item)
	{
		return mMenuPanel->AddItem(item);
	}

	UIWidget* MenuPanel::AddItem(const WString& path, const Function<void()>& clickFunc /*= Function<void()>()*/,
								 const ImageAssetRef& icon /*= ImageAssetRef()*/, 
								 const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		return mMenuPanel->AddItem(path, clickFunc, icon, shortcut);
	}

	UIWidget* MenuPanel::InsertItem(const UIMenuPanel::Item& item, int position)
	{
		return mMenuPanel->InsertItem(item, position);
	}

	void MenuPanel::AddItems(Vector<UIMenuPanel::Item> items)
	{
		mMenuPanel->AddItems(items);
	}

	void MenuPanel::InsertItems(Vector<UIMenuPanel::Item> items, int position)
	{
		mMenuPanel->InsertItems(items, position);
	}

	UIMenuPanel::Item MenuPanel::GetItem(int position)
	{
		return mMenuPanel->GetItem(position);
	}

	Vector<UIMenuPanel::Item> MenuPanel::GetItems() const
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
			*wnd->layout = UIWidgetLayout::Based(BaseCorner::Center, Vec2F(300, 200));

			auto verLayout = o2UI.CreateVerLayout();
			wnd->AddChild(verLayout);
			*verLayout->layout = UIWidgetLayout::BothStretch();
			verLayout->baseCorner = BaseCorner::Top;

			auto text = o2UI.CreateLabel("Current scene was modified and not saved. Do you want to save it?");
			text->horOverflow = UILabel::HorOverflow::Wrap;
			verLayout->AddChild(text);

			auto horLayout = o2UI.CreateHorLayout();
			verLayout->AddChild(horLayout);

			*horLayout->layout = UIWidgetLayout::BothStretch();
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
		auto treeWindow = o2EditorWindows.GetWindow<TreeWindow>();
		if (treeWindow)
			treeWindow->Show();
	}

	void MenuPanel::OnShowScenePressed()
	{
		auto sceneWindow = o2EditorWindows.GetWindow<SceneWindow>();
		if (sceneWindow)
			sceneWindow->Show();
	}

	void MenuPanel::OnShowAssetsPressed()
	{
		auto sceneWindow = o2EditorWindows.GetWindow<AssetsWindow>();
		if (sceneWindow)
			sceneWindow->Show();
	}

	void MenuPanel::OnShowPropertiesPressed()
	{
		auto sceneWindow = o2EditorWindows.GetWindow<PropertiesWindow>();
		if (sceneWindow)
			sceneWindow->Show();
	}

	void MenuPanel::OnShowAnimationPressed()
	{

	}

	void MenuPanel::OnShowLogPressed()
	{
		auto sceneWindow = o2EditorWindows.GetWindow<LogWindow>();
		if (sceneWindow)
			sceneWindow->Show();
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

	String MenuPanel::GetOpenFileNameDialog(const String& title, const Dictionary<String, String>& extensions)
	{
		char szFile[MAX_PATH];
		char oldDir[MAX_PATH];

		GetCurrentDirectory(MAX_PATH, oldDir);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = o2Application.GetWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		int filterStrSize = extensions.Sum<int>([](auto x) { return x.Key().Length() + x.Value().Length() + 2; }) + 1;
		char* filterStr = new char[filterStrSize];
		filterStr[filterStrSize - 1] = '\0';
		ofn.lpstrFilter = filterStr;

		int i = 0;
		for (auto x : extensions)
		{
			memcpy(filterStr + i, x.Key().Data(), x.Key().Length() + 1);
			i += x.Key().Length() + 1;

			memcpy(filterStr + i, x.Value().Data(), x.Value().Length() + 1);
			i += x.Value().Length() + 1;
		}

		if (GetOpenFileName(&ofn)==TRUE)
		{
			SetCurrentDirectory(oldDir);
			return ofn.lpstrFile;
		}

		return "";
	}

	String MenuPanel::GetSaveFileNameDialog(const String& title, const Dictionary<String, String>& extensions)
	{
		char szFile[MAX_PATH];
		char oldDir[MAX_PATH];

		GetCurrentDirectory(MAX_PATH, oldDir);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = o2Application.GetWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		int filterStrSize = extensions.Sum<int>([](auto x) { return x.Key().Length() + x.Value().Length() + 2; }) + 1;
		char* filterStr = new char[filterStrSize];
		filterStr[filterStrSize - 1] = '\0';
		ofn.lpstrFilter = filterStr;

		int i = 0;
		for (auto x : extensions)
		{
			memcpy(filterStr + i, x.Key().Data(), x.Key().Length() + 1);
			i += x.Key().Length() + 1;

			memcpy(filterStr + i, x.Value().Data(), x.Value().Length() + 1);
			i += x.Value().Length() + 1;
		}

		if (GetSaveFileName(&ofn) == TRUE)
		{
			SetCurrentDirectory(oldDir);
			return ofn.lpstrFile;
		}

		return "";
	}

}
