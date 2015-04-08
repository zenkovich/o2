#include "build_config.h"

#include "asset_config.h"
#include "util\serialize_util.h"

OPEN_O2_NAMESPACE

ProjectBuildConfig::ProjectBuildConfig(ProjectConfig* projConfig):
mProjectConfig(projConfig)
{
	String cfgFilePath = BUILD_CONFIG_FILE_PATH;

	Serializer serializer;
	if (!serializer.Load(cfgFilePath))
	{
		LogError("Failed to load build config: %s. Initializing default.", cfgFilePath.c_str());
		InitializeDefault(cfgFilePath);
		return;
	}

	serializer.Serialize(mName, "buildName");
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
	mName = "unnamed";

	Serializer serializer();
	serializer.Serialize(mName, "buildName");
	serializer.Save(configFilePath);
}

CLOSE_O2_NAMESPACE