#include "o2Editor/stdafx.h"
#include "EditorConfig.h"

#include "o2/Application/Application.h"
#include "o2/Config/ProjectConfig.h"
#include "o2Editor/Core/WindowsSystem/DockWindowPlace.h"
#include "o2Editor/Core/WindowsSystem/DockableWindow.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "WindowsSystem/IEditorWindow.h"
#include "MenuPanel.h"
#include "ToolsPanel.h"


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

	DataNode& EditorConfig::GetGlobalUserData()
	{
		return mGlobalConfig.mUserData;
	}

	DataNode& EditorConfig::GetProjectUserData()
	{
		return mProjectConfig.mUserData;
	}

	void EditorConfig::SaveGlobalConfigs()
	{
		DataNode data;

		mGlobalConfig.mLastOpenedProjectpath = o2Config.GetProjectPath();
		mGlobalConfig.mAvailableLayouts = o2EditorWindows.mAvailableLayouts;

		data = mGlobalConfig;
		data.SaveToFile(mGlobalConfigPath);
	}

	void EditorConfig::SaveProjectConfigs()
	{
		mProjectConfig.mLayout = o2EditorWindows.GetWindowsLayout();

		DataNode data;
		data = mProjectConfig;
		data.SaveToFile(o2Config.GetProjectPath() + mConfigPath);
	}

	void EditorConfig::LoadConfigs()
	{
		LoadGlobalConfig();
		LoadProjectConfig();

		mConfigsLoaded = true;
	}

	void EditorConfig::LoadProjectConfig()
	{
		DataNode data;
		o2Config.SetProjectPath(mGlobalConfig.mLastOpenedProjectpath);

		if (data.LoadFromFile(o2Config.GetProjectPath() + mConfigPath))
			mProjectConfig = data;
		else
		{
			mProjectConfig = ProjectConfig();
		}
	}

	void EditorConfig::LoadGlobalConfig()
	{
		DataNode data;
		if (data.LoadFromFile(mGlobalConfigPath))
			mGlobalConfig = data;
		else
		{
			mGlobalConfig = GlobalConfig();
			mGlobalConfig.mLastOpenedProjectpath = GetProjectPath();
		}
	}

	void EditorConfig::OnWindowChange()
	{
		mProjectConfig.mWindowPosition = o2Application.GetWindowPosition();
		mProjectConfig.mWindowSize = o2Application.GetWindowSize();
		mProjectConfig.mMaximized = o2Application.IsMaximized();
	}

}

DECLARE_CLASS(Editor::EditorConfig);

DECLARE_CLASS(Editor::EditorConfig::GlobalConfig);

DECLARE_CLASS(Editor::EditorConfig::ProjectConfig);
