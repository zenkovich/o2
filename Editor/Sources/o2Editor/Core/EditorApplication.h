#pragma once

#include "o2/Application/Application.h"
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

	protected:
		const bool mNeedRebuildWndStyle = true; // Is need to rebuild dockable windows style

		Sprite* mBackground; // Background sprite
		Sprite* mBackSign;   // Background o2 signature

		UIRoot*         mUIRoot;         // Root editor UI
		WindowsManager* mWindowsManager; // Windows manager
		EditorConfig*   mConfig;         // Application configuration
		ToolsPanel*     mToolsPanel;     // Tools panel
		MenuPanel*      mMenuPanel;      // Menu panel

		Properties* mProperties; // Properties manager

		String mLoadedScene; // Current loaded scene

		int mDrawCalls; // Draw calls count, stored before beginning rendering

	protected:
		// Check style rebuilding and loads editor UI style
		void LoadUIStyle();

		// Calling on updating
		void OnUpdate(float dt);

		// Calling on drawing
		void OnDraw();

		// Calling when application activated
		void OnActivated();

		// Calling when application deactivated
		void OnDeactivated();

		// Calling when application is starting
		void OnStarted();

		// Initializes menu
		void InitializeMenu();

		// Calling when application is closing
		void OnClosing();

		// Calling when application window resized. Ignoring on mobiles/tablets
		void OnResizing();

		// Calling when application window moved. Ignoring on mobiles/tablets
		void OnMoved();

		// Processing frame update, drawing and input messages without scene
		void ProcessFrame();
	};
}
