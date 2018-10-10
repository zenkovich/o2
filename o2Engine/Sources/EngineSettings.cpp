#include "stdafx.h"
#include "EngineSettings.h"

o2::Platform GetEnginePlatform()
{
#ifdef PLATFORM_WINDOWS
	return o2::Platform::Windows;
#elif defined PLATFORM_ANDROID
    return o2::Platform::Android;
#endif
}

const char* GetProjectPath()
{
#if IS_EDITOR
	return "../../../";
#else
    return "AndroidAssets/";
#endif
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
#ifdef PLATFORM_WINDOWS
	return true;
#else
    return false;
#endif
}

const char* GetAssetsPath()
{
	return "Assets/";
}

const char* GetDataPath()
{
#if defined PLATFORM_WINDOWS
	return "../Data/";
#elif defined PLATFORM_ANDROID
    return "AndroidAssets/Data/";
#endif
}

const char* GetBasicAtlasPath()
{
	return "BasicAtlas.atlas";
}

const char* GetDataAssetsTreePath()
{
#if defined PLATFORM_WINDOWS
	return "../DataTree.xml";
#elif defined PLATFORM_ANDROID
	return "AndroidAssets/DataTree.xml";
#endif
}

#ifdef PLATFORM_ANDROID

const char* GetAndroidAssetsPath()
{
    return "AndroidAssets/";
}

#endif