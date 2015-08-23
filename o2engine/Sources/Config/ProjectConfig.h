#pragma once

#include "Utils/String.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"

namespace o2
{
	class ProjectBuildConfig;

	class ProjectConfig: public ISerializable, public Singleton<ProjectConfig>
	{
		friend class AssetBuildSystem;
		friend class IApplication;

	public:
		enum class Platform { Windows, MacOSX, iOS, Android };

	protected:
		String   mProjectName;
		bool     mAssetsUsesMetaIds;
		Platform mPlatform;

	protected:
		ProjectConfig();

	public:
		~ProjectConfig();

		static Property<String>   ProjectName;
		static Property<Platform> CurrentPlatform;

		static String GetProjectName();
		static void SetProjectName(const String& name);

		static Platform GetPlatform();
		static void SetPlatform(Platform platform);

		SERIALIZABLE_FIELDS(ProjectConfig)
			FIELD(mProjectName)
			FIELD(mPlatform)
			FIELD(mAssetsUsesMetaIds);
		END_SERIALIZABLE_FIELDS;

	protected:
		void InitializeDefault(const String& configFilePath);

		String GetProjectNameProp() const;
		void SetProjectNameProp(const String& name);

		Platform GetPlatformProp();
		void SetPlatformProp(Platform platform);

		void InitializeProperties();
	};
}