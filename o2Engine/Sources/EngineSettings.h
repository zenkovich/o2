#pragma once
#include "Utils/Types/CommonTypes.h"

// Enables memory managing
#if defined DEBUG
#define ENALBE_MEMORY_MANAGE true
#else
#define ENALBE_MEMORY_MANAGE false
#endif

// Enables render debugging
#if defined DEBUG
#define RENDER_DEBUG true
#else
#define RENDER_DEBUG false
#endif

#define _HAS_ITERATOR_DEBUGGING 0

// Describes that engine running as editor
#define IS_EDITOR true

// Current working platform
o2::Platform GetEnginePlatform();

// Project location path. Relative from executable
const char* GetProjectPath();

// Project config file path. Relative from project path
const char* GetProjectSettingFileLocalPath();

// Enables stopping on log errors
bool IsStoppingOnLogErrors();

// Enables debug ui rectangles drawing
bool IsUIDebugEnabled();

// Is development mode
bool IsDevMode();

// Is build release
bool IsReleaseBuild();

// ----------------------------
// Assets configuration section
// ----------------------------

// Building assets before launching app
bool IsAssetsPrebuildEnabled();

// Assets path. Relative from project path
const char* GetAssetsPath();

// Data path with built assets. Relative from executable
const char* GetDataPath();

// Basic atlas path (from assets path)
const char* GetBasicAtlasPath();

// Data assets tree path
const char* GetDataAssetsTreePath();


// ----------------------
// Platform configuration
// ----------------------

#ifdef PLATFORM_ANDROID

// Returns android assets path prefix
const char* GetAndroidAssetsPath();

#endif
