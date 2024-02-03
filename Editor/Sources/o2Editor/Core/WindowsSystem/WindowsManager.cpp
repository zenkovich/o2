#include "o2Editor/stdafx.h"
#include "WindowsManager.h"

#include "o2/Render/Render.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2Editor/Core/Dialogs/ColorPickerDlg.h"
#include "o2Editor/Core/Dialogs/CurveEditorDlg.h"
#include "o2Editor/Core/Dialogs/EditNameDlg.h"
#include "o2Editor/Core/Dialogs/KeyEditDlg.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/UIStyle/EditorUIStyle.h"
#include "o2Editor/Core/WindowsSystem/DockWindowPlace.h"
#include "o2Editor/Core/WindowsSystem/DockableWindow.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

namespace Editor
{
	WindowsManager::WindowsManager()
	{
		PushEditorScopeOnStack scope;

		mmake<ColorPickerDlg>();
		mmake<CurveEditorDlg>();
		mmake<NameEditDlg>();
		mmake<KeyEditDlg>();

		InitializeDock();
		InitializeWindows();

		SetWindowsLayout(o2EditorConfig.projectConfig.mLayout);

		mAvailableLayouts = o2EditorConfig.globalConfig.mAvailableLayouts;
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
			IEditorWindow* newWindow = mmaket<IEditorWindow>();
			mEditorWindows.push_back(newWindow);
		}

		for (auto wnd : mEditorWindows)
			wnd->PostInitializeWindow();
	}

	void WindowsManager::InitializeDock()
	{
		PushEditorScopeOnStack scope;

		mMainDockPlace = mmaket<DockWindowPlace>();
		mMainDockPlace->name = "main dock";
		*mMainDockPlace->layout = WidgetLayout::BothStretch(0, 0, 0, 48);
		mMainDockPlace->SetResizibleDir(TwoDirection::Horizontal, 0, nullptr, nullptr);
		EditorUIRoot.AddWidget(mMainDockPlace);
	}

	void WindowsManager::Update(float dt)
	{
		for (const auto& wnd : mEditorWindows)
			wnd->Update(dt);
	}

	void ProcHierarchy(String& hierarchy, Widget* widget, int level)
	{
		String sideNames[] = { "Hor", "Ver" };

		for (int i = 0; i < level; i++)
			hierarchy += ' ';

		hierarchy += widget->GetName();

		if (widget->GetType() == TypeOf(DockWindowPlace))
		{
			hierarchy += ": ";
			hierarchy += (String)(bool)((DockWindowPlace*)widget)->interactable;
			hierarchy += " ";
			hierarchy += sideNames[(int)((DockWindowPlace*)widget)->GetResizibleDir()];
			RectF rt = widget->layout->GetWorldRect();
			hierarchy += (String)rt.left + " " + (String)rt.bottom + " " + (String)rt.right + " " + (String)rt.top;
		}

		hierarchy += '\n';

		for (auto child : widget->GetChildWidgets())
			ProcHierarchy(hierarchy, child, level + 1);
	}

#undef DrawText

	void WindowsManager::Draw(const Ref<Render>& render)
	{
		for (const auto& wnd : mEditorWindows)
			wnd->Draw(render);

//		return;
//		String hierarchy;
//		ProcHierarchy(hierarchy, EditorUIRoot.GetRootWidget(), 0);
//		o2Debug.DrawText((Vec2F)(o2Render.GetResolution().InvertedX())*0.5f, hierarchy);
	}

	void WindowsManager::AddWindow(const Ref<IEditorWindow>& window)
	{
		mEditorWindows.push_back(window);
	}

	WindowsLayout WindowsManager::GetWindowsLayout()
	{
		WindowsLayout res;

		res.mainDock.RetrieveLayout(o2EditorWindows.mMainDockPlace);

		for (const auto& widget : EditorUIRoot.GetRootWidget()->GetChildWidgets())
		{
			if (widget->GetType() == TypeOf(DockableWindow))
				res.windows.Add(widget->name, *widget->layout);
		}

		return res;
	}

	void WindowsManager::SetWindowsLayout(const WindowsLayout& layout)
	{
		PushEditorScopeOnStack scope;

		for (const auto& wnd : layout.windows)
		{
			const auto& editorWindow = o2EditorWindows.mEditorWindows.FindOrDefault([&](const auto& x) {
				return x->mWindow->GetName() == wnd.first;
			});

			if (!editorWindow)
			{
				o2Debug.Log("Can't restore window with name:" + wnd.first);
				continue;
			}

			if (auto dockWnd = editorWindow->mWindow.lock())
			{
				editorWindow->Show();
				*dockWnd->layout = wnd.second;
			}
		}

		layout.RestoreDock(&layout.mainDock, o2EditorWindows.mMainDockPlace);

		for (const auto& wnd : o2EditorWindows.mEditorWindows)
		{
			bool hide = !layout.windows.ContainsKey(wnd->mWindow.lock()->GetName()) &&
				wnd->mWindow.lock()->GetParent()->GetType() != TypeOf(DockWindowPlace);

			if (hide)
				wnd->Hide();
		}
	}

	void WindowsManager::SetWindowsLayout(const String& name)
	{
		if (mAvailableLayouts.ContainsKey(name))
			SetWindowsLayout(mAvailableLayouts[name]);
	}

	void WindowsManager::SetDefaultWindowsLayout()
	{
		SetWindowsLayout(o2EditorConfig.globalConfig.mDefaultLayout);
	}

	void WindowsManager::SaveCurrentWindowsLayout(const String& name)
	{
		mAvailableLayouts[name] = GetWindowsLayout();
	}

}