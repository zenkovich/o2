#pragma once

#include "o2/Application/Application.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2/Render/Sprite.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2/Assets/Types/SceneAsset.h"

using namespace o2;

namespace o2
{
	class MenuPanel;
}

// Editor application access macros
#define o2EditorApplication (*(EditorApplication*)(Application::InstancePtr()))

namespace Editor
{
	FORWARD_CLASS_REF(EditorConfig);
	FORWARD_CLASS_REF(IAction);
	FORWARD_CLASS_REF(MenuPanel);
	FORWARD_CLASS_REF(Properties);
	FORWARD_CLASS_REF(ToolsPanel);
	FORWARD_CLASS_REF(UIRoot);
	FORWARD_CLASS_REF(WindowsManager);

	// ------------------
	// Editor application
	// ------------------
	class EditorApplication: public Application, public ActionsList
	{
	public:
		bool isPaused = false;  // Is editor scene paused
		bool step = false;      // Is needed to do one update step

	public:
		// Default constructor. Initializes all editor components
		EditorApplication();

		// Destructor
		~EditorApplication();

		// Returns current scene name
		const String& GetLoadedSceneName() const;

		// Loads scene from file
		void LoadScene(const AssetRef<SceneAsset>& scene);

		// Saves current scene
		void SaveScene();

		// Saves scene from file
		void SaveSceneAs(const String& path);

		// Makes new scene
		void MakeNewScene();

		// Returns is current scene was changed
		bool IsSceneChanged() const;

		// Runs or stops scene playing. Saving scene into temporary file before playing, restoring after stopping
		void SetPlaying(bool playing);

		// Is scene playing
		bool IsPlaying() const;

		REF_COUNTERABLE_IMPL(Application, ActionsList);

	protected:
        Ref<Sprite> mBackground; // Background sprite
        Ref<Sprite> mBackSign;   // Background o2 signature

        Ref<UIRoot>         mUIRoot;         // Root editor UI
        Ref<WindowsManager> mWindowsManager; // Windows manager
        Ref<EditorConfig>   mConfig;         // Application configuration
        Ref<ToolsPanel>     mToolsPanel;     // Tools panel
        Ref<MenuPanel>      mMenuPanel;      // Menu panel

        Ref<Properties> mProperties; // Properties manager

		AssetRef<SceneAsset> mLoadedScene; // Current loaded scene

		DataDocument mSceneDump; // Scene dump, created before playing

		bool mIsPlaying = false;      // Is editor scene playing
		bool mPlayingChanged = false; // True when need to update playing mode on update
		bool mUpdateStep = false;     // True when frame updating available on this frame

		int mDrawCalls = 0;       // Draw calls count, stored before beginning rendering
		int mDrawnPrimitives = 0; // DRaw promitives at frame

	protected:
		// Initializing all systems and log. Sets up editor settings for some systems
		void InitalizeSystems() override;

		// Check style rebuilding and loads editor UI style
		void LoadUIStyle();

		// Before update physics
		void PreUpdatePhysics() override;

		// Updates physics
		void UpdatePhysics(float dt) override;

		// After update physics
		void PostUpdatePhysics() override;

		// Updates scene
		void UpdateScene(float dt) override;

		// Updates scene with fixed delta time
		void FixedUpdateScene(float dt) override;

		// Draws scene
		void DrawScene() override;

		// Draws UI manager
		void DrawUIManager() override;

		// Draw debug empty stub
		void DrawDebug() override;

		// Updates debug inly in play mode
		void UpdateDebug(float dt) override;

		// Updates event system
		void UpdateEventSystem() override;

		// Post updates event system
		void PostUpdateEventSystem() override;

		// Calling on updating
		void OnUpdate(float dt) override;

		// Calling on drawing
		void OnDraw() override;

		// Calling when application activated
		void OnActivated() override;

		// Calling when application deactivated
		void OnDeactivated() override;

		// Calling when application is starting
		void OnStarted() override;

		// Calling when application is closing
		void OnClosing() override;

		// Calling when application window resized. Ignoring on mobiles/tablets
		void OnResizing() override;

		// Calling when application window moved. Ignoring on mobiles/tablets
		void OnMoved() override;

		// Processing frame update, drawing and input messages without scene
		void ProcessFrame() override;

		// Checks that required to switch playing mode
		void CheckPlayingSwitch();
	};
}
