#include "o2Editor/stdafx.h"
#include "EditorApplication.h"

#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Animation/Tracks/AnimationVec2FTrack.h"
#include "o2/Application/Input.h"
#include "o2/Assets/Assets.h"
#include "o2/Events/EventSystem.h"
#include "o2/Render/Render.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/Components/ImageComponent.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/System/Time/Time.h"
#include "o2/Utils/System/Time/Timer.h"
#include "o2/Utils/Tasks/TaskManager.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/MenuPanel.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/UIStyle/EditorUIStyle.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{
	EditorApplication::EditorApplication()
	{}

	EditorApplication::~EditorApplication()
	{}

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

	void EditorApplication::SetPlaying(bool playing)
	{
		ForcePopEditorScopeOnStack scope;

		if (playing == mIsPlaying)
			return;

		if (playing)
		{
			o2EditorSceneScreen.ClearSelection();

			mSceneDump.Clear();
			o2Scene.Save(mSceneDump);
			o2Scene.Load(mSceneDump);
		}
		else
		{
			o2EditorSceneScreen.ClearSelection();
			o2Scene.Load(mSceneDump);
		}

		mIsPlaying = playing;
	}

	bool EditorApplication::IsPlaying() const
	{
		return mIsPlaying;
	}

	void EditorApplication::OnStarted()
	{
		PushEditorScopeOnStack enterScope;

		o2Application.SetWindowCaption("o2 Editor");

		mUIRoot = mnew UIRoot();

		mBackground = mnew Sprite("ui/UI4_Background.png");
		mBackSign = mnew Sprite("ui/UI4_o2_sign.png");

		mConfig = mnew EditorConfig();
		mConfig->LoadConfigs();

		LoadUIStyle();

		mProperties = mnew Properties();
		mWindowsManager = mnew WindowsManager();
		mMenuPanel = mnew MenuPanel();
		mToolsPanel = mnew ToolsPanel();

		if (mConfig->mProjectConfig.mMaximized)
			o2Application.Maximize();
		else
		{
			Vec2I pos = mConfig->mProjectConfig.mWindowPosition;
			o2Application.SetWindowSize(mConfig->mProjectConfig.mWindowSize);
			o2Application.SetWindowPosition(pos);
			mConfig->mProjectConfig.mWindowPosition = pos;
		}

		OnResizing();

		auto widget = EditorUIRoot.GetRootWidget()->GetChildWidget("tools panel/play panel");
		o2EditorAnimationWindow.SetAnimation(&widget->GetStateObject("playing")->GetAnimationClip(),
											 &widget->GetStateObject("playing")->player);

		o2EditorAnimationWindow.SetTarget(widget);
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
		mUpdateStep = mIsPlaying && (!isPaused || step);
		step = false;

		Application::ProcessFrame();

		mDrawCalls = mRender->GetDrawCallsCount();
	}

	void EditorApplication::LoadUIStyle()
	{
		EditorUIStyleBuilder builder;
		builder.RebuildEditorUIManager("editor_ui_style.json", true, true);
	}

	void EditorApplication::PreUpdatePhysics()
	{
		Application::PreUpdatePhysics();
	}

	void EditorApplication::UpdatePhysics(float dt)
	{
		if (mUpdateStep)
			Application::UpdatePhysics(dt);
	}

	void EditorApplication::PostUpdatePhysics()
	{
		Application::PostUpdatePhysics();
	}

	void EditorApplication::UpdateScene(float dt)
	{
		if (mUpdateStep)
		{
			mScene->Update(dt);
			o2EditorSceneScreen.NeedRedraw();
		}
	}

	void EditorApplication::DrawScene()
	{}

	void EditorApplication::DrawUIManager()
	{
		PushEditorScopeOnStack scope;
		Application::DrawUIManager();
	}

	void EditorApplication::UpdateEventSystem()
	{
		PushEditorScopeOnStack scope;
		Application::UpdateEventSystem();
	}

	void EditorApplication::PostUpdateEventSystem()
	{
		PushEditorScopeOnStack scope;
		Application::PostUpdateEventSystem();
	}

	void EditorApplication::OnUpdate(float dt)
	{
		mWindowsManager->Update(dt);
		mUIRoot->Update(dt);
		mToolsPanel->Update(dt);

		o2Application.windowCaption = String("o2 Editor. FPS: ") + (String)((int)o2Time.GetFPS()) +
			" DC: " + (String)mDrawCalls +
			" Cursor: " + (String)o2Input.GetCursorPos();

		if (o2Input.IsKeyPressed('K'))
			o2Memory.DumpInfo();
	}

#undef DrawText

	void EditorApplication::OnDraw()
	{
		PushEditorScopeOnStack scope;

		o2Render.Clear();
		o2Render.camera = Camera::Default();

		mBackground->Draw();
		mBackSign->Draw();
		mWindowsManager->Draw(); 
		mUIRoot->Draw();

		// Debug draw undo actions
		if (o2Input.IsKeyDown(VK_F6)) 
		{
			for (int i = 0; i < mActions.Count(); i++)
				o2Debug.DrawText(Vec2F(0, (float)(20 * i)), (String)i + mActions[i]->GetName());
		}
	}

	void EditorApplication::OnActivated()
	{
		//o2Assets.RebuildAssets();
	}

	void EditorApplication::OnDeactivated()
	{}

}
