#include "ProjectConfig.h"

#include "EngineSettings.h"
#include "Utils/Debug.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfigStuff);

	IOBJECT_CPP(ProjectConfigStuff);

	ProjectConfigStuff::ProjectConfigStuff()
	{
		InitializeProperties();

		String cfgFilePath = PROJECT_SETTINGS_FILE_PATH;

		DataNode data;
		if (!data.LoadFromFile(cfgFilePath))
		{
			Debug.LogError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath.Data());
			InitializeDefault(cfgFilePath);
			return;
		}
		else Deserialize(data);
	}

	ProjectConfigStuff::~ProjectConfigStuff()
	{
	}

	String ProjectConfigStuff::GetProjectName() const
	{
		return mInstance->mProjectName;
	}

	void ProjectConfigStuff::SetProjectName(const String& name)
	{
		mInstance->mProjectName = name;
	}

	ProjectConfigStuff::Platform ProjectConfigStuff::GetPlatform() const
	{
		return mInstance->mPlatform;
	}

	void ProjectConfigStuff::SetPlatform(Platform platform)
	{
		mInstance->mPlatform;
	}

	void ProjectConfigStuff::InitializeDefault(const String& configFilePath)
	{
		mProjectName = "Unnamed";
		DataNode data = Serialize();
		data.SaveToFile(configFilePath);
	}

	void ProjectConfigStuff::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ProjectConfigStuff, ProjectName, SetProjectName, GetProjectName);
		INITIALIZE_PROPERTY(ProjectConfigStuff, CurrentPlatform, SetPlatform, GetPlatform);
	}
}