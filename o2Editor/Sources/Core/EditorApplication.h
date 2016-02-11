#pragma once

#include "Animation/Animatable.h"
#include "Application/Application.h"
#include "EditorConfig.h"
#include "Render/Sprite.h"

using namespace o2;

class WindowsManager;
class EditorConfig;
class ToolsPanel;
class MenuPanel;

namespace o2
{
	class UIMenuPanel;
}

class IEditorAction;

// Editor application access macros
#define o2EditorApplication (*(EditorApplication*)(Application::InstancePtr()))

// ------------------
// Editor application
// ------------------
class EditorApplication: public Application
{
public:
	typedef Vector<IEditorAction*> EditorActionsVec;

public:
	// Default constructor. Initializes all editor components
	EditorApplication();

	// Destructor
	~EditorApplication();

	// Returns last action name
	String GetLastActionName() const;

	// Returns next forward action name
	String GetNextForwardActionName() const;

	// Undo last action
	void UndoAction();

	// Redo next action
	void RedoAction();

	// Calls when action was done
	void DoneAction(IEditorAction* action);

protected:
	Sprite*            mBackground;     // Background sprite
	Sprite*            mBackSign;       // Background o2 signature

	WindowsManager*    mWindowsManager; // Windows manager
	EditorConfig*      mConfig;         // Application configuration
	ToolsPanel*        mToolsPanel;     // Tools panel
	MenuPanel*         mMenuPanel;      // Menu panel

	EditorActionsVec   mActions;        // Done actions
	EditorActionsVec   mForwardActions; // Forward actions, what you can redo

protected:
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
