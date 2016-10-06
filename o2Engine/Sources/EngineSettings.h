#pragma once

// TODO: Need to make this configuration fully automatic and depending with project configuration

// Current working platform
#define PLATFORM ProjectConfig::Platform::Windows

// Project location path. Relative from executable
#define PROJECT_PATH "../../../"

// Project config file path. Relative from project path
#define PROJECT_SETTINGS_FILE_LOCAL_PATH "ProjectSettings.xml"

// Enables stopping on log errors
#define ENABLE_STOPPING_ON_LOG_ERRORS false

// Enables render debugging
#define RENDER_DEBUG true

// Enables debug ui rectangles drawing
#define UI_DEBUG false

// Enables memory managing
#define ENALBE_MEMORY_MANAGE false

// Describes that engine running as editor
#define IS_EDITOR true

// Is development mode
#define IS_DEV_MODE true

// ----------------------------
// Assets configuration section
// ----------------------------

// Building assets before launching app
#define ASSETS_PREBUILDING_ENABLE true

// Assets path. Relative from project path
#define ASSETS_PATH "Assets/"

// Data path with builded assets. Relative from executable
#define DATA_PATH "../Data/"

// Basic atlas path (from assets path)
#define BASIC_ATLAS_PATH "BasicAtlas.atlas"

