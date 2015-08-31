#pragma once

/*********************************/
/* Basic configuration section.  */
/*********************************/

/* Engine namespace. If not defined - no namespace. */
#define ENGINE_NAMESPACE_NAME o2

/* Using render system */
#define RENDER_OGL
//#define RENDER_D3D9C
//#define RENDER_GLES
//#define RENDER_GLES2

/* Using platform. */
#define PLATFORM_WIN
//#define PLATFORM_MAC
//#define PLATFORM_IOS

/* Basic memory allocator. Variations: 
 * -cStdAllocator: standard system allocator, see /util/mem_utils/std_allocator.h
 * 
 * BASIC_MEMORY_ALLOCATOR not defined - using standard system allocator */
//#define BASIC_MEMORY_ALLOCATOR cStdAllocator
//#define BASIC_MEMORY_ALLOCATOR_PARAMS 


/*********************************/
/* Debug configuration section.  */
/*********************************/

/* Using render debug info. */
#define RENDER_DEBUG

/* Log Level. 
 * 0 - not using log
 * 1 - using low log (only log() commands will be processed)
 * 2 - using hight log (all log commands will be processed) */
#define BASIC_LOG_LEVEL 2

/* Global log file. */
#define GLOBAL_LOG_FILENAME "log.txt"

/* Memory manager logging. */
#define MEMORY_TRACE

/* UI debug frames drawing. */
#define UI_DEBUG_FRAMES false

/* Debug checks and messages in containers. */
#define CONTAINERS_DEBUG false


/*********************************/
/* Project configuration section. */
/*********************************/

/* Project config file path. Relative from executable. */
#define PROJECT_CONFIG_FILE_PATH "../../../config"

/* Build config file path. Relative from executable. */
#define BUILD_CONFIG_FILE_PATH "../config"


/*********************************/
/* Assets configuration section. */
/*********************************/

/* Building assets before launching app. */
#define ASSETS_PREBUILDING_ENABLE true

/* Enabling saving assets from code. */
#define ASSETS_RUNTIME_EDITING_ENABLE true

/* Assets path. Relative from executable. */
#define ASSETS_PATH "../../../assets/"

/* Builded assets path. Relative from executable. */
#define ASSETS_BUILDED_PATH "../assets/"

/* Assets configuration file path. */
#define ASSETS_FOLDER_CONFIG_FILE_PATH "../assets_config"

/* Assets info file path. Relative from executable. */
#define ASSETS_INFO_FILE_PATH "../assets_info"

/* Assets builded info file path. */
#define ASSETS_BUILDED_INFO_FILE_PATH "../tmp/assets_build_info"