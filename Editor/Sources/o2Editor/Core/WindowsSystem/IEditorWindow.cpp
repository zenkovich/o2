#include "o2Editor/stdafx.h"
#include "IEditorWindow.h"

#include "o2/Application/Application.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/WindowsSystem/DockableWindow.h"

namespace Editor
{
	IEditorWindow::IEditorWindow()
	{
		mWindow = o2UI.CreateWidget<DockableWindow>();
		mWindow->layout->size = Vec2F(200, 200);
		mWindow->onOpened = THIS_FUNC(OnOpened);
		mWindow->onClosed = THIS_FUNC(OnClosed);

		EditorUIRoot.AddWidget(mWindow);
	}

	IEditorWindow::IEditorWindow(const IEditorWindow& other):
		mWindow(other.mWindow->CloneAsRef<DockableWindow>())
	{
		if (mWindow)
			EditorUIRoot.AddWidget(mWindow);
	}

	IEditorWindow::~IEditorWindow()
	{}

	void IEditorWindow::Show()
	{
		SetVisible(true);
	}

	void IEditorWindow::Hide()
	{
		SetVisible(false);
	}

	const Ref<DockableWindow>& IEditorWindow::GetWindow() const
	{
		return mWindow;
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
// --- META ---

DECLARE_CLASS(Editor::IEditorWindow, Editor__IEditorWindow);
// --- END META ---
