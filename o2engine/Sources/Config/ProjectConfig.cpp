#include "ProjectConfig.h"

#include "EngineSettings.h"
#include "Utils/Debug.h"
#include "Config/ProjectBuildConfig.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfig);

	ProjectConfig::ProjectConfig():
		mBuildConfig(NULL), mAssetsUsesMetaIds(true)
	{
		String cfgFilePath = PROJECT_CONFIG_FILE_PATH;

		Serializer serializer;
		if (!serializer.Load(cfgFilePath))
		{
			Debug::LogError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath.Data());
			InitializeDefault(cfgFilePath);
			return;
		}
		else serializer.Serialize(this, "Config");

		mBuildConfig = new ProjectBuildConfig(this);
	}

	ProjectConfig::~ProjectConfig()
	{
		delete mBuildConfig;
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
		mProjectName = "Unnamed";

		Serializer serializer;
		serializer.Serialize(this, "Config");
		serializer.Save(configFilePath);
	}

	SERIALIZE_METHOD_IMPL(ProjectConfig)
	{
		SERIALIZE_ID(mProjectName, "ProjectName");
		SERIALIZE_ID(mAssetsUsesMetaIds, "AssetsUsingMetaIds");

		return true;
	}
}