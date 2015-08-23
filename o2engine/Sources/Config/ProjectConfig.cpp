#include "ProjectConfig.h"

#include "EngineSettings.h"
#include "Utils/Debug.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfig);

	ProjectConfig::ProjectConfig():
		mAssetsUsesMetaIds(true)
	{
		InitializeProperties();

		String cfgFilePath = PROJECT_CONFIG_FILE_PATH;

		DataDoc data;
		if (!data.LoadFromFile(cfgFilePath))
		{
			Debug::LogError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath.Data());
			InitializeDefault(cfgFilePath);
			return;
		}
		else Deserialize(data);
	}

	ProjectConfig::~ProjectConfig()
	{
	}

	Property<String> ProjectConfig::ProjectName;

	Property<ProjectConfig::Platform> ProjectConfig::CurrentPlatform;

	String ProjectConfig::GetProjectName()
	{
		return mInstance->mProjectName;
	}

	void ProjectConfig::SetProjectName(const String& name)
	{
		mInstance->mProjectName = name;
	}

	ProjectConfig::Platform ProjectConfig::GetPlatform()
	{
		return mInstance->mPlatform;
	}

	void ProjectConfig::SetPlatform(Platform platform)
	{
		mInstance->mPlatform;
	}

	void ProjectConfig::InitializeDefault(const String& configFilePath)
	{
		mProjectName = "Unnamed";
		DataDoc data = Serialize();
		data.SaveToFile(configFilePath);
	}

	String ProjectConfig::GetProjectNameProp() const
	{
		return mProjectName;
	}

	void ProjectConfig::SetProjectNameProp(const String& name)
	{
		mProjectName = name;
	}

	ProjectConfig::Platform ProjectConfig::GetPlatformProp()
	{
		return mPlatform;
	}

	void ProjectConfig::SetPlatformProp(Platform platform)
	{
		mPlatform = platform;
	}

	void ProjectConfig::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ProjectConfig, ProjectName, SetProjectNameProp, GetProjectNameProp);
		INITIALIZE_PROPERTY(ProjectConfig, CurrentPlatform, SetPlatformProp, GetPlatformProp);
	}
}