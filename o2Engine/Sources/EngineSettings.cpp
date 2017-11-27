#include "stdafx.h"
#include "EngineSettings.h"

o2::Platform GetEnginePlatform()
{
	return o2::Platform::Windows;
}

const char* GetProjectPath()
{
	return "../../../";
}

const char* GetProjectSettingFileLocalPath()
{
	return "ProjectSettings.xml";
}

bool IsStoppingOnLogErrors()
{
	return false;
}

bool IsUIDebugEnabled()
{
	return false;
}

bool IsDevMode()
{
	return true;
}

bool IsReleaseBuild()
{
#ifdef DEBUG
	return false;
#else
	return true;
#endif
}

bool IsAssetsPrebuildEnabled()
{
	return true;
}

const char* GetAssetsPath()
{
	return "Assets/";
}

const char* GetDataPath()
{
	return "../Data/";
}

const char* GetBasicAtlasPath()
{
	return "BasicAtlas.atlas";
}
