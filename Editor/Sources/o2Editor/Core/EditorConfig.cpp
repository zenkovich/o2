#include "o2Editor/stdafx.h"
#include "EditorConfig.h"

#include "o2/Application/Application.h"
#include "o2/Config/ProjectConfig.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2Editor/Core/MenuPanel.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/WindowsSystem/DockWindowPlace.h"
#include "o2Editor/Core/WindowsSystem/DockableWindow.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"


namespace Editor
{
	EditorConfig::EditorConfig()
	{}

	EditorConfig::~EditorConfig()
	{
		if (mConfigsLoaded)
		{
			SaveProjectConfigs();
			SaveGlobalConfigs();
		}
	}

	void EditorConfig::SaveGlobalConfigs()
	{
		DataDocument data;

		globalConfig.mAvailableLayouts = o2EditorWindows.mAvailableLayouts;

		data = globalConfig;
		data.SaveToFile(mGlobalConfigPath);
	}

	void EditorConfig::SaveProjectConfigs()
	{
		projectConfig.mLayout = o2EditorWindows.GetWindowsLayout();

		DataDocument data;
		data = projectConfig;
		data.SaveToFile(mConfigPath);
	}

	void EditorConfig::LoadConfigs()
	{
		LoadGlobalConfig();
		LoadProjectConfig();

		mConfigsLoaded = true;
	}

	void EditorConfig::LoadProjectConfig()
	{
		DataDocument data;

		if (data.LoadFromFile(mConfigPath))
			projectConfig = data;
		else
		{
			projectConfig = ProjectConfig();
		}
	}

	void EditorConfig::LoadGlobalConfig()
	{
		DataDocument data;
		if (data.LoadFromFile(mGlobalConfigPath))
			globalConfig = data;
		else
			globalConfig = GlobalConfig();
	}

	void EditorConfig::OnWindowChange()
	{
		projectConfig.mWindowPosition = o2Application.GetWindowPosition();
		projectConfig.mWindowSize = o2Application.GetWindowSize();
		projectConfig.mMaximized = o2Application.IsMaximized();
	}

}
// --- META ---

DECLARE_CLASS(Editor::EditorConfig, Editor__EditorConfig);

DECLARE_CLASS(Editor::EditorConfig::GlobalConfig, Editor__EditorConfig__GlobalConfig);

DECLARE_CLASS(Editor::EditorConfig::ProjectConfig, Editor__EditorConfig__ProjectConfig);
// --- END META ---