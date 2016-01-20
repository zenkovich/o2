#include "WindowsManager.h"

#include "Core/UIStyle/EditorUIStyle.h"
#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "Render/Render.h"
#include "UI/MenuPanel.h"
#include "UI/UIManager.h"

WindowsManager::WindowsManager()
{
	if (mNeedRebuildWndStyle)
		UIStyle::RebuildEditorUIStyle();

	InitializeDock();
	InitializeWindows();
}

WindowsManager::~WindowsManager()
{
	for (auto wnd : mEditorWindows)
		delete wnd;
}

void WindowsManager::InitializeWindows()
{
	auto windowTypes = IEditorWindow::type->DerivedTypes();

	for (auto type : windowTypes)
	{
		IEditorWindow* newWindow = (IEditorWindow*)type->CreateSample();
		mEditorWindows.Add(newWindow);
	}
}

void WindowsManager::InitializeDock()
{
	auto mainDockPlace = mnew UIDockWindowPlace();
	mainDockPlace->name = "main dock";
	mainDockPlace->layout = UIWidgetLayout::BothStretch(0, 0, 0, 48);
	mainDockPlace->SetResizibleDir(TwoDirection::Horizontal, 0, nullptr, nullptr);
	o2UI.AddWidget(mainDockPlace);
}

void WindowsManager::Update(float dt)
{
	for (auto wnd : mEditorWindows)
		wnd->Update(dt);
}

void ProcHierarchy(String& hierarchy, UIWidget* widget, int level)
{
	String sideNames[] = { "Hor", "Ver" };

	for (int i = 0; i < level; i++)
		hierarchy += ' ';

	hierarchy += widget->GetName();

	if (widget->GetType() == *UIDockWindowPlace::type)
	{
		hierarchy += ": ";
		hierarchy += (String)(bool)((UIDockWindowPlace*)widget)->interactable;
		hierarchy += " ";
		hierarchy += sideNames[(int)((UIDockWindowPlace*)widget)->GetResizibleDir()];
		RectF rt = widget->layout.GetAbsoluteRect();
		hierarchy += (String)rt.left + " " + (String)rt.bottom + " " + (String)rt.right + " " + (String)rt.top;
	}

	hierarchy += '\n';

	for (auto child : widget->GetChilds())
		ProcHierarchy(hierarchy, child, level + 1);
}

#undef DrawText

void WindowsManager::Draw()
{
	for (auto wnd : mEditorWindows)
		wnd->Draw();

	return;
	String hierarchy;
	ProcHierarchy(hierarchy, o2UI.GetScreenWidget(), 0);
	o2Debug.DrawText((Vec2F)(o2Render.GetResolution().InvertedX())*0.5f, hierarchy);
}

void WindowsManager::AddWindow(IEditorWindow* window)
{
	mEditorWindows.Add(window);
}

