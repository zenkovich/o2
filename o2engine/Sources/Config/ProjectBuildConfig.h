#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/String.h"

namespace o2
{
	class ProjectConfig;
	class AssetConfig;

	class ProjectBuildConfig
	{
	public:
		typedef Array<AssetConfig*> AssetsConfigsArr;

	protected:
		String         mName;
		ProjectConfig* mProjectConfig;

	public:
		ProjectBuildConfig(ProjectConfig* projConfig);
		~ProjectBuildConfig();

		String GetName() const;
		ProjectConfig* GetProjectConfigs() const;

	protected:
		void InitializeDefault(const String& configFilePath);
	};
}