#pragma once
#include "Utils/CommonTypes.h"

// Enables memory managing
#define ENALBE_MEMORY_MANAGE true

// Describes that engine running as editor
#define IS_EDITOR true

// Enables render debugging
#define RENDER_DEBUG true

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
