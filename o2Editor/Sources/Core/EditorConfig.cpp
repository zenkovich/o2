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

CLASS_META(Editor::EditorConfig)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::Singleton<EditorConfig>);

	PROTECTED_FIELD(mConfigPath);
	PROTECTED_FIELD(mGlobalConfigPath);
	PROTECTED_FIELD(mConfigsLoaded);
	PROTECTED_FIELD(mProjectConfig);
	PROTECTED_FIELD(mGlobalConfig);

	PUBLIC_FUNCTION(DataNode&, GetGlobalUserData);
	PUBLIC_FUNCTION(DataNode&, GetProjectUserData);
	PROTECTED_FUNCTION(void, SaveGlobalConfigs);
	PROTECTED_FUNCTION(void, SaveProjectConfigs);
	PROTECTED_FUNCTION(void, LoadConfigs);
	PROTECTED_FUNCTION(void, LoadProjectConfig);
	PROTECTED_FUNCTION(void, LoadGlobalConfig);
	PROTECTED_FUNCTION(void, OnWindowChange);
}
END_META;

CLASS_META(Editor::EditorConfig::GlobalConfig)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mLastOpenedProjectpath).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDefaultLayout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAvailableLayouts).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mUserData).SERIALIZABLE_ATTRIBUTE();
}
END_META;

CLASS_META(Editor::EditorConfig::ProjectConfig)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mWindowSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWindowPosition).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMaximized).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mLayout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mUserData).SERIALIZABLE_ATTRIBUTE();
}
END_META;
