add_compile_definitions(PLATFORM_WINDOWS SCRIPTING_BACKEND_JERRYSCRIPT _CRT_SECURE_NO_WARNINGS)

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

target_link_libraries(o2Framework 
    PRIVATE 
        Box2D
        jerry-core
        jerry-ext
        jerry-port-default
        libpng
        zlib
        FreeType
        mte::lib::pugixml
        mte::library
)        

if(WIN32)
    target_link_libraries(o2Framework PRIVATE Shlwapi.lib)  

    add_subdirectory(Framework/3rdPartyLibs/StackWalker)
    add_dependencies(o2Framework StackWalker)
    target_link_libraries(o2Framework PRIVATE StackWalker)
    set_target_properties(StackWalker PROPERTIES FOLDER o2/3rdPartyLibs)  

    add_custom_command(TARGET o2Framework PRE_BUILD
                      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CodeTool/Bin/CodeTool.exe -sources "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2")
endif()

if(MSVC)
    target_compile_options(o2Framework PRIVATE "/MP")
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
    target_compile_options(o2Editor PRIVATE "/MP")
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
