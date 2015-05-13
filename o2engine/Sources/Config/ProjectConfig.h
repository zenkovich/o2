#pragma once

#include "Utils/String.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

namespace o2
{
	class ProjectBuildConfig;

	class ProjectConfig: public Serializable, public Singleton<ProjectConfig>
	{
		friend class AssetBuildSystem;

		String              mProjectName;
		bool                mAssetsUsesMetaIds;
		ProjectBuildConfig* mBuildConfig;

	public:
		ProjectConfig();
		~ProjectConfig();

		String GetProjectName() const;
		ProjectBuildConfig* GetBuildConfig() const;

		SERIALIZE_METHODS(ProjectConfig);

	protected:
		void InitializeDefault(const String& configFilePath);
	};
}