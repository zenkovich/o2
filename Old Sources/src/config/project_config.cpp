#include "project_config.h"

#include "build_config.h"

OPEN_O2_NAMESPACE

ProjectConfig::ProjectConfig():
mBuildConfig(NULL), mAssetsUsesMetaIds(true)
{
	String cfgFilePath = PROJECT_CONFIG_FILE_PATH;

	Serializer serializer;
	if (!serializer.Load(cfgFilePath))
	{
		LogError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath.c_str());
		InitializeDefault(cfgFilePath);
		return;
	}
	else serializer.Serialize(this, "config");

	mBuildConfig = mnew ProjectBuildConfig(this);
}

ProjectConfig::~ProjectConfig()
{
	SafeRelease(mBuildConfig);
}

String ProjectConfig::GetProjectName() const
{
	return mProjectName;
}

ProjectBuildConfig* ProjectConfig::GetBuildConfig() const
{
	return mBuildConfig;
}

void ProjectConfig::InitializeDefault(const String& configFilePath)
{
	mProjectName = "unnamed";

	Serializer serializer;
	serializer.Serialize(this, "config");
	serializer.Save(configFilePath);
}

bool ProjectConfig::Serialize(Serializer* serializer)
{
	SERIALIZE_ID(mProjectName, "projectName");
	SERIALIZE_ID(mAssetsUsesMetaIds, "assetsUsingMetaIds");

	return true;
}


CLOSE_O2_NAMESPACE