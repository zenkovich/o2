#include "o2/stdafx.h"
#include "ProjectConfig.h"

#include "o2/EngineSettings.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfig);

	ProjectConfig::ProjectConfig():
		mPlatform(GetEnginePlatform())
	{
		SetProjectPath(::GetProjectPath());
	}

	ProjectConfig::~ProjectConfig()
	{}

	String ProjectConfig::GetProjectName() const
	{
		return mProjectName;
	}

	void ProjectConfig::SetProjectName(const String& name)
	{
		mProjectName = name;
	}

	Platform ProjectConfig::GetPlatform() const
	{
		return mPlatform;
	}

	void ProjectConfig::SetPlatform(Platform platform)
	{
		mPlatform = platform;
	}

	String ProjectConfig::GetProjectPath() const
	{
		return mProjectPath;
	}

	void ProjectConfig::SetProjectPath(const String& path)
	{
		String cfgFilePath = path + GetProjectSettingFileLocalPath();

		DataNode data;
		if (!data.LoadFromFile(cfgFilePath))
		{
			o2Debug.LogError("Failed to load Project Config. Path: " + cfgFilePath + ". Initializing default values.");
			InitializeDefault(cfgFilePath);
			return;
		}
		else Deserialize(data);

		mProjectPath = path;
	}

	void ProjectConfig::InitializeDefault(const String& configFilePath)
	{
		mProjectName = "Unnamed";
		DataNode data = Serialize();
		data.SaveToFile(configFilePath);
	}
}

DECLARE_CLASS(o2::ProjectConfig);
