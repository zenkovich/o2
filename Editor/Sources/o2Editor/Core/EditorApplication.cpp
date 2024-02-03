#include <memory>

#include "o2Editor/stdafx.h"
#include "EditorApplication.h"

#include "o2/Animation/Tracks/AnimationColor4Track.h"
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
#include "o2/Scripts/ScriptEngine.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/Function/ActorSubscription.h"
#include "o2/Utils/System/Time/Time.h"
#include "o2/Utils/System/Time/Timer.h"
#include "o2/Utils/Tasks/TaskManager.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/MenuPanel.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/UIStyle/EditorUIStyle.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/TreeWindow.h"
#include "o2Editor/Utils/CommonTextures.h"

namespace Editor
{
	EditorApplication::EditorApplication()
	{
	}

	EditorApplication::~EditorApplication()
	{
	}

	const String& EditorApplication::GetLoadedSceneName() const
	{
		if (mLoadedScene)
			return mLoadedScene->GetPath();

		return String::empty;
	}

	void EditorApplication::LoadScene(const Ref<SceneAsset>& scene)
	{
		ForcePopEditorScopeOnStack scope;

		if (scene)
			scene->Load();

		mLoadedScene = scene;

		ResetUndoActions();

		if (mLoadedScene)
			o2EditorConfig.projectConfig.mLastLoadedScene = mLoadedScene->GetPath();
	}

	void EditorApplication::SaveScene()
	{
		o2Scene.Save(mLoadedScene->GetFullPath());
		mLoadedScene->Save();

		if (mLoadedScene)
			o2EditorConfig.projectConfig.mLastLoadedScene = mLoadedScene->GetPath();
	}

	void EditorApplication::SaveSceneAs(const String& path)
	{
		String relativePath = o2FileSystem.GetPathRelativeToPath(path, ::GetAssetsPath());

		o2Scene.Save(path);

		mLoadedScene = mmake<SceneAsset>();
		mLoadedScene->SetPath(relativePath);
		mLoadedScene->Save();

		if (mLoadedScene)
			o2EditorConfig.projectConfig.mLastLoadedScene = mLoadedScene->GetPath();
	}

	void EditorApplication::MakeNewScene()
	{
		mLoadedScene = nullptr;
		o2Scene.Clear();

		ResetUndoActions();
		o2EditorPropertiesWindow.ResetTargets();
	}

	bool EditorApplication::IsSceneChanged() const
	{
		return GetUndoActionsCount() > 0;
	}

	void EditorApplication::SetPlaying(bool playing)
	{
		if (playing == mIsPlaying)
			return;

		mPlayingChanged = true;
		mIsPlaying = playing;

		o2Scene.SetEditorPlaying(mIsPlaying);
	}

	bool EditorApplication::IsPlaying() const
	{
		return mIsPlaying;
	}

	void EditorApplication::OnStarted()
	{
		PushEditorScopeOnStack enterScope;

		o2Application.SetWindowCaption("o2 Editor");

		mUIRoot = mmake<UIRoot>();

		mBackground = mmake<Sprite>("ui/UI4_Background.png");
		mBackSign = mmake<Sprite>("ui/UI4_o2_sign.png");

		CommonTextures::Initialize();

		mConfig = mmake<EditorConfig>();
		mConfig->LoadConfigs();

		LoadUIStyle();

		mProperties = mmake<Properties>();
		mWindowsManager = mmake<WindowsManager>();
		mMenuPanel = mmake<MenuPanel>();
		mToolsPanel = mmake<ToolsPanel>();

		if (mConfig->projectConfig.mMaximized)
			o2Application.Maximize();
		else
		{
			Vec2I pos = mConfig->projectConfig.mWindowPosition;
			o2Application.SetWindowSize(mConfig->projectConfig.mWindowSize);
			o2Application.SetWindowPosition(pos);
			mConfig->projectConfig.mWindowPosition = pos;
		}

		OnResizing();

		o2EditorApplication.LoadScene(mmake<SceneAsset>(o2EditorConfig.projectConfig.mLastLoadedScene));

		auto widget = EditorUIRoot.GetRootWidget()->GetChildWidget("tools panel/play panel");
		o2EditorAnimationWindow.SetAnimation(widget->GetStateObject("playing")->GetAnimationClip().Get(),
											 &widget->GetStateObject("playing")->GetAnimationPlayer());

		o2EditorAnimationWindow.SetTarget(widget);

		o2Scripts.CollectGarbage();
	}
}#include <memory>

template<typename T>
class Ref
{
public:
	Ref() : m_ptr(nullptr) {}
	Ref(T* ptr) : m_ptr(ptr) {}
	Ref(const Ref<T>& other) : m_ptr(other.m_ptr) {}
	Ref<T>& operator=(const Ref<T>& other) { m_ptr = other.m_ptr; return *this; }

	operator T*() { return m_ptr; }
	T* operator->() { return m_ptr; }
	const T* operator->() const { return m_ptr; }

private:
	T* m_ptr;
};

template<typename T>
class WeakRef
{
public:
	WeakRef() : m_ptr(nullptr) {}
	WeakRef(const Ref<T>& ref) : m_ptr(ref) {}
	WeakRef(const WeakRef<T>& other) : m_ptr(other.m_ptr) {}
	WeakRef<T>& operator=(const WeakRef<T>& other) { m_ptr = other.m_ptr; return *this; }

	operator Ref<T>() { return m_ptr; }
	Ref<T> Lock() { return m_ptr; }

private:
	Ref<T> m_ptr;
};

template<typename T, typename ...Args>
Ref<T> mmake(Args&& ...args)
{
	return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& ref)
{
	return Ref<Derived>(dynamic_cast<Derived*>(ref));
}

// Example usage

class Widget {};

class Background {};

void nerty(const std::string& str, const Ref<Widget>& widget) 
{
	// code here...
	auto ptr = DynamicCast<Background>(widget); // Example usage of DynamicCast
}

int main()
{
	Ref<Widget> widget(new Widget());
	Ref<Background> bkg(new Background());

	nerty("x", widget);
	// o2Scripts.GetGlobal().SetPropertyWrapper<float>(ScriptValue("test"),
	// 		Function<void(float)>([&](float xx) { testValue = xx; }),
	// 		Function<float()>([&]() { return testValue; }));
	// etc...

	return 0;
}{
    void EditorApplication::OnClosing()
    {
        mConfig = nullptr;
        mWindowsManager = nullptr;
        mBackground = nullptr;
        mBackSign = nullptr;
        mToolsPanel = nullptr;
        mMenuPanel = nullptr;
        mUIRoot = nullptr;
    }

    void EditorApplication::OnResizing()
    {
        mBackground->SetSize(o2Render.GetResolution() + Vec2F(20, 20));
        mBackSign->position = (Vec2F)(o2Render.GetResolution()).InvertedX() * 0.5f + Vec2F(40.0f, -85.0f);

        mConfig->OnWindowChange();
        mUIRoot->OnApplicationSized();
    }

    void EditorApplication::OnMoved()
    {
        mConfig->OnWindowChange();
    }

    void EditorApplication::ProcessFrame()
    {
        PushEditorScopeOnStack scope;

        mUpdateStep = mIsPlaying && (!isPaused || step);
        step = false;

        Application::ProcessFrame();

        mDrawCalls = mRender->GetDrawCallsCount();
        mDrawnPrimitives = mRender->GetDrawnPrimitives();
    }

    void EditorApplication::CheckPlayingSwitch()
    {
        if (!mPlayingChanged)
            return;

        ForcePopEditorScopeOnStack scope;

        if (mIsPlaying)
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

        mPlayingChanged = false;
    }

    void EditorApplication::InitalizeSystems()
    {
        mMainListenersLayer.SetEditorMode(true);

        Application::InitalizeSystems();
    }

    void EditorApplication::LoadUIStyle()
    {
        EditorUIStyleBuilder builder;
        builder.RebuildEditorUIManager("Editor UI styles", true, true);
    }

    void EditorApplication::PreUpdatePhysics()
    {
        ForcePopEditorScopeOnStack scope;

        Application::PreUpdatePhysics();
    }

    void EditorApplication::UpdatePhysics(float dt)
    {
        ForcePopEditorScopeOnStack scope;

        if (mUpdateStep)
            Application::UpdatePhysics(dt);
    }

    void EditorApplication::PostUpdatePhysics()
    {
        ForcePopEditorScopeOnStack scope;

        if (mUpdateStep)
            Application::PostUpdatePhysics();
    }

    void EditorApplication::UpdateScene(float dt)
    {
        ForcePopEditorScopeOnStack scope;

        if (mUpdateStep)
        {
            mScene->Update(dt);
            o2EditorSceneScreen.NeedRedraw();
        }
        else
        {
            mScene->UpdateAddedEntities();
            mScene->UpdateDestroyingEntities();
        }

        CheckPlayingSwitch();
    }

    void EditorApplication::FixedUpdateScene(float dt)
    {
        ForcePopEditorScopeOnStack scope;

        if (mUpdateStep)
            mScene->FixedUpdate(dt);
    }

    void EditorApplication::DrawScene()
    {
    }

    void EditorApplication::DrawUIManager()
    {
        PushEditorScopeOnStack scope;
        Application::DrawUIManager();
    }
}#include <Ref.h>

class EditorApplication : public Application
{
public:
	EditorApplication() {}

	void DrawDebug()
	{
		o2Debug.Draw(true);
	}

	void UpdateDebug(float dt)
	{
		if (mUpdateStep)
			o2Debug.Update(false, dt);

		o2Debug.Update(true, dt);
	}

	void UpdateEventSystem()
	{
		PushEditorScopeOnStack scope;
		Application::UpdateEventSystem();
	}

	void PostUpdateEventSystem()
	{
		PushEditorScopeOnStack scope;
		Application::PostUpdateEventSystem();
	}

	void OnUpdate(float dt)
	{
		mWindowsManager->Update(dt);
		mUIRoot->Update(dt);
		mToolsPanel->Update(dt);

		String currentScene = mLoadedScene ? mLoadedScene->GetPath() : String("");

		o2Application.windowCaption = String("o2 Editor: ") + currentScene +
			"; FPS: " + (String)((int)o2Time.GetFPS()) +
			" DC: " + (String)mDrawCalls +
			" Tris: " + (String)mDrawnPrimitives +
			" Cursor: " + (String)o2Input.GetCursorPos() +
			" JS: " + (String)(o2Scripts.GetUsedMemory() / 1024) + "kb";

		if (o2Input.IsKeyPressed('K'))
			o2Memory.DumpInfo();
	}

#undef DrawText

	void OnDraw()
	{
		PushEditorScopeOnStack scope;

		o2Render.Clear();

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

	void OnActivated()
	{
		//o2Assets.RebuildAssets();
	}

	void OnDeactivated()
	{
	}

private:
	Ref<WindowsManager> mWindowsManager;
	Ref<UIRoot> mUIRoot;
	Ref<ToolsPanel> mToolsPanel;
	Ref<Background> mBackground;
	Ref<BackSign> mBackSign;
	Ref<Scene> mLoadedScene;
	bool mUpdateStep;
	int mDrawCalls;
	int mDrawnPrimitives;
	Array<Ref<Action>> mActions;
};