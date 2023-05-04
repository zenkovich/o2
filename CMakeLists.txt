project(o2)

# Common definitions
set(O2_COMPILE_DEFINITIONS SCRIPTING_BACKEND_JERRYSCRIPT _CRT_SECURE_NO_WARNINGS)

if (WIN32)
    list(APPEND O2_COMPILE_DEFINITIONS PLATFORM_WINDOWS)
elseif (UNIX)
    list(APPEND O2_COMPILE_DEFINITIONS PLATFORM_LINUX)
endif()

if (O2_IS_FILESYSTEM_EXPERIMENTAL)
    list(APPEND O2_COMPILE_DEFINITIONS O2_FILESYSTEM_EXPERIMENTAL)
endif()

if (O2_DONT_BUILD_ASSETS)
    list(APPEND O2_COMPILE_DEFINITIONS O2_DONT_BUILD_ASSETS)
endif()

if (O2_RENDER_API STREQUAL "gles2")
    list(APPEND O2_COMPILE_DEFINITIONS O2_RENDER_GLES2)
    list(APPEND O2_COMPILE_DEFINITIONS O2_DISABLE_PLATFORM)
endif()

set(O2_COMPILE_DEFINITIONS_EXPORT ${O2_COMPILE_DEFINITIONS} PARENT_SCOPE)
add_compile_definitions(${O2_COMPILE_DEFINITIONS})

# dependencies
add_subdirectory(Framework/3rdPartyLibs)

# o2 Framework library
add_library(o2Framework STATIC)

file(GLOB_RECURSE o2Framework_SOURCES "Framework/Sources/*.cpp" "Framework/Sources/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${o2Framework_SOURCES})

set(o2Framework_INCLUDES 
    "Framework/Sources"
    "Framework/3rdPartyLibs"
    "Framework/3rdPartyLibs/rapidjson/include"
    "Framework/3rdPartyLibs/FreeType/include"
    "Framework/3rdPartyLibs/jerryscript/jerry-core/include"
    "Framework/3rdPartyLibs/jerryscript"
    "Framework/"
)

target_sources(o2Framework PRIVATE ${o2Framework_SOURCES})
target_include_directories(o2Framework PUBLIC ${o2Framework_INCLUDES})
target_precompile_headers(o2Framework PUBLIC "Framework/Sources/o2/stdafx.h")

target_link_libraries(o2Framework PUBLIC
        Box2D
        jerry-core
        jerry-ext
        jerry-port-default
        libpng
        zlib
        FreeType
        pugixml
)        

if (UNIX)
    if (O2_RENDER_API STREQUAL "gles2")
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
        find_package(OpenGLES2 REQUIRED)
        target_link_libraries(o2Framework PRIVATE OpenGLES2::OpenGLES2)
    else()
        find_package(OpenGL REQUIRED COMPONENTS GLX)
        target_link_libraries(o2Framework PUBLIC OpenGL::GL OpenGL::GLX)
    endif()
endif()

if(WIN32)
    target_link_libraries(o2Framework PRIVATE Shlwapi.lib)
    target_link_libraries(o2Framework PRIVATE opengl32.lib)  

    add_subdirectory(Framework/3rdPartyLibs/StackWalker)
    add_dependencies(o2Framework StackWalker)
    target_link_libraries(o2Framework PRIVATE StackWalker)
    set_target_properties(StackWalker PROPERTIES FOLDER o2/3rdPartyLibs)  

    add_custom_command(TARGET o2Framework PRE_BUILD
                      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/CodeTool.exe -sources "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2")
endif()

if (UNIX)
    target_compile_options(o2Framework PRIVATE -Wno-pedantic)
elseif(MSVC)
    target_compile_options(o2Framework PRIVATE "/MP" "/Zc:__cplusplus")
endif()

# o2 Editor Library
add_library(o2Editor STATIC)

file(GLOB_RECURSE o2Framework_SOURCES "Editor/Sources/*.cpp" "Editor/Sources/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${o2Framework_SOURCES})

target_sources(o2Editor PRIVATE ${o2Framework_SOURCES})
target_include_directories(o2Editor 
    PUBLIC 
        "Editor/Sources"
        ${o2Framework_INCLUDES}
)

target_precompile_headers(o2Editor PUBLIC "Editor/Sources/o2Editor/stdafx.h")

if(MSVC)
    target_compile_options(o2Editor PRIVATE "/MP" "/Zc:__cplusplus")
endif()

if(WIN32)
    add_custom_command(TARGET o2Editor PRE_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/CodeTool.exe -sources "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Sources/o2Editor" -parent_projects "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2/CodeToolCache.xml")
endif()

# dependency
add_dependencies(o2Framework 
    Box2D
    jerry-core
    jerry-ext
    jerry-port-default
    libpng
    zlib
    FreeType)

add_dependencies(o2Editor o2Framework)

# group in IDE
set_target_properties(o2Framework PROPERTIES FOLDER o2)
set_target_properties(o2Editor PROPERTIES FOLDER o2)
