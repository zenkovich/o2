#include "EditorApplication.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Application/Input.h"
#include "Core/ToolsPanel.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "UI/MenuPanel.h"
#include "UI/MenuPanel.h"
#include "UI/UIManager.h"
#include "Utils/Debug.h"
#include "Utils/TaskManager.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"

EditorApplication::EditorApplication()
{}

EditorApplication::~EditorApplication()
{}

UIMenuPanel* EditorApplication::GetMenuPanel() const
{
	return mMenuPanel;
}

void EditorApplication::OnStarted()
{
	o2Application.SetWindowCaption("o2 Editor");

	mConfig = mnew ApplicationConfig();
	mWindowsManager = mnew WindowsManager();

	mBackground = mnew Sprite("ui/UI_Background.png");
	mBackSign = mnew Sprite("ui/UI_o2_sign.png");

	mMenuPanel = o2UI.CreateWidget<UIMenuPanel>();
	o2UI.AddWidget(mMenuPanel);

	mMenuPanel->layout.anchorMin = Vec2F(0, 1);
	mMenuPanel->layout.anchorMax = Vec2F(1, 1);
	mMenuPanel->layout.offsetMin = Vec2F(0, -20);
	mMenuPanel->layout.offsetMax = Vec2F(0, 0);

	mMenuPanel->AddItem("File/New", []() { o2Debug.Log("New file"); });
	mMenuPanel->AddItem("File/Open", []() { o2Debug.Log("Open file"); });
	mMenuPanel->AddItem("File/Save", []() { o2Debug.Log("Open file"); });
	mMenuPanel->AddItem("File/Save as", []() { o2Debug.Log("Open file"); });
	mMenuPanel->AddItem("File/---", []() { o2Debug.Log("Open file"); });
	mMenuPanel->AddItem("File/Exit", []() { o2Debug.Log("Open file"); });

	mMenuPanel->AddItem("Edit/Undo", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/Redo", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/---", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/Copy", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/Cut", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/Paste", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/Delete", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/---", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("Edit/Transform/Move", []() { o2Debug.Log("Edit transform move"); });
	mMenuPanel->AddItem("Edit/Transform/Rotate", []() { o2Debug.Log("Edit transform move"); });
	mMenuPanel->AddItem("Edit/Transform/Scale", []() { o2Debug.Log("Edit transform move"); });

	mMenuPanel->AddItem("View/Windows/Tree", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("View/Windows/Scene", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("View/Windows/Assets", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("View/Windows/Properties", []() { o2Debug.Log("Edit copy"); });
	mMenuPanel->AddItem("View/Windows/Animation", []() { o2Debug.Log("Edit copy"); });

	mMenuPanel->AddItem("Help/About", []() { o2Debug.Log("About"); });
	mMenuPanel->AddItem("Help/Documentation", []() { o2Debug.Log("About"); });

	mToolsPanel = mnew ToolsPanel();

	OnResizing();
}

void EditorApplication::OnClosing()
{
	delete mWindowsManager;
	delete mBackground;
	delete mConfig;
	delete mToolsPanel;
}

void EditorApplication::OnResizing()
{
	mBackground->SetSize(o2Render.GetResolution() + Vec2F(20, 20));
	mBackSign->position = (Vec2F)(o2Render.GetResolution()).InvertedX()*0.5f + Vec2F(40.0f, -85.0f);
}

void EditorApplication::ProcessFrame()
{
	if (!mReady)
		return;

	float realdDt = mTimer->GetDeltaTime();
	float dt = Math::Clamp(realdDt, 0.001f, 0.05f);

	mTime->Update(realdDt);
	o2Debug.Update(dt);
	mTaskManager->Update(dt);
	mEventSystem->Update(dt);

	OnUpdate(dt);

	mUIManager->Update(dt);

	mRender->Begin();

	OnDraw();
	mUIManager->Draw();
	o2Debug.Draw();

	mRender->End();

	mInput->Update(dt);
}

void EditorApplication::OnUpdate(float dt)
{
	mWindowsManager->Update(dt);
	o2Application.windowCaption = String::Format("o2 Editor. FPS: %i DC: %i (%vi)", (int)o2Time.GetFPS(),
												 o2Render.GetDrawCallsCount(), (Vec2I)o2Input.GetCursorPos());

	if (o2Input.IsKeyPressed('K'))
		MemoryManager::instance->DumpInfo();
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
