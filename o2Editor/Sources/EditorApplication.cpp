#include "EditorApplication.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Application/Input.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Render/Render.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "Utils/Timer.h"

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

	auto actor = mnew Actor({ mnew ImageComponent("ui/UI_o2_sign.png") });	
	actor->Play(Animation::EaseInOut<Vec2F>(actor, &actor->transform.position, Vec2F(), Vec2F(200, 200), 2.0f))
		->animation.loop = Loop::PingPong;

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

Timer timer;

void EditorApplication::OnUpdate(float dt)
{
	//o2Debug.Log("DT = %f", timer.GetDeltaTime());
	mWindowsManager->Update(dt);
	o2Application.windowCaption = String::Format("o2 Editor. FPS: %i (%vi)", (int)o2Time.GetFPS(), 
												 (Vec2I)o2Input.GetCursorPos());
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
