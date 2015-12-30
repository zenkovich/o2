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
		wnd.Release();
}

void WindowsManager::InitializeWindows()
{
	auto windowTypes = IEditorWindow::type->DerivedTypes();

	for (auto type : windowTypes)
	{
		for (int i = 0; i < 5; i++)
		{
			Ptr<IEditorWindow> newWindow = type->CreateSample();
			newWindow->mWindow->caption = String::Format("window %i", i + 1);
			newWindow->mWindow->name = String::Format("window %i", i + 1);
			mEditorWindows.Add(newWindow);
		}
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

void ProcHierarchy(String& hierarchy, Ptr<UIWidget> widget, int level)
{
	String sideNames[] = { "Hor", "Ver" };

	for (int i = 0; i < level; i++)
		hierarchy += ' ';

	hierarchy += widget->GetName();

	if (widget->GetType() == *UIDockWindowPlace::type)
	{
		hierarchy += ": ";
		hierarchy += (String)(bool)widget.Cast<UIDockWindowPlace>()->interactable;
		hierarchy += " ";
		hierarchy += sideNames[(int)widget.Cast<UIDockWindowPlace>()->GetResizibleDir()];
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

void WindowsManager::AddWindow(Ptr<IEditorWindow> window)
{
	mEditorWindows.Add(window);
}

