#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

/* 
 * Engine settings. 
 */

/* Engine namespace. If not defined - no namespace. */
#define ENGINE_NAMESPACE_NAME o2

/* Using render system */
//#define RENDER_OGL
#define RENDER_D3D9C
//#define RENDER_GLES
//#define RENDER_GLES2

/* Using render debug info. */
#define RENDER_DEBUG

/* Using platform. */
#define PLATFORM_WIN
//#define PLATFORM_MAC
//#define PLATFORM_IOS

/* Log Level. 
 * 0 - not using log
 * 1 - using low log (only log() commands will be processed)
 * 2 - using hight log (all log commands will be processed) */
#define BASIC_LOG_LEVEL 2

/* Global log file. */
#define GLOBAL_LOG_FILENAME "log.txt"

/* Memory manager logging. */
#define MEMORY_TRACE

/* Basic memory allocator. Variations: 
 * -cStdAllocator: standart system allocator, see /util/mem_utils/std_allocator.h
 * 
 * BASIC_MEMORY_ALLOCATOR not defined - using standart system allocator */
//#define BASIC_MEMORY_ALLOCATOR cStdAllocator
//#define BASIC_MEMORY_ALLOCATOR_PARAMS 


#endif //ENGINE_SETTINGS_H