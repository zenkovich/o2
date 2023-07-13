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

		mGlobalConfig.mAvailableLayouts = o2EditorWindows.mAvailableLayouts;

		data = mGlobalConfig;
		data.SaveToFile(mGlobalConfigPath);
	}

	void EditorConfig::SaveProjectConfigs()
	{
		mProjectConfig.mLayout = o2EditorWindows.GetWindowsLayout();

		DataDocument data;
		data = mProjectConfig;
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
			mProjectConfig = data;
		else
		{
			mProjectConfig = ProjectConfig();
		}
	}

	void EditorConfig::LoadGlobalConfig()
	{
		DataDocument data;
		if (data.LoadFromFile(mGlobalConfigPath))
			mGlobalConfig = data;
		else
			mGlobalConfig = GlobalConfig();
	}

	void EditorConfig::OnWindowChange()
	{
		mProjectConfig.mWindowPosition = o2Application.GetWindowPosition();
		mProjectConfig.mWindowSize = o2Application.GetWindowSize();
		mProjectConfig.mMaximized = o2Application.IsMaximized();
	}

}
// --- META ---

DECLARE_CLASS(Editor::EditorConfig);

DECLARE_CLASS(Editor::EditorConfig::GlobalConfig);

DECLARE_CLASS(Editor::EditorConfig::ProjectConfig);
// --- END META ---
