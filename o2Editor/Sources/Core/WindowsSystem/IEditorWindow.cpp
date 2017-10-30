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

CLASS_META(Editor::IEditorWindow)
{
	BASE_CLASS(o2::IObject);

	PUBLIC_FIELD(visible);
	PROTECTED_FIELD(mWindow);

	PUBLIC_FUNCTION(void, SetVisible, bool);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsVisible);
	PUBLIC_FUNCTION(void, Show);
	PUBLIC_FUNCTION(void, Hide);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
}
END_META;
