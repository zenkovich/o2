#include "ProjectBuildConfig.h"

#include "EngineSettings.h"
#include "Utils/Serialization.h"
#include "Utils/Debug.h"

namespace o2
{

	ProjectBuildConfig::ProjectBuildConfig(ProjectConfig* projConfig):
		mProjectConfig(projConfig)
	{
		String cfgFilePath = BUILD_CONFIG_FILE_PATH;

		Serializer serializer;
		if (!serializer.Load(cfgFilePath))
		{
			Debug::LogError("Failed to load build config: %s. Initializing default.", cfgFilePath.Data());
			InitializeDefault(cfgFilePath);
			return;
		}

		serializer.Serialize(mName, "BuildName");
	}

	ProjectBuildConfig::~ProjectBuildConfig()
	{
	}

	String ProjectBuildConfig::GetName() const
	{
		return mName;
	}

	ProjectConfig* ProjectBuildConfig::GetProjectConfigs() const
	{
		return mProjectConfig;
	}

	void ProjectBuildConfig::InitializeDefault(const String& configFilePath)
	{
		mName = "Unnamed";

		Serializer serializer;
		serializer.Serialize(mName, "BuildName");
		serializer.Save(configFilePath);
	}
}