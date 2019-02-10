#include "stdafx.h"
#include "IEditorWindow.h"

#include "Application/Application.h"
#include "Core/UIRoot.h"
#include "Core/WindowsSystem/DockableWindow.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	IEditorWindow::IEditorWindow()
	{
		mWindow = o2UI.CreateWidget<DockableWindow>();
		mWindow->layout->size = Vec2F(200, 200);
		EditorUIRoot.AddWidget(mWindow);
	}

	IEditorWindow::IEditorWindow(const IEditorWindow& other):
		mWindow(other.mWindow->CloneAs<DockableWindow>())
	{
		if (mWindow)
			EditorUIRoot.AddWidget(mWindow);
	}

	IEditorWindow::~IEditorWindow()
	{
		if (mWindow)
			delete mWindow;
	}

	void IEditorWindow::Show()
	{
		SetVisible(true);
	}

	void IEditorWindow::Hide()
	{
		SetVisible(false);
	}

	void IEditorWindow::SetVisible(bool visible)
	{
		mWindow->SetEnabled(visible);
	}

	void IEditorWindow::Update(float dt)
	{}

	void IEditorWindow::Draw()
	{}

	bool IEditorWindow::IsVisible()
	{
		return mWindow->IsEnabled();
	}

}

DECLARE_CLASS(Editor::IEditorWindow);
