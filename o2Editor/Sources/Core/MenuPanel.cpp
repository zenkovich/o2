#include "MenuPanel.h"

#include "Application/Application.h"
#include "Core/EditorConfig.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "LogWindow/LogWindow.h"
#include "SceneWindow/SceneWindow.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/MenuPanel.h"
#include "UI/UIManager.h"

DECLARE_SINGLETON(MenuPanel);

MenuPanel::MenuPanel()
{
	mMenuPanel = o2UI.CreateWidget<UIMenuPanel>();
	mMenuPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
	o2UI.AddWidget(mMenuPanel);

	// FILE
	mMenuPanel->AddItem("File/New scene", [&]() { OnNewScenePressed(); }, nullptr, ShortcutKeys('N', true));
	mMenuPanel->AddItem("File/Open scene", [&]() { OnOpenScenePressed(); }, nullptr, ShortcutKeys('O', true));
	mMenuPanel->AddItem("File/Save scene", [&]() { OnSaveScenePressed(); }, nullptr, ShortcutKeys('S', true));
	mMenuPanel->AddItem("File/Save scene as ...", [&]() { OnSaveSceneAsPressed(); }, nullptr, ShortcutKeys('S', true, false, true));

	mMenuPanel->AddItem("File/---");

	mMenuPanel->AddItem("File/New project", [&]() { OnNewProjectPressed(); });
	mMenuPanel->AddItem("File/Open project", [&]() { OnOpenProjectPressed(); });
	mMenuPanel->AddItem("File/Save project", [&]() { OnSaveProjectPressed(); }, nullptr, ShortcutKeys('S', true, true));

	mMenuPanel->AddItem("File/---");

	mMenuPanel->AddItem("File/Exit", [&]() { OnExitPressed(); });

	// EDIT
	mMenuPanel->AddItem("Edit/Undo", [&]() { OnUndoPressed(); }, nullptr, ShortcutKeys('Z', true));
	mMenuPanel->AddItem("Edit/Redo", [&]() { OnRedoPressed(); }, nullptr, ShortcutKeys('Y', true));

	mMenuPanel->AddItem("Edit/---");

	mMenuPanel->AddItem("Edit/Copy", [&]() {}, nullptr, ShortcutKeys('C', true));
	mMenuPanel->AddItem("Edit/Cut", [&]() {}, nullptr, ShortcutKeys('X', true));
	mMenuPanel->AddItem("Edit/Paste", [&]() {}, nullptr, ShortcutKeys('V', true));
	mMenuPanel->AddItem("Edit/Delete", [&]() {}, nullptr, ShortcutKeys(VK_DELETE));

	mMenuPanel->AddItem("Edit/---");

	mMenuPanel->AddItem("Edit/Transform/Move", [&]() {});
	mMenuPanel->AddItem("Edit/Transform/Rotate", [&]() {});
	mMenuPanel->AddItem("Edit/Transform/Scale", [&]() {});

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
	mMenuPanel->AddItem("Build/Run", [&]() { OnRunPressed(); }, nullptr, ShortcutKeys(VK_F5));
	mMenuPanel->AddItem("Build/Build & run", [&]() { OnBuildAndRunPressed(); }, nullptr, ShortcutKeys('R', true));
	mMenuPanel->AddItem("Build/Build", [&]() { OnBuildPressed(); }, nullptr, ShortcutKeys('R', true, true));

	// HELP
	mMenuPanel->AddItem("Help/About", [&]() { OnAboutPressed(); });
	mMenuPanel->AddItem("Help/Documentation", [&]() { OnDocumentationPressed(); });

	// DEBUG
	mMenuPanel->AddItem("Debug/Save layout as default", [&]() { OnSaveDefaultLayoutPressed(); });
}

MenuPanel::~MenuPanel()
{}

UIWidget* MenuPanel::AddItem(const UIMenuPanel::Item& item)
{
	return mMenuPanel->AddItem(item);
}

UIWidget* MenuPanel::AddItem(const WString& path, const Function<void()>& clickFunc /*= Function<void()>()*/, 
							 ImageAsset* icon /*= nullptr*/, const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
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

}

void MenuPanel::OnOpenScenePressed()
{

}

void MenuPanel::OnSaveScenePressed()
{

}

void MenuPanel::OnSaveSceneAsPressed()
{

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

}

void MenuPanel::OnRedoPressed()
{

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

}

void MenuPanel::OnShowPropertiesPressed()
{

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
