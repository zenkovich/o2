#include "IEditorWindow.h"

#include "Application/Application.h"
#include "Core/UIRoot.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	IEditorWindow::IEditorWindow()
	{
		mWindow = o2UI.CreateWidget<UIDockableWindow>();
		mWindow->layout->size = Vec2F(200, 200);
		EditorUIRoot.AddWidget(mWindow);
	}

	IEditorWindow::IEditorWindow(const IEditorWindow& other):
		mWindow(other.mWindow->CloneAs<UIDockableWindow>())
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
		mWindow->SetVisible(visible);
	}

	void IEditorWindow::Update(float dt)
	{}

	void IEditorWindow::Draw()
	{}

	bool IEditorWindow::IsVisible()
	{
		return mWindow->IsVisible();
	}

}

DECLARE_CLASS(Editor::IEditorWindow);
