#include "stdafx.h"
#include "EditorApplication.h"

#include "Actions/PropertyChange.h"
#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Application/Input.h"
#include "Assets/Assets.h"
#include "Core/Actions/IAction.h"
#include "Core/EditorScope.h"
#include "Core/MenuPanel.h"
#include "Core/Properties/Properties.h"
#include "Core/ToolsPanel.h"
#include "Core/UIRoot.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "Scene/Scene.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/MenuPanel.h"
#include "UI/MenuPanel.h"
#include "UI/UIManager.h"
#include "UIStyle/EditorUIStyle.h"
#include "Utils/Debug/Debug.h"
#include "Utils/System/Time/Time.h"
#include "Utils/System/Time/Timer.h"
#include "Utils/Tasks/TaskManager.h"

namespace Editor
{
	EditorApplication::EditorApplication()
	{}

	EditorApplication::~EditorApplication()
	{}

	int EditorApplication::GetUndoActionsCount() const
	{
		return mActions.Count();
	}

	int EditorApplication::GetRedoActionsCount() const
	{
		return mForwardActions.Count();
	}

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

	void EditorApplication::DoneActorPropertyChangeAction(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

	void EditorApplication::ResetUndoActions()
	{
		for (auto x : mActions)
			delete x;

		for (auto x : mForwardActions)
			delete x;

		mActions.Clear();
		mForwardActions.Clear();
	}

	const String& EditorApplication::GetLoadedSceneName() const
	{
		return mLoadedScene;
	}

	void EditorApplication::LoadScene(const String& name)
	{
		mLoadedScene = name;
		o2Scene.Load(name);

		ResetUndoActions();
	}

	void EditorApplication::SaveScene(const String& name)
	{
		mLoadedScene = name;
		o2Scene.Save(name);
	}

	void EditorApplication::MakeNewScene()
	{
		mLoadedScene = "";
		o2Scene.Clear();

		ResetUndoActions();
	}

	bool EditorApplication::IsSceneChanged() const
	{
		return GetUndoActionsCount() > 0;
	}

	void EditorApplication::OnStarted()
	{
		PushScopeEnterOnStack enterScope;

		o2Application.SetWindowCaption("o2 Editor");

		mUIRoot = mnew UIRoot();

		mBackground = mnew Sprite("ui/UI_Background.png");
		mBackSign = mnew Sprite("ui/UI_o2_sign.png");

		mConfig = mnew EditorConfig();
		mConfig->LoadConfigs();

		LoadUIStyle();

		mProperties = mnew Properties();
		mWindowsManager = mnew WindowsManager();
		mMenuPanel = mnew MenuPanel();
		mToolsPanel = mnew ToolsPanel();

		if (mConfig->mProjectConfig.mMaximized)
		{
			o2Application.Maximize();
		}
		else
		{
			Vec2I pos = mConfig->mProjectConfig.mWindowPosition;
			o2Application.SetWindowSize(mConfig->mProjectConfig.mWindowSize);
			o2Application.SetWindowPosition(pos);
			mConfig->mProjectConfig.mWindowPosition = pos;
		}

		OnResizing();
	}

	void EditorApplication::OnClosing()
	{
		delete mConfig;
		delete mWindowsManager;
		delete mBackground;
		delete mBackSign;
		delete mToolsPanel;
		delete mMenuPanel;
		delete mUIRoot;
	}

	void EditorApplication::OnResizing()
	{
		mBackground->SetSize(o2Render.GetResolution() + Vec2F(20, 20));
		mBackSign->position = (Vec2F)(o2Render.GetResolution()).InvertedX()*0.5f + Vec2F(40.0f, -85.0f);

		mConfig->OnWindowChange();
		mUIRoot->OnApplicationSized();
	}

	void EditorApplication::OnMoved()
	{
		mConfig->OnWindowChange();
	}

	void EditorApplication::ProcessFrame()
	{
		if (!mReady)
			return;

		if (mCursorInfiniteModeEnabled)
			CheckCursorInfiniteMode();

		float maxFPS = 60.0f;
		float maxFPSDeltaTime = 1.0f/maxFPS;

		float realdDt = mTimer->GetDeltaTime();

		if (realdDt < maxFPSDeltaTime)
		{
			Sleep((int)((maxFPSDeltaTime - realdDt)*1000.0f));
			realdDt = maxFPSDeltaTime;
		}

		float dt = Math::Clamp(realdDt, 0.001f, 0.05f);

		mInput->PreUpdate();
		mTime->Update(realdDt);
		o2Debug.Update(dt);
		mTaskManager->Update(dt);
		mEventSystem->Update(dt);

		mRender->Begin();

		OnDraw();
		OnUpdate(dt);

		mUIRoot->Update(dt);
		mEventSystem->PostUpdate();
		mScene->Update(dt);


		OnDraw();
		mUIRoot->Draw();
		mUIManager->Draw();
		o2Debug.Draw();

		mRender->End();

		mInput->Update(dt);

		mDrawCalls = mRender->GetDrawCallsCount();
	}

	void EditorApplication::LoadUIStyle()
	{
		if (mNeedRebuildWndStyle)
		{
			EditorUIStyleBuilder builder;
			builder.RebuildEditorUIManager();
		}

		o2UI.LoadStyle("editor_ui_style.xml");
	}

	void EditorApplication::OnUpdate(float dt)
	{
		mWindowsManager->Update(dt);
		o2Application.windowCaption = String::Format("o2 Editor. FPS: %i DC: %i (%vi)", (int)o2Time.GetFPS(),
													 mDrawCalls, (Vec2I)o2Input.GetCursorPos());

		if (o2Input.IsKeyPressed('K'))
			o2Memory.DumpInfo();
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
	{
		//o2Assets.RebuildAssets();
	}

	void EditorApplication::OnDeactivated()
	{}

}