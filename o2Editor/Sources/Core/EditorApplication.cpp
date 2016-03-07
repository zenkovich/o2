#include "EditorApplication.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Application/Input.h"
#include "Core/Actions/IAction.h"
#include "Core/MenuPanel.h"
#include "Core/ToolsPanel.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/MenuPanel.h"
#include "UI/MenuPanel.h"
#include "UI/UIManager.h"
#include "Utils/Debug.h"
#include "Utils/TaskManager.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"

namespace Editor
{
	EditorApplication::EditorApplication()
	{}

	EditorApplication::~EditorApplication()
	{}

	String EditorApplication::GetLastActionName() const
	{
		if (mActions.Count() > 0)
			return mActions.Last()->GetName();

		return "";
	}

	String EditorApplication::GetNextForwardActionName() const
	{
		if (mForwardActions.Count() > 0)
			return mForwardActions.Last()->GetName();

		return "";
	}

	void EditorApplication::UndoAction()
	{
		if (mActions.Count() > 0)
		{
			mActions.Last()->Undo();
			mForwardActions.Add(mActions.PopBack());
		}
	}

	void EditorApplication::RedoAction()
	{
		if (mForwardActions.Count() > 0)
		{
			mForwardActions.Last()->Redo();
			mActions.Add(mForwardActions.PopBack());
		}
	}

	void EditorApplication::DoneAction(IAction* action)
	{
		mActions.Add(action);

		for (auto action : mForwardActions)
			delete action;

		mForwardActions.Clear();
	}

	void EditorApplication::OnStarted()
	{
		o2Application.SetWindowCaption("o2 Editor");

		mBackground = mnew Sprite("ui/UI_Background.png");
		mBackSign = mnew Sprite("ui/UI_o2_sign.png");

		mConfig = mnew EditorConfig();
		mWindowsManager = mnew WindowsManager();
		mMenuPanel = mnew MenuPanel();
		mToolsPanel = mnew ToolsPanel();

		OnResizing();
		mConfig->LoadConfigs();
	}

	void EditorApplication::OnClosing()
	{
		delete mConfig;
		delete mWindowsManager;
		delete mBackground;
		delete mBackSign;
		delete mToolsPanel;
		delete mMenuPanel;
	}

	void EditorApplication::OnResizing()
	{
		mBackground->SetSize(o2Render.GetResolution() + Vec2F(20, 20));
		mBackSign->position = (Vec2F)(o2Render.GetResolution()).InvertedX()*0.5f + Vec2F(40.0f, -85.0f);

		mConfig->OnWindowChange();
	}

	void EditorApplication::OnMoved()
	{
		mConfig->OnWindowChange();
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

#undef DrawText

	void EditorApplication::OnDraw()
	{
		o2Render.Clear();
		o2Render.camera = Camera::Default();

		mBackground->Draw();
		mBackSign->Draw();
		mWindowsManager->Draw();

		// Debug draw undo actions
	// 	for (int i = 0; i < mActions.Count(); i++)
	// 		o2Debug.DrawText(Vec2F(0, 20 * i), (String)i + mActions[i]->GetName());
	}

	void EditorApplication::OnActivated()
	{}

	void EditorApplication::OnDeactivated()
	{}

}