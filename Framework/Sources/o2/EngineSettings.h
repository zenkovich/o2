#pragma once
#include "o2/Utils/Types/CommonTypes.h"

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

// Describes that engine running as editor
#define IS_EDITOR true

// Is enabled scripting engine
#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#define IS_SCRIPTING_SUPPORTED true
#else
#define IS_SCRIPTING_SUPPORTED false
#endif

// Current working platform
o2::Platform GetEnginePlatform();

// Returns current device type
o2::DeviceType GetDeviceType();

// Project config file path. Relative from executable
const char* GetProjectSettingPath();

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

// Basic atlas path (from assets path)
const char* GetBasicAtlasPath();

// Assets path. Relative from executable
const char* GetAssetsPath();

// Built assets path with built assets. Relative from executable
const char* GetBuiltAssetsPath();

// Built assets assets tree path
const char* GetBuiltAssetsTreePath();

// Editor's assets path. Relative from executable
const char* GetEditorAssetsPath();

// Editor's built assets path with built assets. Relative from executable
const char* GetEditorBuiltAssetsPath();

// Editor's built assets assets tree path
const char* GetEditorBuiltAssetsTreePath();


// ----------------------
// Platform configuration
// ----------------------

#ifdef PLATFORM_ANDROID

// Returns android assets path prefix
const char* GetAndroidAssetsPath();

#endif

// --------------------
// Other configurations
// --------------------

#ifndef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING 0
#endif
