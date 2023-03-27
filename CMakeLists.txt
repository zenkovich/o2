# dependencies
set(BOX2D_BUILD_STATIC TRUE)
set(BOX2D_VERSION 1)
add_subdirectory(Framework/3rdPartyLibs/Box2D)

set(ENABLE_AMALGAM OFF)
set(JERRY_DEBUGGER ON)
set(JERRY_MEM_STATS ON)
set(JERRY_LOGGING ON)
set(JERRY_LINE_INFO ON)
set(JERRY_ERROR_MESSAGES ON)
set(JERRY_CMDLINE OFF)
add_subdirectory(Framework/3rdPartyLibs/jerryscript)

add_subdirectory(Framework/3rdPartyLibs/libpng)
add_subdirectory(Framework/3rdPartyLibs/zlib)
add_subdirectory(Framework/3rdPartyLibs/FreeType)
#add_subdirectory(Framework/3rdPartyLibs/pugixml)

# o2 Framework library
add_library(o2Framework STATIC)

add_compile_definitions(PLATFORM_WINDOWS SCRIPTING_BACKEND_JERRYSCRIPT _CRT_SECURE_NO_WARNINGS)

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

target_link_libraries(o2Framework PRIVATE Box2D)
target_link_libraries(o2Framework PRIVATE jerry-core)
target_link_libraries(o2Framework PRIVATE jerry-ext)
target_link_libraries(o2Framework PRIVATE jerry-port-default)
target_link_libraries(o2Framework PRIVATE libpng)
target_link_libraries(o2Framework PRIVATE zlib)
target_link_libraries(o2Framework PRIVATE FreeType)
#target_link_libraries(o2Framework PRIVATE pugixml)
target_link_libraries(o2Framework PRIVATE mte::lib::pugixml)        

if(WIN32)
 target_link_libraries(o2Framework PRIVATE Shlwapi.lib)

 add_subdirectory(Framework/3rdPartyLibs/StackWalker)
 add_dependencies(o2Framework StackWalker)
 target_link_libraries(o2Framework PRIVATE StackWalker)
 set_target_properties(StackWalker PROPERTIES FOLDER o2/3rdPartyLibs)
endif()


if(MSVC)
 target_compile_options(o2Framework PRIVATE "/MP")
endif()

# o2 Editor Library
add_library(o2Editor STATIC)

file(GLOB_RECURSE o2Framework_SOURCES "Editor/Sources/*.cpp" "Editor/Sources/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${o2Framework_SOURCES})

target_sources(o2Editor PRIVATE ${o2Framework_SOURCES})
target_include_directories(o2Editor PUBLIC "Editor/Sources")
target_include_directories(o2Editor PUBLIC ${o2Framework_INCLUDES})
target_precompile_headers(o2Editor PUBLIC "Editor/Sources/o2Editor/stdafx.h")

if(MSVC)
 target_compile_options(o2Editor PRIVATE "/MP")
endif()

# dependency
add_dependencies(o2Framework Box2D)
add_dependencies(o2Framework jerry-core)
add_dependencies(o2Framework jerry-ext)
add_dependencies(o2Framework jerry-port-default)
add_dependencies(o2Framework libpng)
add_dependencies(o2Framework zlib)
add_dependencies(o2Framework FreeType)
#add_dependencies(o2Framework pugixml)

add_dependencies(o2Editor o2Framework)

# group in IDe
set_target_properties(o2Framework PROPERTIES FOLDER o2)
set_target_properties(o2Editor PROPERTIES FOLDER o2)

set_target_properties(Box2D PROPERTIES FOLDER o2/3rdPartyLibs)
set_target_properties(jerry-core PROPERTIES FOLDER o2/3rdPartyLibs)
set_target_properties(jerry-ext PROPERTIES FOLDER o2/3rdPartyLibs)
set_target_properties(jerry-port-default PROPERTIES FOLDER o2/3rdPartyLibs)
set_target_properties(libpng PROPERTIES FOLDER o2/3rdPartyLibs)
set_target_properties(zlib PROPERTIES FOLDER o2/3rdPartyLibs)
set_target_properties(FreeType PROPERTIES FOLDER o2/3rdPartyLibs)
#set_target_properties(pugixml PROPERTIES FOLDER o2/3rdPartyLibs)
