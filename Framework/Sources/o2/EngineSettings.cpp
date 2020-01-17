#include "o2/stdafx.h"
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
	return "";
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

const char* GetBuiltAssetsPath()
{
#if defined PLATFORM_WINDOWS
	return "BuiltAssets/Windows/Data/";
#elif defined PLATFORM_ANDROID
    return "AndroidAssets/BuiltAssets/";
#endif
}

const char* GetBasicAtlasPath()
{
	return "BasicAtlas.atlas";
}

const char* GetBuiltAssetsTreePath()
{
#if defined PLATFORM_WINDOWS
	return "BuiltAssets/Windows/Data.xml";
#elif defined PLATFORM_ANDROID
	return "AndroidAssets/AssetsTree.xml";
#endif
}

const char* GetEditorAssetsPath()
{
	return "o2/Editor/Assets/";
}

const char* GetEditorBuiltAssetsPath()
{
	return "BuiltAssets/Windows/EditorData/";
}

const char* GetEditorBuiltAssetsTreePath()
{
	return "BuiltAssets/Windows/EditorData.xml";
}

#ifdef PLATFORM_ANDROID

const char* GetAndroidAssetsPath()
{
    return "AndroidAssets/";
}

#endif
