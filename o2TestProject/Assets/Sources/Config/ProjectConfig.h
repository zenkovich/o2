#pragma once

#include "Utils/String.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"


namespace o2
{
	class ProjectBuildConfig;

#define o2ProjectConfig ProjectConfigStuff::Instance()

	class ProjectConfigStuff: public ISerializable, public Singleton<ProjectConfigStuff>
	{
		friend class AssetBuildSystem;
		friend class IApplication;

	public:
		enum class Platform { Windows, MacOSX, iOS, Android };

	protected:
		String   mProjectName;
		Platform mPlatform;

	public:
		ProjectConfigStuff();
		~ProjectConfigStuff();

		Property<String>   ProjectName;
		Property<Platform> CurrentPlatform;

		String GetProjectName() const;
		void SetProjectName(const String& name);

		Platform GetPlatform() const;
		void SetPlatform(Platform platform);

		SERIALIZABLE_IMPL(ProjectConfigStuff);

		IOBJECT(ProjectConfigStuff)
		{
			SRLZ_FIELD(mProjectName);
			SRLZ_FIELD(mPlatform);
		}

	protected:
		void InitializeDefault(const String& configFilePath);

		void InitializeProperties();
	};
}