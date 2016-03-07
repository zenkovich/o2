#include "EditorConfig.h"

#include "Application/Application.h"
#include "Config/ProjectConfig.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "UI/UIManager.h"
#include "UI/Window.h"
#include "WindowsSystem/IEditorWindow.h"
#include "MenuPanel.h"
#include "ToolsPanel.h"


namespace Editor
{
	EditorConfig::EditorConfig()
	{}

	EditorConfig::~EditorConfig()
	{
		SaveProjectConfigs();
		SaveGlobalConfigs();
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

		if (mProjectConfig.mMaximized)
		{
			o2Application.Maximize();
		}
		else
		{
			Vec2I pos = mProjectConfig.mWindowPosition;
			o2Application.SetWindowSize(mProjectConfig.mWindowSize);
			o2Application.SetWindowPosition(pos);
			mProjectConfig.mWindowPosition = pos;
		}

		o2EditorWindows.SetWindowsLayout(mProjectConfig.mLayout);
	}

	void EditorConfig::LoadGlobalConfig()
	{
		DataNode data;
		if (data.LoadFromFile(mGlobalConfigPath))
			mGlobalConfig = data;
		else
		{
			mGlobalConfig = GlobalConfig();
			mGlobalConfig.mLastOpenedProjectpath = PROJECT_PATH;
		}

		o2EditorWindows.mAvailableLayouts = mGlobalConfig.mAvailableLayouts;
		o2EditorTools.UpdateWndLayoutSchemas();
	}

	void EditorConfig::OnWindowChange()
	{
		mProjectConfig.mWindowPosition = o2Application.GetWindowPosition();
		mProjectConfig.mWindowSize = o2Application.GetWindowSize();
		mProjectConfig.mMaximized = o2Application.IsMaximized();
	}

}