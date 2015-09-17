#include "ProjectConfig.h"

#include "EngineSettings.h"
#include "Utils/Debug.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfig);

	IOBJECT_CPP(ProjectConfig);

	ProjectConfig::ProjectConfig()
	{
		InitializeProperties();

		String cfgFilePath = PROJECT_SETTINGS_FILE_PATH;

		DataNode data;
		if (!data.LoadFromFile(cfgFilePath))
		{
			o2Debug.LogError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath);
			InitializeDefault(cfgFilePath);
			return;
		}
		else Deserialize(data);
	}

	ProjectConfig::~ProjectConfig()
	{}

	String ProjectConfig::GetProjectName() const
	{
		return mInstance->mProjectName;
	}

	void ProjectConfig::SetProjectName(const String& name)
	{
		mInstance->mProjectName = name;
	}

	ProjectConfig::Platform ProjectConfig::GetPlatform() const
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
		DataNode data = Serialize();
		data.SaveToFile(configFilePath);
	}

	void ProjectConfig::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ProjectConfig, projectName, SetProjectName, GetProjectName);
		INITIALIZE_PROPERTY(ProjectConfig, currentPlatform, SetPlatform, GetPlatform);
	}
}