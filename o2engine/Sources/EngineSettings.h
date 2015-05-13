#pragma once

/**********************************/
/** Project configuration section. */
/**********************************/

/** Project config file path. Relative from executable. */
#define PROJECT_CONFIG_FILE_PATH "../../../config"

/** Build config file path. Relative from executable. */
#define BUILD_CONFIG_FILE_PATH "../config"


/**********************************/
/** Assets configuration section. */
/**********************************/

/** Building assets before launching app. */
#define ASSETS_PREBUILDING_ENABLE true

/** Enabling saving assets from code. */
#define ASSETS_RUNTIME_EDITING_ENABLE true

/** Assets path. Relative from executable. */
#define ASSETS_PATH "../../../assets/"

/** Builded assets path. Relative from executable. */
#define ASSETS_BUILDED_PATH "../assets/"

/** Assets configuration file path. */
#define ASSETS_FOLDER_CONFIG_FILE_PATH "../assets_config"

/** Assets info file path. Relative from executable. */
#define ASSETS_INFO_FILE_PATH "../assets_info"

/** Assets builded info file path. */
#define ASSETS_BUILDED_INFO_FILE_PATH "../tmp/assets_build_info"
