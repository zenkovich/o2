#include "o2/stdafx.h"
#include "ProjectConfig.h"

#include "o2/EngineSettings.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfig);

	ProjectConfig::ProjectConfig():
		mPlatform(GetEnginePlatform())
	{}

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

	void ProjectConfig::InitializeDefault(const String& configFilePath)
	{
		mProjectName = "Unnamed";
		DataDocument data;
		Serialize(data);
		data.SaveToFile(configFilePath);
	}
}

DECLARE_CLASS(o2::ProjectConfig);
