#pragma once

#include "Utils/String.h"
#include "Utils/Serializable.h"
#include "Utils/Singleton.h"

#include "Utils/Property.h"

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
		enum class Platform { Windows, MacOSX, iOS, Android };

	public:
		Property<String>   projectName;     // Project name property
		Property<Platform> currentPlatform; // Project platform property
		Getter<String>     projectPath;     // Project path location

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

		// Returns project path
		String GetProjectPath() const;

		// Sets new project path. Loads config from new path and will o2 works with new path
		void SetProjectPath(const String& path);

		SERIALIZABLE(ProjectConfig);

	protected:
		String   mProjectName; // Current project name @SERIALIZABLE
		Platform mPlatform;    // Current project target platform
		String   mProjectPath; // Current project location path

	protected:
		// Initializes config by default
		void InitializeDefault(const String& configFilePath);

		// Initializes properties
		void InitializeProperties();

		friend class AssetBuildSystem;
		friend class Application;
	};
}