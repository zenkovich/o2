#include "WindowsManager.h"

#include "Core/Dialogs/ColorPickerDlg.h"
#include "Core/Dialogs/CurveEditorDlg.h"
#include "Core/Dialogs/EditNameDlg.h"
#include "Core/EditorConfig.h"
#include "Core/UIRoot.h"
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
			builder.RebuildEditorUIManager();
		}

		o2UI.LoadStyle("editor_ui_style.xml");

		mColorPickerDlg = mnew ColorPickerDlg();
		mCurveEditorDlg = mnew CurveEditorDlg();
		mNameEditDlg = mnew NameEditDlg();

		InitializeDock();
		InitializeWindows();

		SetWindowsLayout(o2EditorConfig.mProjectConfig.mLayout);

		mAvailableLayouts = o2EditorConfig.mGlobalConfig.mAvailableLayouts;
	}

	WindowsManager::~WindowsManager()
	{
		for (auto wnd : mEditorWindows)
			delete wnd;

		if (mColorPickerDlg)
			delete mColorPickerDlg;

		if (mCurveEditorDlg)
			delete mCurveEditorDlg;

		if (mNameEditDlg)
			delete mNameEditDlg;
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
		*mMainDockPlace->layout = UIWidgetLayout::BothStretch(0, 0, 0, 48);
		mMainDockPlace->SetResizibleDir(TwoDirection::Horizontal, 0, nullptr, nullptr);
		EditorUIRoot.AddWidget(mMainDockPlace);
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
			RectF rt = widget->layout->GetWorldRect();
			hierarchy += (String)rt.left + " " + (String)rt.bottom + " " + (String)rt.right + " " + (String)rt.top;
		}

		hierarchy += '\n';

		for (auto child : widget->GetChildWidgets())
			ProcHierarchy(hierarchy, child, level + 1);
	}

#undef DrawText

	void WindowsManager::Draw()
	{
		for (auto wnd : mEditorWindows)
			wnd->Draw();

		return;
		String hierarchy;
		ProcHierarchy(hierarchy, EditorUIRoot.GetRootWidget(), 0);
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

		for (auto widget : EditorUIRoot.GetRootWidget()->GetChildWidgets())
		{
			if (widget->GetType() == TypeOf(UIDockableWindow))
				res.windows.Add(widget->name, *widget->layout);
		}

		return res;
	}

	void WindowsManager::SetWindowsLayout(WindowsLayout layout)
	{
		for (auto wnd : layout.windows)
		{
			IEditorWindow* editorWindow = o2EditorWindows.mEditorWindows.FindMatch([&](IEditorWindow* x) {
				return x->mWindow->GetName() == wnd.Key();
			});

			if (!editorWindow)
			{
				o2Debug.Log("Can't restore window with name:" + wnd.Key());
				continue;
			}

			if (UIDockableWindow* dockWnd = editorWindow->mWindow)
			{
				editorWindow->Show();
				*dockWnd->layout = wnd.Value();
			}
		}

		layout.RestoreDock(&layout.mainDock, o2EditorWindows.mMainDockPlace);

		for (auto wnd : o2EditorWindows.mEditorWindows)
		{
			bool hide = !layout.windows.ContainsKey(wnd->mWindow->GetName()) && 
				wnd->mWindow->GetParent()->GetType() != TypeOf(UIDockWindowPlace);

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
		SetWindowsLayout(o2EditorConfig.mGlobalConfig.mDefaultLayout);
	}

	void WindowsManager::SaveCurrentWindowsLayout(const String& name)
	{
		mAvailableLayouts[name] = GetWindowsLayout();
	}

}