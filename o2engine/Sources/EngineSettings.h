#pragma once

// TODO: Need create this configuration fully automatic and depending with project configuration

// Project config file path. Relative from executable
#define PROJECT_SETTINGS_FILE_PATH "../../ProjectSettings.xml"

// Enables stopping on log errors
#define ENABLE_STOPPING_ON_LOG_ERRORS true

// ----------------------------
// Assets configuration section
// ----------------------------

// Building assets before launching app
#define ASSETS_PREBUILDING_ENABLE true

// Assets path. Relative from executable
#define ASSETS_PATH "../../../Assets/"

// Data path with builded assets. Relative from executable
#define DATA_PATH "../Data/"
