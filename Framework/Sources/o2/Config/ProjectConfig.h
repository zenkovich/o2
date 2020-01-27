#pragma once

#include "o2/Utils/Types/String.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"

#include "o2/Utils/Property.h"

// Project configuration access macros
#define o2Config o2::ProjectConfig::Instance()

namespace o2
{
	class ProjectBuildConfig;

	// ---------------------
	// Project configuration
	// ---------------------
	class ProjectConfig: public ISerializable, public Singleton<ProjectConfig>
	{
	public:
		PROPERTIES(ProjectConfig);
		PROPERTY(String, projectName, SetProjectName, GetProjectName); // Project name property
		PROPERTY(Platform, currentPlatform, SetPlatform, GetPlatform); // Project platform property
		GETTER(String, projectPath, GetProjectPath);                   // Project path location

	public:
		// Default constructor
		ProjectConfig();

		// Destructor
		~ProjectConfig();

		// Returns project name 
		String GetProjectName() const;

		// Sets project name
		void SetProjectName(const String& name);

		// Returns platform
		Platform GetPlatform() const;

		// Sets platform
		void SetPlatform(Platform platform);

		SERIALIZABLE(ProjectConfig);

	protected:
		String   mProjectName; // Current project name @SERIALIZABLE
		Platform mPlatform;    // Current project target platform

	protected:
		// Initializes config by default
		void InitializeDefault(const String& configFilePath);

		friend class AssetBuildSystem;
		friend class Application;
	};
}

CLASS_BASES_META(o2::ProjectConfig)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::Singleton<ProjectConfig>);
}
END_META;
CLASS_FIELDS_META(o2::ProjectConfig)
{
	PUBLIC_FIELD(projectName);
	PUBLIC_FIELD(currentPlatform);
	PUBLIC_FIELD(projectPath);
	PROTECTED_FIELD(mProjectName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPlatform);
	PROTECTED_FIELD(mProjectPath);
}
END_META;
CLASS_METHODS_META(o2::ProjectConfig)
{

	PUBLIC_FUNCTION(String, GetProjectName);
	PUBLIC_FUNCTION(void, SetProjectName, const String&);
	PUBLIC_FUNCTION(Platform, GetPlatform);
	PUBLIC_FUNCTION(void, SetPlatform, Platform);
	PUBLIC_FUNCTION(String, GetProjectPath);
	PUBLIC_FUNCTION(void, SetProjectPath, const String&);
	PROTECTED_FUNCTION(void, InitializeDefault, const String&);
}
END_META;
