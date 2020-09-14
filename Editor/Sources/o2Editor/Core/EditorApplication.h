#pragma once

#include "o2/Application/Application.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2/Render/Sprite.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/EditorConfig.h"

using namespace o2;

namespace o2
{
	class MenuPanel;
}

// Editor application access macros
#define o2EditorApplication (*(EditorApplication*)(Application::InstancePtr()))

namespace Editor
{
	class EditorConfig;
	class IAction;
	class MenuPanel;
	class Properties;
	class ToolsPanel;
	class UIRoot;
	class WindowsManager;

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
		void LoadScene(const String& name);

		// Saves scene from file
		void SaveScene(const String& name);

		// Makes new scene
		void MakeNewScene();

		// Returns is current scene was changed
		bool IsSceneChanged() const;

		// Runs or stops scene playing. Saving scene into temprorary file before playing, restoring after stopping
		void SetPlaying(bool playing);

		// Is scene playing
		bool IsPlaying() const;

		// Returns game view render target listeners layer
		CursorAreaEventListenersLayer& GetGameViewListenersLayer();

	protected:
		Sprite* mBackground; // Background sprite
		Sprite* mBackSign;   // Background o2 signature

		UIRoot*         mUIRoot;         // Root editor UI
		WindowsManager* mWindowsManager; // Windows manager
		EditorConfig*   mConfig;         // Application configuration
		ToolsPanel*     mToolsPanel;     // Tools panel
		MenuPanel*      mMenuPanel;      // Menu panel

		Properties* mProperties; // Properties manager

		CursorAreaEventListenersLayer mListenersLayer; // Game view render target listeners layer

		String mLoadedScene; // Current loaded scene

		DataDocument mSceneDump; // Scene dump, created before playing

		bool mIsPlaying = false;  // Is editor scene playing
		bool mUpdateStep = false; // True when frame updating available on this frame

		int mDrawCalls; // Draw calls count, stored before beginning rendering

	protected:
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
	};
}
