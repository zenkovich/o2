#pragma once

#include "public.h"
#include "app/application.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class ProjectBuildConfig;

class ProjectConfig: public Serializable
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

	SERIALIZBLE_METHODS(ProjectConfig);

protected:
	void InitializeDefault(const String& configFilePath);
};

CLOSE_O2_NAMESPACE
