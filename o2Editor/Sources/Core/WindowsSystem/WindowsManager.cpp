#include "WindowsManager.h"

#include "Core/EditorConfig.h"
#include "Core/UIStyle/EditorUIStyle.h"
#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "Render/Render.h"
#include "UI/MenuPanel.h"
#include "UI/UIManager.h"

namespace Editor
{
	WindowsManager::WindowsManager()
	{
		if (mNeedRebuildWndStyle)
		{
			EditorUIStyleBuilder builder;
			builder.RebuildEditorUIStyle();
		}

		o2UI.LoadStyle("editor_ui_style.xml");

		InitializeDock();
		InitializeWindows();

		SetWindowsLayout(o2EditorConfig.mProjectConfig.mLayout);

		mAvailableLayouts = o2EditorConfig.mGlobalConfig.mAvailableLayouts;
	}

	WindowsManager::~WindowsManager()
	{
		for (auto wnd : mEditorWindows)
			delete wnd;
	}

	void WindowsManager::InitializeWindows()
	{
		auto windowTypes = TypeOf(IEditorWindow).GetDerivedTypes();

		for (auto type : windowTypes)
		{
			IEditorWindow* newWindow = (IEditorWindow*)type->CreateSample();
			mEditorWindows.Add(newWindow);
		}

		for (auto wnd : mEditorWindows)
			wnd->PostInitializeWindow();
	}

	void WindowsManager::InitializeDock()
	{
		mMainDockPlace = mnew UIDockWindowPlace();
		mMainDockPlace->name = "main dock";
		mMainDockPlace->layout = UIWidgetLayout::BothStretch(0, 0, 0, 48);
		mMainDockPlace->SetResizibleDir(TwoDirection::Horizontal, 0, nullptr, nullptr);
		o2UI.AddWidget(mMainDockPlace);
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

		if (widget->GetType() == TypeOf(UIDockWindowPlace))
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

	WindowsLayout WindowsManager::GetWindowsLayout()
	{
		WindowsLayout res;

		res.mainDock.RetrieveLayout(o2EditorWindows.mMainDockPlace);

		for (auto widget : o2UI.GetAllWidgets())
		{
			if (widget->GetType() == TypeOf(UIDockableWindow))
				res.windows.Add(widget->name, widget->layout);
		}

		return res;
	}

	void WindowsManager::SetWindowsLayout(WindowsLayout layout)
	{
		for (auto wnd : layout.windows)
		{
			UIDockableWindow* dockWnd = (UIDockableWindow*)o2EditorWindows.mEditorWindows.FindMatch([&](IEditorWindow* x) {
				return x->mWindow->GetName() == wnd.Key();
			})->mWindow;

			if (dockWnd)
				dockWnd->layout = wnd.Value();
		}

		layout.RestoreDock(&layout.mainDock, o2EditorWindows.mMainDockPlace);
	}

	void WindowsManager::SetWindowsLayout(const String& name)
	{
		if (mAvailableLayouts.ContainsKey(name))
			SetWindowsLayout(mAvailableLayouts[name]);
	}

	void WindowsManager::SetDefaultWindowsLayout()
	{
		SetWindowsLayout(o2EditorConfig.mGlobalConfig.mDefaultLayout);
	}

	void WindowsManager::SaveCurrentWindowsLayout(const String& name)
	{
		mAvailableLayouts[name] = GetWindowsLayout();
	}

}