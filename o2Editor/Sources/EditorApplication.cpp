#include "EditorApplication.h"

#include "Core/WindowsSystem/WindowsManager.h"

EditorApplication::EditorApplication()
{}

EditorApplication::~EditorApplication()
{}

void EditorApplication::OnStarted()
{
	o2Application.SetWindowCaption("o2 Editor");

	mWindowsManager = mnew WindowsManager();
	mConfig = mnew ApplicationConfig();

	mBackground = mnew Sprite("ui/UI_Background.png");
	mBackSign = mnew Sprite("ui/UI_o2_sign.png");

	OnResizing();
}

void EditorApplication::OnClosing()
{
	mWindowsManager.Release();
	mBackground.Release();
	mConfig.Release();
}

void EditorApplication::OnResizing()
{
	mBackground->SetSize(o2Render.GetResolution() + Vec2F(20, 20));
	mBackSign->position = (Vec2F)(o2Render.GetResolution()).InvertedX()*0.5f + Vec2F(50.0f, -50.0f);
}

void EditorApplication::OnUpdate(float dt)
{
	mWindowsManager->Update(dt);
	o2Application.windowCaption = String::Format("o2 Editor FPS: %i", (int)o2Time.GetFPS());
}

void EditorApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.camera = Camera::Default();

	mBackground->Draw();
	mBackSign->Draw();
	mWindowsManager->Draw();
}

void EditorApplication::OnActivated()
{}

void EditorApplication::OnDeactivated()
{}
