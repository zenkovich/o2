project(o2)

set(CMAKE_CXX_STANDARD 20)

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

if (O2_RENDER_API STREQUAL "gles2")
    list(APPEND O2_COMPILE_DEFINITIONS O2_RENDER_GLES2)
    list(APPEND O2_COMPILE_DEFINITIONS O2_DISABLE_PLATFORM)
endif()

if (O2_SELFPROFILE)
    list(APPEND O2_COMPILE_DEFINITIONS O2_PROFILE_STATS)
endif()

if (O2_EDITOR)
    list(APPEND O2_COMPILE_DEFINITIONS O2_EDITOR_ENABLED)
endif()

set(O2_COMPILE_DEFINITIONS_EXPORT ${O2_COMPILE_DEFINITIONS} PARENT_SCOPE)
add_compile_definitions(${O2_COMPILE_DEFINITIONS})

# warings and errors
if (MSVC)
    add_compile_options(/W2)
else()
    add_compile_options(-Wall)
endif()

# dependencies
add_subdirectory(Framework/3rdPartyLibs)

# code tool
add_subdirectory(CodeTool)

# o2 copy builtin framework assets
if (UNIX)
    set(O2_ASSETS_PLATFORM "Linux")
elseif (WIN32)
    set(O2_ASSETS_PLATFORM "Windows")
endif()

add_custom_target(
    o2CopyBuiltinAssets
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Assets"
        "${CMAKE_CURRENT_SOURCE_DIR}/../BuiltAssets/${O2_ASSETS_PLATFORM}/FrameworkData"
    COMMENT "Copy o2 builtin assets"
)

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
    target_link_libraries(o2Framework PUBLIC ${X11_LIBRARIES})

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
                      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/o2CodeTool -project o2Framework -sources "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2"
                      COMMENT "Run CodeTool: ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/o2CodeTool -project o2Framework -sources \"${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2\"")
endif()

if (UNIX)
    target_compile_options(o2Framework PRIVATE -Wno-pedantic)
elseif(MSVC)
    target_compile_options(o2Framework PUBLIC "/MP" "/Zc:__cplusplus")
    target_sources(o2Framework PRIVATE "Framework/Platforms/Windows/Framework.natvis")
endif()

add_dependencies(o2Framework o2CopyBuiltinAssets)
add_dependencies(o2Framework o2CodeTool)

# o2 Editor Library
if (O2_EDITOR)
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
                           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/o2CodeTool 
                                   -project o2Editor 
                                   -sources "${CMAKE_CURRENT_SOURCE_DIR}/Editor/Sources/o2Editor" 
                                   -parent_projects "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2/CodeToolCache.xml"
                           COMMENT "Run CodeTool: ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/o2CodeTool "
                                   "-project o2Editor "
                                   "-sources \"${CMAKE_CURRENT_SOURCE_DIR}/Editor/Sources/o2Editor\" "
                                   "-parent_projects \"${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2/CodeToolCache.xml\"")
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
    set_target_properties(o2Editor PROPERTIES FOLDER o2)
endif()

# assets build tool
add_subdirectory(AssetsBuildTool)

# group in IDE
set_target_properties(o2Framework PROPERTIES FOLDER o2)
set_target_properties(o2AssetsBuilder PROPERTIES FOLDER o2)
set_target_properties(o2CopyBuiltinAssets PROPERTIES FOLDER o2)
set_target_properties(o2CodeTool PROPERTIES FOLDER o2)

