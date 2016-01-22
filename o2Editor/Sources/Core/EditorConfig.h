#pragma once

#include "Core/WindowsSystem/WindowsLayout.h"
#include "UI/WidgetLayout.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

class UIDockWindowPlace;

// Editor configuration access macros
#define o2EditorConfig EditorConfig::Instance()

// -------------------------
// Application configuration
// -------------------------
class EditorConfig: public ISerializable, public Singleton<EditorConfig>
{
public:
	// Default constructor. Loads data and applies to application 
	EditorConfig();

	// Destructor. Saves application configuration
	~EditorConfig();

	SERIALIZABLE(EditorConfig);

public:
	class GlobalConfig: public ISerializable
	{
	public:
		typedef Dictionary<String, WindowsLayout> WndLayoutsDict;
	public:
		String         mLastOpenedProjectpath; // Last opened project path @SERIALIZABLE
		WindowsLayout  mDefaultLayout;         // Default windows layout, using in resetting @SERIALIZABLE
		WndLayoutsDict mAvailableLayouts;      // Available windows layouts @SERIALIZABLE

		SERIALIZABLE(GlobalConfig);
	};

	class ProjectConfig: public ISerializable
	{
	public:
		Vec2I         mWindowSize = Vec2I(800, 600); // Application window size @SERIALIZABLE
		Vec2I         mWindowPosition;               // Application window position @SERIALIZABLE
		bool          mMaximized = true;             // Is application window is maximized @SERIALIZABLE
		WindowsLayout mLayout;                       // Windows layout @SERIALIZABLE

		SERIALIZABLE(ProjectConfig);
	};
protected:

	const String mConfigPath = "EditorConfig.xml";
	const String mGlobalConfigPath = "Config.xml";

	ProjectConfig mProjectConfig; // Project editor config
	GlobalConfig  mGlobalConfig;  // Global editor config for all projects

protected:
	// Saves global configs
	void SaveGlobalConfigs();

	// Saves project configs
	void SaveProjectConfigs();

	// Loads and applies configs
	void LoadConfigs();

	// Loads project configs
	void LoadProjectConfig();

	// Loads global configs
	void LoadGlobalConfig();

	// Updates window configs
	void OnWindowChange();

	friend class EditorApplication;
	friend class MenuPanel;
	friend class WindowsManager;
};