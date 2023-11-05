#include "o2/stdafx.h"
#include "ProjectConfig.h"

#include "o2/EngineSettings.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{
    DECLARE_SINGLETON(ProjectConfig);

    ProjectConfig::ProjectConfig():
        mPlatform(GetEnginePlatform())
    {
        Load();
    }

    ProjectConfig::~ProjectConfig()
    {
        Save();
    }

    String ProjectConfig::GetProjectName() const
    {
        return mProjectName;
    }

    void ProjectConfig::SetProjectName(const String& name)
    {
        mProjectName = name;
    }

    Platform ProjectConfig::GetPlatform() const
    {
        return mPlatform;
    }

    void ProjectConfig::SetPlatform(Platform platform)
    {
        mPlatform = platform;
    }

    void ProjectConfig::Load()
    {
        DataDocument data;
        if (!data.LoadFromFile(GetProjectSettingPath()))
            return;

        Deserialize(data);
    }

    void ProjectConfig::Save() const
    {
        DataDocument data;
        Serialize(data);
        data.SaveToFile(GetProjectSettingPath());
    }
}
// --- META ---

DECLARE_CLASS(o2::ProjectConfig);
// --- END META ---
