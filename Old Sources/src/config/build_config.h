#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

class ProjectConfig;
class AssetConfig;

class ProjectBuildConfig
{
public:
	typedef Array<AssetConfig*> AssetsConfigsArr;

protected:
	String           mName;
	ProjectConfig*   mProjectConfig;

public:
	ProjectBuildConfig(ProjectConfig* projConfig);
	~ProjectBuildConfig();

	String GetName() const;
	ProjectConfig* GetProjectConfigs() const;

protected:
	void InitializeDefault(const String& configFilePath);
};

CLOSE_O2_NAMESPACE
